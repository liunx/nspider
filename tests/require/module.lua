--
-- we can use require to keep just one module instance
--
local module = {}
local table = {}

local function uuid ()
    local f = io.open('/proc/sys/kernel/random/uuid', 'r')
    local id = f:read('*l')
    f:close()
    return id
end

function module.insert (k, v)
    table[k] = v
end

function module.get (k)
    return table[k]
end

function module.uuid ()
    local id = uuid()
    print(id)
end

return module
