local yield = coroutine.yield

local states = {}

function states:init(dust)

    dust:playAnimation 'dust'
    dust:despawn()
    yield() -- avoid entering init()
end

return states
