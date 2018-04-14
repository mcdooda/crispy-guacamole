local yield = coroutine.yield

local states = {}

function states:init(splatter)
    splatter:playAnimation 'splatter'
    splatter:despawn()
    yield() -- avoid entering init()
end

return states
