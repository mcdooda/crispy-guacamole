local BehaviorHelper = require 'data/scripts/componenthelpers/behavior'
local EntitiesByType = require 'mods/crispy-guacamole/scripts/entitiesbytype'

local states = BehaviorHelper.basicBuilding()

local init = states.init
function states:init(hut)
	EntitiesByType:add(hut)
	init(self, hut)
end

return states
