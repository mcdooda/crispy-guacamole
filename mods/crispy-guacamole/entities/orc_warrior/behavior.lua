local states = {}

function states:init(warrior)
    warrior:setSpeed(2.5)
end

function states:followTarget(warrior)
    warrior:playAnimation('rage', 1, false)
    warrior:clearPath()
    warrior:setSpeed(4)
    while true do
        local followTarget = warrior:getAttackTarget()
        if not followTarget or not followTarget:isValid() then
            warrior:enterState 'init'
        end
        local targetPosition = followTarget:getPosition()
        warrior:moveTo(targetPosition:toVector2())
    end
end

function states:onEntityEnteredVisionRange(warrior, target)
    local currentTarget = warrior:getAttackTarget()
    if currentTarget and currentTarget:isValid() then
        return
    end
    warrior:setAttackTarget(target)
    return 'followTarget'
end

return states