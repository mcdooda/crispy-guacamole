local BehaviorHelper = require 'data/scripts/componenthelpers/behavior'

local random = math.random

local states = {}

function states:init(larva)
    larva:enterState 'wander'
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
