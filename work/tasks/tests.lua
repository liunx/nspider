-- 
-- task for tests
--
local pipeline = require('pipeline')
local utils = require('utils')

local function getgwaddr (id)
    local addr = pipeline.popen(id, 'IF=`route -n | grep -m 1 "^0.0.0.0" | awk \'{ print $8 }\'`;ifconfig $IF |grep "inet addr:" | sed \'s/\\(^.*\\)addr:\\(.*\\) Bcast\\(.*\\)/\\2/g\'')
    addr = string.gsub(addr, "[ \n]", "")
    return val
end

local function tests_wait (id)
    print('tests_wait begin...')
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
    print('tests_wait end...')
end

local function tests_heartbeat (id)
    local addr
    for i = 1,100 do
        pipeline.sleep(id, 1000)
        addr = getgwaddr(id)
        print(addr)
    end
end

local function tests_heartbeat2 (id)
    for i = 1,100 do
        pipeline.sleep(id, 2000)
        print('sleep...' .. i)
    end
end

local function tests_signal (id)
    while true do
        pipeline.signal(id, 10)
        print('get signal...')
    end
end

local function tests_popen (id)
    local data = pipeline.popen(id, 'echo hello')
    print(data)
    data = pipeline.popen(id, 'ping -c 1 www.baidu.com')
    print(data)
end

local function tests_listen (id)
    local data = pipeline.read(id)

    if string.match(data, 'getpeername') then
        print(pipeline.getpeername(id))
    elseif string.match(data, 'getsockname') then
        print(pipeline.getsockname(id))
    elseif string.match(data, 'getifaceinfo') then
        local t = utils.get_default_route()
        utils.tprint(t, 0)
    end

    pipeline.close(id)
    pipeline.complete(id)
end

local function tests_exit ()
    print('tests exiting...')
end

local function init ()
    pipeline.do_exit(tests_exit)
    pipeline.listen('0.0.0.0', 8080, tests_listen)
    --[[
    pipeline.coroutine(tests_heartbeat)
    --pipeline.coroutine(tests_heartbeat2)
    pipeline.coroutine(tests_signal)
    pipeline.coroutine(tests_popen)
    --]]
end

-- start
init()
