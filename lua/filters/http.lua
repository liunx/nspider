local http = {}

local http_head = {
    GET = true,
    PUT = true,
    POST = true,
    HEAD = true,
    OPTIONS = true,
    DELETE = true,
    TRACE = true,
    CONNECT = true,
    PATCH = true
}

local http_msgtype = {
    REQUEST = true,
    RESPONSE = true
}

function http.parse (data)
    local t = {}
    -- check http features
    local head, tail
    head, tail = string.find(data, "HTTP/1.1")

    if head == nil then
        head, tail = string.find(data, "HTTP/1.0")
    end

    if head == nil then
        return nil
    end

    local crlf_h, crlf_t
    crlf_h, crlf_t = string.find(data, "\r\n\r\n")
    if crlf_h == nil then
        -- not a valid http tail
        return nil
    end
    local msghead = string.sub(data, 1, crlf_h - 1)
    local msgbody = string.sub(data, crlf_t + 1)
    t['body'] = msgbody
    
    local msgtype = 'REQUEST'
    if head == 1 then
        msgtype = 'RESPONSE'
    end
    t['msgtype'] = msgtype
    t['version'] = 'HTTP/1.1'

    if msgtype == 'REQUEST' then
        print(msghead)
        local head_1 = head
        head, tail = string.find(msghead, " ")
        print(head, tail)
        local tail_2 = tail

        local method = string.sub(msghead, 1, head - 1)
        if http_head[method] == nil then
            return nil
        end
        t['method'] = method
        local url = string.sub(msghead, tail_2 + 1, head_1 - 2) 
        t['url'] = url

        local content_t = {}
        for k, v in string.gmatch(msghead, "([%w-]+): ([^\r\n]+)") do
            content_t[k] = v
        end
        t['contents'] = content_t
    elseif msgtype == 'RESPONSE' then
        local tail_1 = tail
        head, tail = string.find(msghead, "\r\n")
        local head_1 = head
        if head > 1 then
            local status = string.sub(msghead, tail_1 + 1, head_1 - 1)
            t['status'] = status
        end
        local content_t = {}
        for k, v in string.gmatch(msghead, "([%w-]+): ([^\r\n]+)") do
            content_t[k] = v
        end
        t['contents'] = content_t
    end
    return t
end

function http.create (t)
    local data
    local msgtype = t['msgtype']

    if http_msgtype[msgtype] == nil then
        return nil
    end

    if msgtype == 'REQUEST' then
        data = string.format("%s %s %s\r\n", t['method'], t['url'], t['version'])
    elseif msgtype == 'RESPONSE' then
        data = string.format("%s %s\r\n", t['version'], t['status'])
    end

    local contents = t['contents']
    if type(contents) == 'table' then
        for k, v in pairs(contents) do
            data = data .. string.format("%s: %s\r\n", k, v)
        end
    end
    local body = t['body']
    if body ~= nil then
        data = data .. string.format("Content-Length: %d\r\n", string.len(body))
        data = data .. "\r\n"
        data = data .. body
    else
        data = data .. "\r\n"
    end
    return data
end

return http
