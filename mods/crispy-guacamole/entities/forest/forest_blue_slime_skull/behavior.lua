local BehaviorHelper = require 'data/scripts/componenthelpers/behavior'

local states = BehaviorHelper.basicAttacker()
local init = states.init
function states:init(slime)
	slime:click(function(slime)
		slime:kill()
    end)
    init(self, slime)
end

return states