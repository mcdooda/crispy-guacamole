local BehaviorHelper = require 'data/scripts/componenthelpers/behavior'

local states = BehaviorHelper.customAttacker(
	BehaviorHelper.doNothing,
	BehaviorHelper.findClosestTarget,
	BehaviorHelper.isValidHostileAttackTarget
)

local init = states.init
function states:init(spearman)
	local moveAnimation = math.random(1, 2) == 1 and 'move2' or 'move'
	spearman:setMoveAnimation(moveAnimation)
	init(self, spearman)
end

return states