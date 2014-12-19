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
local exit_funs = {}

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

local function popen (id, cmd)
    local ev = event.new()
    local fd = file.popen(ev, cmd)
    if fd < 0 then
        return nil
    end
    event.init(ev, fd, event.NSPR_EVENT_TYPE_READ, id)
    event.add(ev, event.NSPR_EVENT_TYPE_READ)
    coroutine.yield()
    event.del(ev)
    local data = file.read(fd)
    file.pclose(ev)

    return data
end


local function gethostbyname (id, addr)
    -- check for ip
    local ip = string.match(addr, "([0-9]+.[0-9]+.[0-9]+.[0-9]+)")
    if ip ~= nil then
        return ip
    end
    -- get ip by name
    local data = popen(id, 'ping -c 1 ' .. addr)
    ip = string.match(data, "([0-9]+.[0-9]+.[0-9]+.[0-9]+)")
    return ip
end

-- user interfaces
function pipeline.popen (id, cmd)
    return popen(id, cmd)
end

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

-- for setting up timer, signal
function pipeline.coroutine (fun)
    local id = eventid_new()
    local table = {}
    events[id] = table
    table['co'] = coroutine.create(fun)
    coroutine.resume(table['co'], id)
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
    local table = events[id]
    local fd = table['fd']
    local ev = table['ev']
    event.add(ev, event.NSPR_EVENT_TYPE_READ)
    coroutine.yield()
    event.del(ev)
    local data = file.read(fd)
    return data
end

function pipeline.write (id, data)
    local table = events[id]
    local fd = table['fd']
    local ev = table['ev']
    event.add(ev, event.NSPR_EVENT_TYPE_WRITE)
    coroutine.yield()
    event.del(ev)
    file.write(fd, data, string.len(data))
end

function pipeline.close (id)
    local table = events[id]
    local fd = table['fd']
    file.close(fd)
end

function pipeline.connect (id, addr, port)
    local newid = eventid_new()
    local ip = gethostbyname(id, addr)
    if ip == nil then
        return nil
    end

    local fd = inet.connect(ip, port)
    if fd == nil then
        return nil
    end

    local table = events[id]
    if fd > 0 then
        local newtable = {}
        local ev = event.new()
        newtable['ev'] = ev
        newtable['fd'] = fd
        newtable['co'] = table['co']
        events[newid] = newtable
        event.init(ev, fd, event.NSPR_EVENT_TYPE_WRITE, newid)
        event.add(ev, event.NSPR_EVENT_TYPE_WRITE)
        coroutine.yield()
        event.del(ev)
        return newid
    end
    return nil
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

function pipeline.sleep (id, msec)
    local t = timer.new()
    timer.set(t, id, msec)
    timer.add(t)
    coroutine.yield()
    timer.del(t)
end

function pipeline.signal (id, signum)
    local s = signal.new()
    signal.set(s, id, signum)
    signal.add(s)
    coroutine.yield()
    signal.del(s)
end

function pipeline.do_exit (f)
    table.insert(exit_funs, f)
end

function pipeline.exit ()
    for k, f in pairs(exit_funs) do
        if type(f) == 'function' then
            f()
        end
    end
    event.exit()
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
            local newtable = {}
            newtable['co'] = coroutine.create(table['fun'])
            newtable['fd'] = newfd
            newtable['ev'] = ev
            events[newid] = newtable
            -- first run the coroutine
            coroutine.resume(newtable['co'], newid)
        end
        return
    end

    coroutine.resume(table['co'], id)
end

function pipeline.owrite (ev)
    local id = event.getid(ev)
    local table = events[id]

    coroutine.resume(table['co'], id)
end

function pipeline.oerror (ev)
end

function pipeline.otimer (ev)
    local id = timer.get(ev)
    local table = events[id]

    coroutine.resume(table['co'], id)
end

function pipeline.osignal (ev)
    local id
    id, _ = signal.get(ev)
    local table = events[id]

    coroutine.resume(table['co'], id)
end

return pipeline
