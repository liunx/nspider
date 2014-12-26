-- foreman.lua
-- just look for the name above, you known it!
--
local pipeline = require('pipeline')

-- coroutines
local function exit_SIGINT (id)
    pipeline.signal(id, 2) -- SIGINT
    print('SIGINT, exiting...')
    pipeline.exit()
end

local function exit_SIGQUIT (id)
    pipeline.signal(id, 3) -- SIGQUIT
    print('SIGQUIT, exiting...')
    pipeline.exit()
end

local function scandir (dir)
    local i, t, popen = 0, {}, io.popen
    for filename in popen('ls ' .. dir .. ' | grep \'.lua\''):lines() do
        i = i + 1
        t[i] = filename
    end
    return t
end

local function load_tasks ()
    local t = scandir(nspider_lua_path .. 'tasks/')
    local task
    for k, v in pairs(t) do
        task = string.gsub(v, '.lua', '')
        if task ~= 'foreman' then
            require(task)
        end
    end
end

local function init ()
    pipeline.coroutine(exit_SIGINT)
    pipeline.coroutine(exit_SIGQUIT)
    load_tasks()
end

init()

