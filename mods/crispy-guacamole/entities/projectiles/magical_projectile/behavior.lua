local yield = coroutine.yield

local states = {}

function states:init(fx)
    fx:playAnimation 'start'
    yield() -- avoid entering init()
end

return states
