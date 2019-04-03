local gdf = require 'mods/gdf/scripts/gdf'
local BehaviorHelper = require 'data/scripts/componenthelpers/behavior'

local random = math.random

local states = {}

function states:init(ant)
    coroutine.yield()
    coroutine.yield()
    ant:enterState 'followTarget'
end

function states:followTarget(ant)
    local players = gdf.getPlayers()
    if #players == 0 then
        ant:enterState 'wander'
    end
    local target = players[random(1, #players)]
    ant:setAttackTarget(target)
    while target:isValid() do
        local targetPosition = target:getPosition():toVector2()
        local antPosition = ant:getPosition():toVector2()
        local moveDirection = (targetPosition - antPosition):getNormalized() * 3
        ant:moveTo(antPosition + moveDirection)
    end
    ant:enterState 'followTarget'
end

states.wander = BehaviorHelper.wander

return states