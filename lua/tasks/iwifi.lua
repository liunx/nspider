-- 
-- task for iwifi
--
pipeline = require('pipeline')

local function iwifi_wait (id)
    print('iwifi_wait begin...')
    local data = pipeline.read(id)
    pipeline.write(id, data)
    data = pipeline.read(id)
    pipeline.write(id, data)

    local cid = pipeline.connect(id, '127.0.0.1', 8000)
    pipeline.write(cid, 'hello from nspider!')
    data = pipeline.read(cid)
    pipeline.write(id, data)
    pipeline.close(cid)
    pipeline.close(id)

    pipeline.complete(id)
    pipeline.complete(cid)
    print('iwifi_wait end...')
end

pipeline.listen('0.0.0.0', 8080, iwifi_wait)
