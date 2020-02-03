local ScriptNode = flat.require 'graph/script/scriptnode'
local PinTypes = flat.require 'graph/pintypes'

local SetInstigatorNode = ScriptNode:inherit 'Set Instigator'

function SetInstigatorNode:buildPins()
    self.impulseInPin = self:addInputPin(PinTypes.IMPULSE, 'In')
    self.entityInPin = self:addInputPin(flat.types['CG.Entity'], 'Entity')
    self.instigatorInPin = self:addInputPin(flat.types['CG.Entity'], 'Instigator')

    self.impulseOutPin = self:addOutputPin(PinTypes.IMPULSE, 'Out')
end

function SetInstigatorNode:execute(runtime, inputPin)
    assert(inputPin == self.impulseInPin)
    local entity = runtime:readPin(self.entityInPin)
    local instigator = runtime:readPin(self.instigatorInPin)

    entity:setInstigator(instigator)

    runtime:impulse(self.impulseOutPin)
end

return SetInstigatorNode