local BehaviorHelper = require 'data/scripts/componenthelpers/behavior'
local states = BehaviorHelper.basicAttacker()
local BarkSystem = require(Mod.getFilePath 'scripts/barksystem')

local init = states.init
function states:init(archer)
    archer:selected(function()
        BarkSystem:requestBark(Mod.getFilePath 'sounds/human_male1_acknowledge')
    end)
    init(self, archer)
end

function states:onPlayerMoveOrder(archer, destination, interactionEntity)
    BarkSystem:requestBark(Mod.getFilePath 'sounds/human_male1_accept_order')
end

return states