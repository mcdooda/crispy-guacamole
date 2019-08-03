local BehaviorHelper = require 'data/scripts/componenthelpers/behavior'

local states = BehaviorHelper.basicAttacker()

local init = states.init
function states:init(warrior)
    warrior:setSpeed(2.5)
    return init(self, warrior)
end

local followAttackTarget = states.followAttackTarget
function states:followAttackTarget(warrior)
    local currentAttackTarget = warrior:getAttackTarget()
    if currentAttackTarget and currentAttackTarget:isValid() then
        warrior:setSpeed(4)
        warrior:playAnimation('rage', 1, false)
    end
    return followAttackTarget(self, warrior)
end

return states