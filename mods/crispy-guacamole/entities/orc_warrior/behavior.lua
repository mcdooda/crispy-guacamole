local BehaviorHelper = require 'data/scripts/componenthelpers/behavior'

local movement = require(Mod.getPath() .. '/entities/orc_warrior/movement')

local states = BehaviorHelper.customAttacker(
	BehaviorHelper.doNothing,
	BehaviorHelper.findClosestTarget,
	BehaviorHelper.isValidHostileAttackTarget
)

local init = states.init
function states:init(warrior)
	warrior:setMoveAnimation 'move'
	init(self, warrior)
end

local followAttackTarget = states.followAttackTarget
function states:followAttackTarget(warrior)
	warrior:setMoveAnimation 'run'
	warrior:setSpeed(movement.speed * 2.5)
	warrior:playAnimation('rage', 1, false)
	return followAttackTarget(self, warrior)
end

return states