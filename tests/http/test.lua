-- http.lua

package.path = package.path .. ";../../lua/filters/?.lua"

local function dump (t)
    for k, v in pairs(t) do
        if type(v) == 'table' then
            for k1, v1 in pairs(v) do
                print('[' .. k1 .. ':' .. v1 .. ']')
            end
        else
            print('[' .. k .. ':' .. v .. ']')
        end
    end
end

local s = "GET /index.html HTTP/1.1\r\n"
s = s ..  "Host: localhost\r\n"
s = s ..  "User-Agent: firefox\r\n"
s = s ..  "Connection: keep-alive\r\n"
s = s .. "\r\n"

local get = "GET /css/news070130.css HTTP/1.1\r\n"
get = get .. "Accept: */*\r\n"
get = get .. "Referer: http://news.qq.com/a/20081224/000088.htm\r\n"
get = get .. "Accept-Language: zh-cn\r\n"
get = get .. "Accept-Encoding: gzip, deflate\r\n"
get = get .. "If-Modified-Since: Thu, 29 Mar 2007 02:05:43 GMT; length=11339\r\n"
get = get .. "User-Agent: Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; SV1; .NET CLR 1.1.4322; .NET CLR 2.0.50727)\r\n"
get = get .. "Host: news.qq.com\r\n"
get = get .. "Connection: Keep-Alive\r\n"
get = get .. "Cookie: ssid=idlsesels; flv=9.0; icache=ADLFMBGC; adid=adsdifids; isflucky_50829637=0; \r\n"
get = get .. "\r\n"

local post = "POST /Login.php HTTP/1.1\r\n"
post = post .. "Accept: image/gif，*/*\r\n"
post = post .. "Accept-language: zh-cn\r\n"
post = post .. "Accept-encodeing: gzip\r\n"
post = post .. "User-Agent: MSIE6.0\r\n"
post = post .. "Host: www.some.com\r\n"
post = post .. "Connection: Keep-Alive\r\n"
post = post .. "Content-Length: 7\r\n"
post = post .. "Cache-Control:no-cache\r\n"
post = post .. "Cookie: name1=value1; name2=value2;\r\n"
post = post .. "\r\n"
post = post .. "username=b&passwd=d"

local status = "HTTP/1.1 200 OK\r\n"
status = status .. "Cache-Control: private, max-age=0\r\n"
status = status .. "Date: Fri, 02 Jan 2009 12:26:17 GMT\r\n"
status = status .. "Expires: -1\r\n"
status = status .. "Content-Type: text/html; charset=GB2312\r\n"
status = status .. "Set-Cookie: PREF=ID=7bbe374f53b6c6a8:NW=1:TM=1230899177:LM=1230899177:S=2EGHuZJnrtdQUB_A; expires=Sun, 02-Jan-2011 12:26:17 GMT; path=/; domain=.google.com\r\n"
status = status .. "Server: gws\r\n"
status = status .. "Transfer-Encoding: chunked\r\n"
status = status .. "Connection: Close\r\n"
status = status .. "\r\n"
status = status .. "<html>"
status = status .. "html数据"
status = status .. "</html>"

local wrong = "    HTTP/1.1 300 \r\n"
wrong = wrong .. "\r\n"
--[[
local t = {}
for k, v in string.gmatch(s, "([%w-]+): ([%w-]+)") do
    t[k] = v
end

for k, v in pairs(t) do
    print(k, v)
end

-- find the first line
e,_ = string.find(s, "\n")
local head = string.sub(s, 1, e - 1)
print(head)

for w in string.gmatch(head, "[^%s]+") do
    print(w)
end
--]]

local hp = require("http")
print(s)
local t = hp.parse(s)
dump(t)
print(get)
t = hp.parse(get)
dump(t)
print(post)
t = hp.parse(post)
dump(t)
print(status)
t = hp.parse(status)
dump(t)

print(wrong)
t = hp.parse(wrong)
if t ~= nil then
    dump(t)
end

-- create http message
local msg = {
    msgtype = 'REQUEST',
    method = 'GET',
    version = 'HTTP/1.1',
    url = '/index.html',
    status = '200 OK',
    body = 'hello,world',
    contents = {
        Accept = 'image/gif. */*',
        Connection = 'Keep-Alive',
        Host = 'www.baidu.com'
    }
}

data = hp.create(msg)
print(data)

