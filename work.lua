--
-- work.lua
--
print(nspr.core.version)
-- print(nspr.debug.debug_arg0())
event = nspr.event
ev = event.new()
print(ev)
event.init(ev, 1, 0)
event.add(ev)
