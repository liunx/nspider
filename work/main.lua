-- cfg.lua
local function script_path ()
    local str = debug.getinfo(2, "S").source:sub(2)
    return str:match("(.*/)")
end

local prefix = script_path()
nspider_lua_path = prefix
local mypath = prefix .. '?.lua;'
mypath = mypath .. prefix .. 'filters/?.lua;'
mypath = mypath .. prefix .. 'schedulers/?.lua;'
mypath = mypath .. prefix .. 'tasks/?.lua;'
mypath = mypath .. prefix .. 'configs/?.lua;'
mypath = mypath .. prefix .. 'modules/?.lua;'

package.path = package.path .. mypath
-- TODO add package.cpath

local pipeline = require("pipeline")
function init ()
    require('foreman')
end


function event_read (event)
    pipeline.oread(event)
end

function event_write (event)
    pipeline.owrite(event)
end

function event_error (event)
    pipeline.oerror(event)
end

function event_timer (event)
    pipeline.otimer(event)
end

function event_signal (event)
    pipeline.osignal(event)
end

-- main
init()
