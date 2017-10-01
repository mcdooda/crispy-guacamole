local ScriptNode = flat.require 'graph/script/scriptnode'
local PinTypes = flat.require 'graph/pintypes'

local LookAtEntityNode = ScriptNode:inherit 'Look At Entity'

function LookAtEntityNode:buildPins()
    self.impulseInPin = self:addInputPin(PinTypes.IMPULSE, 'In')
    self.entityInPin = self:addInputPin(flat.types['CG.Entity'], 'Entity')
    self.targetInPin = self:addInputPin(flat.types['CG.Entity'], 'Target')

    self.impulseOutPin = self:addOutputPin(PinTypes.IMPULSE, 'Out')
end

function LookAtEntityNode:execute(runtime, inputPin)
    assert(inputPin == self.impulseInPin)
    local entity = runtime:readPin(self.entityInPin)
    local target = runtime:readPin(self.targetInPin)

    entity:lookAtEntity(target)

    runtime:impulse(self.impulseOutPin)
end

return LookAtEntityNode