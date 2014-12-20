-- 
-- task for iwifi
--
local pipeline = require('pipeline')

local function getgwaddr (id)
    local addr = pipeline.popen(id, 'IF=`route -n | grep -m 1 "^0.0.0.0" | awk \'{ print $8 }\'`;ifconfig $IF |grep "inet addr:" | sed \'s/\\(^.*\\)addr:\\(.*\\) Bcast\\(.*\\)/\\2/g\'')
    addr = string.gsub(addr, "[ \n]", "")
    return val
end

local function iwifi_wait (id)
    print('iwifi_wait begin...')
    local data = pipeline.read(id)
    pipeline.sleep(id, 3000)
    pipeline.write(id, data)
    data = pipeline.read(id)
    pipeline.sleep(id, 9000)
    pipeline.write(id, data)

    local cid = pipeline.connect(id, 'localhost', 8000)
    if cid ~= nil then
        pipeline.write(cid, 'hello from nspider!')
        data = pipeline.read(cid)
        pipeline.write(id, data)
        pipeline.close(cid)
        pipeline.complete(cid)
    end

    pipeline.close(id)
    pipeline.complete(id)
    print('iwifi_wait end...')
end

local function iwifi_heartbeat (id)
    local addr
    for i = 1,100 do
        pipeline.sleep(id, 1000)
        addr = getgwaddr(id)
        print(addr)
    end
end

local function iwifi_heartbeat2 (id)
    for i = 1,100 do
        pipeline.sleep(id, 2000)
        print('sleep...' .. i)
    end
end

local function iwifi_signal (id)
    while true do
        pipeline.signal(id, 10)
        print('get signal...')
    end
end

local function iwifi_popen (id)
    local data = pipeline.popen(id, 'echo hello')
    print(data)
    data = pipeline.popen(id, 'ping -c 1 www.baidu.com')
    print(data)
end

local function iwifi_exit ()
    print('iwifi exiting...')
end

local function init ()
    pipeline.do_exit(iwifi_exit)
    --[[
    pipeline.listen('0.0.0.0', 8080, iwifi_wait)
    pipeline.coroutine(iwifi_heartbeat)
    --pipeline.coroutine(iwifi_heartbeat2)
    pipeline.coroutine(iwifi_signal)
    pipeline.coroutine(iwifi_popen)
    --]]
end

-- start
init()
