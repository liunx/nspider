--
-- task for iwifi
--
local pipeline = require('pipeline')
local http = require('http')
local url = require('url')
local json = require('json')
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

local function dump (t)
    for k, v in pairs(t) do
        print(k,v)
    end
end

--
-- coroutines
--
local function iwifi_server (id)
end

local function iwifi_redirect (id)
    local data = pipeline.read(id)
    print(data)
    if data then
        local t = http.parse(data)
        dump(t)
        if type(t) == 'table' then
            local host = t['contents']['Host']
            print(host)
            http_response['status'] = '302 Moved Temporarily'
            local url_l = {}
            url_l['dev_id'] = get_devid(id)
            url_l['client_mac'] = get_clientmac(id)
            url_l['gw_id'] = gwinfo['gwid']
            url_l['gw_port'] = gwinfo['gwport']
            url_l['gw_address'] = get_gwaddress(id)
            url_l['url'] = host
            local url_d = url.create(url_l)
            print(url_d)
            http_response['contents']['Location'] = (string.format("http://%s:%d/api10/login?%s", portal[1], portal[2], url_d))
            local rspmsg = http.create(http_response)
            print(rspmsg)
            pipeline.write(id, rspmsg)
        end
    end
    pipeline.close(id)
    pipeline.complete(id)
end

local function iwifi_manager (id)
end

local function iwifi_wificli (id)
end

local function iwifi_gwheartbeat (id)
    while true do
        pipeline.sleep(id, (gwinfo['gwheartbeat'] * 1000))
    end
end

local function iwifi_cliheartbeat (id)
    while true do
        pipeline.sleep(id, (gwinfo['cliheartbeat'] * 1000))
    end
end

local function iwifi_cliexpire (id)
    while true do
        pipeline.sleep(id, (gwinfo['cliexpire'] * 1000))
    end
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

    pipeline.do_exit(exit)
end

init()
