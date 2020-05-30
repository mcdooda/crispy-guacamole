local BuildingBehavior = require 'data/scripts/componenthelpers/behaviors/building'
local BarkSystem = require(Mod.getFilePath 'scripts/barksystem')

local states = BuildingBehavior.basicBuilding()
local init = states.init
function states:init(stoneWall)
    stoneWall:selected(function()
        BarkSystem:requestBark(Mod.getFilePath 'sounds/fx_brick')
    end)
    init(self, stoneWall)
end

return states