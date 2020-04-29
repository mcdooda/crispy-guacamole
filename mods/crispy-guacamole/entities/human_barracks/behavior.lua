local BuildingBehavior = require 'data/scripts/componenthelpers/behaviors/building'
local BarkSystem = require(Mod.getFilePath 'scripts/barksystem')

local states = BuildingBehavior.basicBuilding()
local init = states.init
function states:init(barrack)
    barrack:selected(function()
        BarkSystem:requestBark(Mod.getFilePath 'sounds/fx_army_march')
    end)
    init(self, barrack)
end

return states