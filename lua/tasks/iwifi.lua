--
-- task for iwifi
--
local pipeline = require('pipeline')
local http = require('http')
local url = require('url')
local json = require('json')
local html = require('html')
require('iwifi_cfg')

local http_response = {
    msgtype = 'RESPONSE',
    version = 'HTTP/1.1',
    status = '200 OK',
    contents = {
        Connection = 'close',
    }
}
http_response['contents']['Content-Type'] = 'text/html; charset=iso-8859-1'
http_response['contents']['Content-Length'] = 0

local http_request = {
    msgtype = 'REQUEST',
    version = 'HTTP/1.1',
    method = 'GET',
    contents = {
        Connection = 'close',
    }
}
http_request['contents']['Content-Type'] = 'text/html; charset=iso-8859-1'
http_request['contents']['Content-Length'] = 0
http_request['contents']['Accept-Encoding'] = 'identity'
http_request['contents']['User-Agent'] = 'SmartWiFi'

--
-- utils
--
local function get_devid (id)
    return '123456789'
end

local function get_clientmac (id)
    return '601020304000'
end

local function get_gwaddress (id)
    return '10.129.228.66'
end

local function get_gwmac (id)
    return '601020304000'
end

local function get_sysinfo (id)
    local t = {
        dev_id = '123456',
    }

    return t
end

--
-- coroutines
--
local function iwifi_request(id, addr, port, req)
    local data
    local t = {}
    local cid = pipeline.connect(id, addr, port)
    if cid == nil then
        goto exit2
    end

    pipeline.write(cid, req)
    data = pipeline.read(cid)
    if data == nil then
        goto exit1
    end
    t = http.parse(data)

    ::exit1::
    pipeline.close(cid)
    ::exit2::
    pipeline.complete(cid)
    return t
end

local function refresh_session_key (id)
    local sk, node
    http_request['method'] = 'GET'
    http_request['url'] = '/reflashSessionKey.html?rndnum=0.123456789'
    http_request['contents']['Host'] = '192.168.1.1'
    http_request['contents']['Connection'] = 'keep-alive'
    local http_msg = http.create(http_request)
    local http_t = iwifi_request(id, '192.168.1.1', 80, http_msg)
    if http_t  == nil then
        return nil
    end
    if http_t['body'] == nil then
        return nil
    end
    sk = html.parsestr(http_t['body'])
    if sk == nil then
        return nil
    end
    node = html.findtag(sk, 'script')
    if type(node) == 'table' then
        sk = string.match(node[1], '([0-9]+)')
    end
    return sk
end

local function gw_active (id, url)
    local data
    local t = {}
    local cid = pipeline.connect(id, auth[1], auth[2])
    if cid == nil then
        goto exit2
    end
    http_request['url'] = string.format("/api10/register?dev_id=%s&mac=%s&stage=active", url['dev_id'], get_gwmac(id))
    http_request['contents']['Host'] = string.format("%s:%d", auth[1], auth[2])
    
    pipeline.write(cid, req)
    data = pipeline.read(cid)
    if data == nil then
        goto exit1
    end
    t = json.parse(data)

    ::exit1::
    pipeline.close(cid)
    ::exit2::
    pipeline.complete(cid)
    return t
end

local function gw_save_devid (id, dev_id)
    local sk = refresh_session_key(id)
    http_request['url'] = string.format("/setparams.cmd?node=iwifi&dev_id=%s&sessionKey=%s&rndnum=0.123456789", dev_id, sk)
    local http_msg = http.create(http_request)
    local resp = iwifi_request(id, '192.168.1.1', 80, http_msg)
    print(resp['body'])
end

local function iwifi_server (id)
    local data = pipeline.read(id)
    local result
    if data then
        local t = http.parse(data)
        if type(t) == 'table' then
            if t['url'] == nil then
                goto exit
            end
            local url_l = url.parse(t['url'])
            if type(url_l) ~= 'table' then
                goto exit
            end
            if url_l['cmd'] == '/smartwifi/active' then
                if url_l['dev_id'] == nil then
                    pipeline.write(id, "dev_id not FOUND")
                    goto exit
                end
                gw_save_devid(id, url_l['dev_id'])
                result = gw_active(id, url_l['dev_id'])
                print(result)
                if type(result) ~= 'table' then
                    -- TODO add error response
                    goto exit
                end
                -- TODO check result['result']
            end
        end
    end
    ::exit::
    pipeline.close(id)
    pipeline.complete(id)
