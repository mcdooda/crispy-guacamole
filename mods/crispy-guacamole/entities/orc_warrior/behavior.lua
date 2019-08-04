local BehaviorHelper = require 'data/scripts/componenthelpers/behavior'

local states = BehaviorHelper.basicAttacker()

local followAttackTarget = states.followAttackTarget
function states:followAttackTarget(warrior)
    local currentAttackTarget = warrior:getAttackTarget()
    if currentAttackTarget and currentAttackTarget:isValid() then
        warrior:setSpeed(4)
        warrior:playAnimation('rage', 1, false)
    end
    local newState = followAttackTarget(self, warrior)
    warrior:setSpeed(2.5)
    return newState
end

return states