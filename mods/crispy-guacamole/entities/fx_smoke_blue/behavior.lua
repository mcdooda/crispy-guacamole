local yield = coroutine.yield

local states = {}

function states:init(smoke)
    smoke:playAnimation 'explode'
    smoke:despawn()
    yield() -- avoid entering init()
end

return states
