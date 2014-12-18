package.path = package.path .. ";../../lua/filters/?.lua"

local url = require('url')

local data = '/index.htm?a=100&b=hello&c=%100&key=1234567890'

local t = url.parse(data)

for k, v in pairs(t) do
    print(k, v)
end

local url_t = {
    a = 100,
    b = 1000,
    c = 'hello',
    d = '%100',
    key = '123456789',
}

data = url.create(url_t)
print(data)
