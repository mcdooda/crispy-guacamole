local FunctionalScriptNode = flat.require 'graph/script/functionalscriptnode'
local PinTypes = flat.require 'graph/pintypes'

local PlayerControllerComponentNode = FunctionalScriptNode:inherit 'Player Controller Component'

function PlayerControllerComponentNode:buildPins()
    self.componentOutPin = self:addOutputPin(flat.types.TABLE, 'Component')
    self.onUpdateOutPin = self:addOutputPin(PinTypes.IMPULSE, 'Update')
    self.entityOutPin = self:addOutputPin(flat.types['CG.Entity'], 'Entity')
end

function PlayerControllerComponentNode:execute(runtime)
    local component = {
        update = function(entity)
            runtime:writePin(self.entityOutPin, entity)
            runtime:impulse(self.onUpdateOutPin)
        end
    }

    runtime:writePin(self.componentOutPin, component)
end

return PlayerControllerComponentNode