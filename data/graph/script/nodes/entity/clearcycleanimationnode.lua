local ScriptNode = flat.require 'graph/script/scriptnode'
local PinTypes = flat.require 'graph/pintypes'

local ClearCycleAnimationNode = ScriptNode:inherit 'Clear Cycle Animation'

function ClearCycleAnimationNode:buildPins()
    self.impulseInPin = self:addInputPin(PinTypes.IMPULSE, 'In')
    self.entityInPin = self:addInputPin(flat.types['CG.Entity'], 'Entity')

    self.impulseOutPin = self:addOutputPin(PinTypes.IMPULSE, 'Out')
    self.entityOutPin = self:addOutputPin(flat.types['CG.Entity'], 'Entity')
end

function ClearCycleAnimationNode:execute(runtime, inputPin)
    assert(inputPin == self.impulseInPin)
    local entity = runtime:readPin(self.entityInPin)

    entity:clearCycleAnimation()

    runtime:writePin(self.entityOutPin, entity)
    runtime:impulse(self.impulseOutPin)
end

return ClearCycleAnimationNode