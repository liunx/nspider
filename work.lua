--
-- work.lua
--
events = {}

function init()
    print('init start')
    local ev
    local fd = nspr.inet.listen('0.0.0.0', 8080)
    if fd > 0 then
        ev = nspr.event.new()
        events[fd] = {node = ev, fd = fd, fd_type = 'listen', dir = 'read'}
        nspr.event.init(ev, fd, nspr.event.NSPR_EVENT_TYPE_READ)
        nspr.event.add(ev, nspr.event.NSPR_EVENT_TYPE_READ)
    end

    fd = nspr.inet.connect('10.129.228.66', 8000)
    if fd > 0 then
        ev = nspr.event.new()
        events[fd] = {node = ev, fd = fd, fd_type = 'connect', dir = 'write'}
        nspr.event.init(ev, fd, nspr.event.NSPR_EVENT_TYPE_WRITE)
        nspr.event.add(ev, nspr.event.NSPR_EVENT_TYPE_WRITE)
    end
    print('init done')
end

--
-- we'll register our handlers
--
function event_read(event)
    print('event_read')
    -- get event.fd
    local fd = nspr.event.getfd(event)
    local tb = events[fd]
    if tb['fd_type'] == 'listen' then
        print(tb['fd_type'])
        local newfd = nspr.inet.accept(fd)
        -- add new event node
        local ev = nspr.event.new()
        nspr.event.init(ev, newfd, 0)
        nspr.event.add(ev)
        events[newfd] = {node = ev, fd = newfd, fd_type = 'accept', dir = 'read'}
    elseif tb['fd_type'] == 'accept' then
        print(tb['fd_type'])
        local data = nspr.file.read(tb['fd'])
        print(data)
        if string.len(data) == 0 then
            nspr.file.close(tb['fd'])
            nspr.event.del(event)
            -- release event obj
            events[fd] = nil
        end
    elseif tb['fd_type'] == 'connect' then
        -- checking connect status before write
        print(tb['fd_type'])
        local data = nspr.file.read(fd)
        print(data, string.len(data))
        if string.len(data) == 0 then
            print('connect closed')
            nspr.file.close(fd)
            nspr.event.del(event)
            -- release event obj
            events[fd] = nil
        end
    end
end

function event_write(event)
    print('event_write')
    local fd = nspr.event.getfd(event)
    local tb = events[fd]
    if tb['fd_type'] == 'connect' then
        -- checking connect status before write
        print(tb['fd_type'])
        local data = 'hello from nspider'
        nspr.file.write(tb['fd'], data, string.len(data))
        -- move fd to read set
        nspr.event.del(event)
        nspr.event.add(tb['node'], nspr.event.NSPR_EVENT_TYPE_READ)
    end
end

function event_error(event)
    -- do clean works
    print('event_error')
    local fd = nspr.event.getfd(event)
    local tb = events[fd]
    if tb['fd_type'] == 'connect' then
        print('connect')
        nspr.file.close(fd)
        events[fd] = nil
    end
end

function event_timer(event)
end

function event_signal(event)
end

-- 
-- main
--
init()

