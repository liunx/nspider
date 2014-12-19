-- 
-- task for iwifi
--
pipeline = require('pipeline')

local function iwifi_wait (id)
    print('iwifi_wait begin...')
    local data = pipeline.read(id)
    pipeline.sleep(id, 3000)
    pipeline.write(id, data)
    data = pipeline.read(id)
    pipeline.sleep(id, 9000)
    pipeline.write(id, data)

    --[[
    local cid = pipeline.connect(id, '127.0.0.1', 8000)
    if cid ~= nil then
        pipeline.write(cid, 'hello from nspider!')
        data = pipeline.read(cid)
        pipeline.write(id, data)
        pipeline.close(cid)
        pipeline.complete(cid)
    end
    --]]

    pipeline.close(id)
    pipeline.complete(id)
    print('iwifi_wait end...')
end

local function iwifi_heartbeat (id)
    for i = 1,100 do
        pipeline.sleep(id, 1000)
        print('sleep...' .. i)
    end
end

local function iwifi_heartbeat2 (id)
    for i = 1,100 do
        pipeline.sleep(id, 2000)
        print('sleep...' .. i)
    end
end

pipeline.listen('0.0.0.0', 8080, iwifi_wait)
pipeline.coroutine(iwifi_heartbeat)
pipeline.coroutine(iwifi_heartbeat2)

