-- cfg.lua

-- add custom package cpath
package.cpath = package.cpath
-- add custom package path
package.path = package.path .. "./lua/?.lua;./lua/filters/?.lua;./lua/schedulers/?.lua"

local pipeline = require("pipeline")

function init ()
    print('begin init...')
    dofile('./lua/tasks/iwifi.lua')
    print('complete init...')
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
