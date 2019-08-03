local states = {}

local function isValidAttackTarget(warrior, target)
	return target:hasComponent(Component.life) and (warrior:isHostile(target) or target:isHostile(warrior))
end

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
            return 'init'
        end
        local targetPosition = followTarget:getPosition()
        warrior:moveTo(targetPosition:toVector2())
    end
end

function states:onEntityEnteredVisionRange(warrior, target)
    local currentTarget = warrior:getAttackTarget()
    if currentTarget and currentTarget:isValid() and isValidAttackTarget(warrior, target) then
        return
    end
    warrior:setAttackTarget(target)
    return 'followTarget'
end

return states