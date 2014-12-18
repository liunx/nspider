-- 
-- task for iwifi
--
pipeline = require('pipeline')

local function iwifi_wait (id)
    local data = pipeline.read(id)
    pipeline.write(id, data)
    pipeline.yield()
    data = pipeline.read(id)
    pipeline.write(id, data)
    --pipeline.close(id)

    print('connect...')
    local cid = pipeline.connect(id, '127.0.0.1', 8000)
    pipeline.yield()
    print('write...')
    pipeline.write(cid, 'hello from nspider!')
    pipeline.yield()
    data = pipeline.read(cid)
    print(data)
    pipeline.write(id, data)
    pipeline.close(cid)
    pipeline.close(id)

    pipeline.complete(id)
    pipeline.complete(cid)
end

--local iwifi_wait_t = { type = 'tcp', addr = '0.0.0.0', port = 8080, handler = iwifi_wait }
--pipeline.wait(iwifi_wait_t)
pipeline.listen('0.0.0.0', 8080, iwifi_wait)
