local yield = coroutine.yield

local states = {}

function states:init(fx)
    fx:playAnimation 'charge'
    fx:despawn()
    yield() -- avoid entering init()
end

return states
