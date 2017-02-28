local math = math
local random = math.random

local function sleep(duration)
    local getTime = Time.getTime
    local endTime = getTime() + duration
    local yield = coroutine.yield
    while getTime() < endTime do
        yield()
    end
end

local states = {}

function states:init(larva)
	-- give them a chance to get an external order before wandering
	coroutine.yield()
    coroutine.yield()

    larva:enterState 'wander'
    --larva:playAnimation 'graze'
end

function states:idle(larva)
    larva:jump()
end

function states:wander(larva)
    local x, y = larva:getPosition()
    while true do

        do
            local rx = x + (random() * 2 - 1) * 2
            local ry = y + (random() * 2 - 1) * 2
            larva:moveTo(rx, ry)
        end

    end
end

return states
