local BehaviorHelper = require 'data/scripts/componenthelpers/behavior'
local states = BehaviorHelper.basicAttacker()
local BarkSystem = require(Mod.getFilePath 'scripts/barksystem')

local init = states.init
function states:init(soldier)
    soldier:selected(function()
        BarkSystem:requestBark(Mod.getFilePath 'sounds/human_male1_acknowledge')
    end)
    init(self, soldier)
end

function states:onPlayerMoveOrder(soldier, destination, interactionEntity)
    BarkSystem:requestBark(Mod.getFilePath 'sounds/human_male1_accept_order')
end

return states