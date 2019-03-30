local yield = coroutine.yield

local states = {}

function states:init(fx)
    fx:playAnimation 'start'
    fx:despawn()
    yield() -- avoid entering init()
end

return states
