--
-- we can use require to keep just one module instance
--
local module = {}
local table = {}

function module.insert (k, v)
    table[k] = v
end

function module.get (k)
    return table[k]
end

return module
