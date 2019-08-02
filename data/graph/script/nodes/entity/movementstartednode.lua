local ScriptNode = flat.require 'graph/script/scriptnode'
local PinTypes = flat.require 'graph/pintypes'

local MovementStartedNode = ScriptNode:inherit 'Movement Started'

function MovementStartedNode:buildPins()
    self.impulseInPin = self:addInputPin(PinTypes.IMPULSE, 'In')
    self.entityInPin = self:addInputPin(flat.types['CG.Entity'], 'Entity')

    self.impulseOutPin = self:addOutputPin(PinTypes.IMPULSE, 'Out')
    self.movementStartedOutPin = self:addOutputPin(PinTypes.IMPULSE, 'Movement Started')
    self.entityOutPin = self:addOutputPin(flat.types['CG.Entity'], 'Entity')
end

function MovementStartedNode:execute(runtime, inputPin)
    assert(inputPin == self.impulseInPin)
    local entity = runtime:readPin(self.entityInPin)

    entity:movementStarted(function()
        runtime:writePin(self.entityOutPin, entity)
        runtime:impulse(self.movementStartedOutPin)
        return true
    end)

    runtime:impulse(self.impulseOutPin)
end

return MovementStartedNode