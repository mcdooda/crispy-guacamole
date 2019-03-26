local yield = coroutine.yield

local states = {}

function states:init(fx)
    fx:playAnimation 'sparkle'
    fx:despawn()
    yield() -- avoid entering init()
end

return states
