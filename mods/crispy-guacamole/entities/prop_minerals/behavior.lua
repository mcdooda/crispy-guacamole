local EntitiesByType = require 'mods/crispy-guacamole/scripts/entitiesbytype'

local states = {}

function states:init(minerals)
	EntitiesByType:add(minerals)
	minerals:getExtraData().amount = 5
end

return states
