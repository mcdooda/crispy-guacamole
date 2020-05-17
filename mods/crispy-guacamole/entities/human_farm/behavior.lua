local BuildingBehavior = require 'data/scripts/componenthelpers/behaviors/building'
local BarkSystem = require(Mod.getFilePath 'scripts/barksystem')

local states = BuildingBehavior.basicBuilding()
local init = states.init
function states:init(windmill)
    windmill:selected(function()
        BarkSystem:requestBark(Mod.getFilePath 'sounds/fx_mill')
    end)
    init(self, windmill)
end

return states
