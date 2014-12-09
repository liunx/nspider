--
-- work.lua
--
--print(nspr.core.version)
-- print(nspr.debug.debug_arg0())
event = nspr.event
ev = event.new()
--print(ev)
event.init(ev, 1, 0)
event.add(ev)

--[[
a = {}
a[ev] = {a = 1, b = 2, c = 3}
print(a)
print(a[ev]['a'])
--]]
files = {}
files[ev] = {fd = 1, type = 'file', name = 'stdin'}

--
-- we'll register our handlers
--
function event_read(event)
    nspr.event.del(event)
end

function event_write(event)
end

function event_error(event)
end

function event_timer(event)
end

function event_signal(event)
end
