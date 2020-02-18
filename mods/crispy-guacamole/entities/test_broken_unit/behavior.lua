local states = {}

function states:init(unit)
    coroutine.yield()
    return 'broken'
end

function states:broken()
    coroutine.yield()
    return 'test'
end

return states