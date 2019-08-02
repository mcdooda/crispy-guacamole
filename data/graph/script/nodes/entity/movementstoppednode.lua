local ScriptNode = flat.require 'graph/script/scriptnode'
local PinTypes = flat.require 'graph/pintypes'

local MovementStoppedNode = ScriptNode:inherit 'Movement Stopped'

function MovementStoppedNode:buildPins()
    self.impulseInPin = self:addInputPin(PinTypes.IMPULSE, 'In')
    self.entityInPin = self:addInputPin(flat.types['CG.Entity'], 'Entity')

    self.impulseOutPin = self:addOutputPin(PinTypes.IMPULSE, 'Out')
    self.movementStoppedOutPin = self:addOutputPin(PinTypes.IMPULSE, 'Movement Stopped')
    self.entityOutPin = self:addOutputPin(flat.types['CG.Entity'], 'Entity')
end

function MovementStoppedNode:execute(runtime, inputPin)
    assert(inputPin == self.impulseInPin)
    local entity = runtime:readPin(self.entityInPin)

    entity:movementStopped(function()
        runtime:writePin(self.entityOutPin, entity)
        runtime:impulse(self.movementStoppedOutPin)
        return true
    end)

    runtime:impulse(self.impulseOutPin)
end

return MovementStoppedNode