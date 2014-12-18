--
-- pipeline.lua
--

local inet = nspr.inet
local event = nspr.event
local file = nspr.file
local timer = nspr.timer
local signal = nspr.signal

local pipeline = {}
local pipes = {}
local events = {}
local timers = {}
local signals = {}

local eventids = {}

local databuf = {}

local function eventid_new ()
    local eventid
    while true do
        eventid = math.random(1, 10000)
        if eventids[eventid] == nil then
            eventids[eventid] = true
            break
        elseif eventids[eventid] == false then
            eventids[eventid] = true
            break
        end
    end
    return eventid
end

local function eventid_free (eventid)
    eventids[eventid] = false
end

local function uuid ()
    local f = io.open('/proc/sys/kernel/random/uuid', 'r')
    local uuid = f:read('*l')
    f:close()
    return uuid
end

-- user interfaces
function pipeline.wait (wtb)
    local fd
    local ev
    local table = {}
    local id = uuid()

    if wtb['type'] == 'tcp' then
        local fd = inet.listen(wtb['addr'], wtb['port'])
        if fd > 0 then
            ev = event.new()
            events[fd] = id
            event.init(ev, fd, event.NSPR_EVENT_TYPE_READ)
            event.add(ev, event.NSPR_EVENT_TYPE_READ)
        end
    end
    table['handler'] = wtb['handler']
    table['listen'] = true
    pipes[id] = table
end

function pipeline.listen (addr, port, fun)
    local fd
    local ev
    local table = {}
    local id = eventid_new()
    print('id -- ' .. id)
    local fd = inet.listen(addr, port)
    if fd > 0 then
        ev = event.new()
        event.init(ev, fd, event.NSPR_EVENT_TYPE_READ, id)
        event.add(ev, event.NSPR_EVENT_TYPE_READ)
        table['ev'] = ev
        table['fd'] = fd
        table['fun'] = fun
        table['listen'] = true
        events[id] = table
    end
end

function pipeline.read (id)
    return databuf[id]
end

function pipeline.write (id, data)
    local table = events[id]
    local fd = table['fd']
    file.write(fd, data, string.len(data))
end

function pipeline.close (id)
    local table = events[id]
    local fd = table['fd']
    local ev = table['ev']
    event.del(ev)
    file.close(fd)
end

function pipeline.connect (id, addr, port)
    local newid = eventid_new()
    local fd = inet.connect(addr, port)
    local table = events[id]
    if fd > 0 then
        local newtable = {}
        local ev = event.new()
        event.init(ev, fd, event.NSPR_EVENT_TYPE_WRITE, newid)
        event.add(ev, event.NSPR_EVENT_TYPE_WRITE)
        newtable['ev'] = ev
        newtable['fd'] = fd
        print('fd --- ' .. fd)
        print(table['co'])
        newtable['co'] = table['co']
        print('newid ' .. newid)
        events[newid] = newtable
    end
    return newid
end

function pipeline.request (rtb)
end

function pipeline.response (id, data)
    local table = events[id]
    local fd = table['fd']
    file.write(fd, data, string.len(data))
end

function pipeline.forward (ftb)
end

function pipeline.backward (btb)
end

function pipeline.complete (id)
    eventid_free(id)
    events[id] = nil
end

function pipeline.timer ()
    local id = eventid_new()
end

function pipeline.sleep (id, msec)
    local newtable = {}
    local newid = eventid_new()
    local table = events[id]

    local timer = timer.new()
    timer.set(id, timer, 3000)
    timer.add(timer)
    coroutine.yield()
    timer.del(timer)
end

function pipeline.yield ()
    coroutine.yield()
end

-- event hooks
function pipeline.oread (ev)
    local id = event.getid(ev)
    local table = events[id]
    local fd = table['fd']
    -- check for listen
    if table['listen'] == true then
        local newfd = inet.accept(fd)
        if newfd > 0 then
            local newid = eventid_new()
            local ev = event.new()
            event.init(ev, newfd, event.NSPR_EVENT_TYPE_READ, newid)
            event.add(ev, event.NSPR_EVENT_TYPE_READ)
            local newtable = {}
            newtable['co'] = coroutine.create(table['fun'])
            newtable['fd'] = newfd
            newtable['ev'] = ev
            events[newid] = newtable
        end
        return
    end
    -- care for close event
    local data = file.read(fd)
    if string.len(data) == 0 then
        event.del(table['ev'])
        file.close(fd)
    else
        -- call callback hook
        databuf[id] = data
        coroutine.resume(table['co'], id)
    end
end

function pipeline.owrite (ev)
    local id = event.getid(ev)
    local table = events[id]
    local fd = table['fd']

    coroutine.resume(table['co'], id)
    event.del(table['ev'])
    event.add(table['ev'], event.NSPR_EVENT_TYPE_READ)
end

function pipeline.oerror (ev)
end

function pipeline.otimer (ev)
end

function pipeline.osignal (ev)
end

return pipeline
