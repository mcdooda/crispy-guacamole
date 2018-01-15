local BehaviorHelper = require 'data/scripts/componenthelpers/behavior'

function doNothing()
	while true do
		coroutine.yield()
	end
end

local customAttacker = BehaviorHelper.customAttacker(
	doNothing,
	BehaviorHelper.findClosestTarget,
	BehaviorHelper.isValidHostileAttackTarget
)
local init = customAttacker.init
function customAttacker:init(warrior)
	local moveAnimation = 'move'
	warrior:setMoveAnimation(moveAnimation)
	init(self, warrior)
end
return customAttacker