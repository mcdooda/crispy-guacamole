local EntitiesByType = require 'mods/crispy-guacamole/scripts/entitiesbytype'

local states = {}

function states:init(hut)
	EntitiesByType:add(hut)
end

return states
