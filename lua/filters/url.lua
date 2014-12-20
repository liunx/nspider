-- url filters

local url = {}

function url.parse (url)
    local t = {}
    local hd, tl
    hd, tl = string.find(url, '?')
    if hd == nil then
        return nil
    end

    t['cmd'] = string.sub(url, 1, hd - 1)
    for k, v in string.gmatch(url, "([^?&]+)=([^?&]+)") do
        t[k] = v
    end

    return t
end

function url.create (t)
    local url = ""
    for k, v in pairs(t) do
        url = url .. string.format("%s=%s&", k, v)
    end
    url = string.sub(url, 1, #url - 1)
    return url
end

return url
