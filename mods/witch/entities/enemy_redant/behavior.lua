local BehaviorHelper = require 'data/scripts/componenthelpers/behavior'

local states = {}

function states:init(ant)
    coroutine.yield()
    coroutine.yield()
    return 'followTarget'
end

function states:followTarget(ant)
    local players = Map.getEntitiesOfType('player')
    local target = players[1]
    if not target then
        return 'wander'
    end
    ant:setAttackTarget(target)
    while target:isValid() do
        local targetPosition = target:getPosition():toVector2()
        local antPosition = ant:getPosition():toVector2()
        local moveDirection = (targetPosition - antPosition):getNormalized() * 3
        ant:moveTo(antPosition + moveDirection)
    end
    return 'wander'
end

states.wander = BehaviorHelper.wander

return states