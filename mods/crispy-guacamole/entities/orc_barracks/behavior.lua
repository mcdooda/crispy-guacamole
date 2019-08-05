local yield = coroutine.yield

local states = {}

function states:init(barracks)
    barracks:setCycleAnimation 'stand'
    yield() -- avoid entering init()
end

return states
