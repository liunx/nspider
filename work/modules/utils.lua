
local utils = {}
function utils.tprint (tbl, indent)
    if not indent then indent = 0 end
    for k, v in pairs(tbl) do
        formatting = string.rep("  ", indent) .. k .. ": "
        if type(v) == "table" then
            print(formatting)
            tprint(v, indent+1)
        elseif type(v) == 'boolean' then
            print(formatting .. tostring(v))      
        else
            print(formatting .. v)
        end
    end
end

function utils.get_default_gw ()
    local found = 0
    local gwinfo = {
        gwaddr = nil,
        iface = nil,
        mac = nil,
        inet_addr = nil,
        mask = nil,
        bcast = nil,
        rx = nil,
        tx = nil,
    }

    local t = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11}
    local f = io.open('/proc/net/route', 'r')
    local i
    for line in f:lines() do
        i = 1
        for w in string.gmatch(line, "[%a%d]+") do
            t[i] = w
            i = i + 1
        end
        if t[4] == '0003' and t[7] == '0' then
            found = 1
            break
        end
    end
    f:close()
    if found == 1 then
        gwinfo['iface'] = t[1]
        local gwaddr = t[3]
        gwinfo['gwaddr'] = string.format("%d.%d.%d.%d",
        tonumber(string.sub(gwaddr, 7, 8), 16),
        tonumber(string.sub(gwaddr, 5, 6), 16),
        tonumber(string.sub(gwaddr, 3, 4), 16),
        tonumber(string.sub(gwaddr, 1, 2), 16))
        f = io.open('/sys/class/net/' .. gwinfo['iface'] .. '/address', 'r')
        gwinfo['mac'] = f:read('*l')
        f:close()
        print(gwinfo['mac'])
    end
end

utils.get_default_gw()
return utils
