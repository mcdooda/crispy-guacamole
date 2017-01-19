local yield = coroutine.yield

local states = {}

function states:init(explosion)
    explosion:playAnimation 'explode'
    explosion:despawn()
    yield() -- avoid entering init()
end

return states
