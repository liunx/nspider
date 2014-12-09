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
files[1] = ev

--
-- we'll register our handlers
--
function event_read(event)
    -- get event.fd
    local fd = nspr.event.getfd(event)
    print(fd, files[fd])
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
