local BuildingBehavior = require 'data/scripts/componenthelpers/behaviors/building'
local BarkSystem = require(Mod.getFilePath 'scripts/barksystem')

local states = BuildingBehavior.basicBuilding()
local init = states.init
function states:init(townhall)
    townhall:selected(function()
        BarkSystem:requestBark(Mod.getFilePath 'sounds/fx_wood_work')
    end)
    init(self, townhall)
end

return states