end

local function iwifi_redirect (id)
    local data = pipeline.read(id)
    if data then
        local t = http.parse(data)
        if type(t) == 'table' then
            local host = t['contents']['Host']
            http_response['status'] = '302 Moved Temporarily'
            local url_l = {}
            url_l['dev_id'] = get_devid(id)
            url_l['client_mac'] = get_clientmac(id)
            url_l['gw_id'] = gwinfo['gwid']
            url_l['gw_port'] = gwinfo['gwport']
            url_l['gw_address'] = get_gwaddress(id)
            url_l['url'] = host
            local url_d = url.create(url_l)
            http_response['contents']['Location'] = (string.format("http://%s:%d/api10/login?%s", portal[1], portal[2], url_d))
            local rspmsg = http.create(http_response)
            pipeline.write(id, rspmsg)
        end
    end
    pipeline.close(id)
    pipeline.complete(id)
end

local function gw_register (id, t)
    http_response['status'] = '302 Moved Temporarily'
    local url_l = {}
    url_l['gw_mac'] = get_gwmac(id)
    url_l['gw_port'] = gwinfo['gwport']
    url_l['gw_address'] = get_gwaddress(id)
    url_l['soft_ver'] = gwinfo['softver']
    local url_d = url.create(url_l)
    http_response['contents']['Location'] = (string.format("http://%s:%d/api10/register.htm?%s", www[1], www[2], url_d))
    local rspmsg = http.create(http_response)
    pipeline.write(id, rspmsg)
end

local function iwifi_manager (id)
    local data = pipeline.read(id)
    if data then
        local t = http.parse(data)
        if type(t) == 'table' then
            gw_register(id, t)
        end
    end
    pipeline.close(id)
    pipeline.complete(id)
end

local function iwifi_wificli (id)
end

local function iwifi_gwheartbeat (id)
    while true do
        local info_t = get_sysinfo(id)
        local url_l = url.create(info_t)
        print(url_l)
        http_request['url'] = string.format("/api10/ping?%s", url_l)
        local http_msg = http.create(http_request)
        print(http_msg)
        local http_t = iwifi_request(id, auth[1], auth[2], http_msg)
        -- TODO check return val
        pipeline.sleep(id, (gwinfo['gwheartbeat'] * 1000))
    end
end

local function iwifi_cliheartbeat (id)
    while true do
        print('cli heartbeat...')
        pipeline.sleep(id, (gwinfo['cliheartbeat'] * 1000))
    end
end

local function iwifi_cliexpire (id)
    while true do
        print('cli expire...')
        pipeline.sleep(id, (gwinfo['cliexpire'] * 1000))
    end
end

-- for debugging
local function iwifi_debug (id)
    local data = pipeline.read(id)
    if string.match(data, 'devid') then
        gw_save_devid(id, '123456789')
    end

    pipeline.close(id)
    pipeline.complete(id)
end

local tasks = {
    SERVER = iwifi_server,
    REDIRECT = iwifi_redirect,
    MANAGER = iwifi_manager,
    WIFICLI = iwifi_wificli,
}

local function exit ()
end

local function init ()
    local task
    local fun
    for i = 1,#listen do
        task = listen[i]
        fun = tasks[task[3]]
        if type(fun) == 'function' then
            pipeline.listen(task[1], task[2], fun)
        end
    end
    pipeline.coroutine(iwifi_gwheartbeat)
    pipeline.coroutine(iwifi_cliheartbeat)
    pipeline.coroutine(iwifi_cliexpire)
    pipeline.listen('0.0.0.0', 1234, iwifi_debug)

    pipeline.do_exit(exit)
end

init()
