local BehaviorHelper = require 'data/scripts/componenthelpers/behavior'
local basicAttacker = BehaviorHelper.basicAttacker()
local init = basicAttacker.init
function basicAttacker:init(spearman)
	local moveAnimation = math.random(1, 2) == 1 and 'move2' or 'move'
	spearman:setMoveAnimation(moveAnimation)
	init(self, spearman)
end
return basicAttacker