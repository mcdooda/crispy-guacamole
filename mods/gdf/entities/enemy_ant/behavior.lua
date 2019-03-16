local states = {}

function states:init(ant)
    coroutine.yield()
    coroutine.yield()
    ant:enterState 'followTarget'
end

function states:followTarget(ant)
    local players = Map.getEntitiesOfType('player')
    local target = players[1]
    while true do
        local targetPosition = target:getPosition():toVector2()
        local antPosition = ant:getPosition():toVector2()
        local moveDirection = (targetPosition - antPosition):getNormalized() * 3
        ant:moveTo(antPosition + moveDirection)
    end
end

return states