--
-- work.lua
--
--print(nspr.core.version)
-- print(nspr.debug.debug_arg0())
event = nspr.event
ev = event.new()
print(ev)
event.init(ev, 1, 0)
event.add(ev)

--
-- we'll register our handlers
--
function onread(event)
    print(type(event))
    nspr.event.del(event)
end

function onwrite(event)
end

function onerror(event)
end

function ontimer(event)
end

function onsignal(event)
end
