local BehaviorHelper = require 'data/scripts/componenthelpers/behavior'
local states = BehaviorHelper.basicAttacker()
local BarkSystem = require(Mod.getFilePath 'scripts/barksystem')

local init = states.init
function states:init(dog)
    dog:selected(function()
        BarkSystem:requestBark(Mod.getFilePath 'sounds/dog_bark')
    end)
    init(self, dog)
end

function states:onPlayerMoveOrder(dog, destination, interactionEntity)
    BarkSystem:requestBark(Mod.getFilePath 'sounds/dog_bark')
end

return states