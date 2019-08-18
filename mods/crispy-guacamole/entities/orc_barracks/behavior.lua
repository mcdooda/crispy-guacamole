local BuildingSelection = require 'mods/crispy-guacamole/scripts/buildingselection'

local states = {}

function states:init(barracks)
    barracks:setCycleAnimation 'stand'
    BuildingSelection.init(barracks)
end

return states
