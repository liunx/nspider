local function yield ()
    coroutine.yield()
end

co = coroutine.create(
function (n)
    os.execute('sleep 1')
    print('1...')
    yield()
    os.execute('sleep 1')
    print('2...')
    coroutine.yield()
    os.execute('sleep 1')
    print('3...')
    coroutine.yield()
end
)

print(coroutine.status(co))
coroutine.resume(co)
print(coroutine.status(co))
coroutine.resume(co)
coroutine.resume(co)
print(coroutine.status(co))
coroutine.resume(co)
print(coroutine.status(co))
