local ScriptNode = flat.require 'graph/script/scriptnode'
local PinTypes = flat.require 'graph/pintypes'

local ResetCycleAnimationNode = ScriptNode:inherit 'Reset Cycle Animation'

function ResetCycleAnimationNode:buildPins()
    self.impulseInPin = self:addInputPin(PinTypes.IMPULSE, 'In')
    self.entityInPin = self:addInputPin(flat.types['CG.Entity'], 'Entity')

    self.impulseOutPin = self:addOutputPin(PinTypes.IMPULSE, 'Out')
end

function ResetCycleAnimationNode:execute(runtime, inputPin)
    assert(inputPin == self.impulseInPin)
    local entity = runtime:readPin(self.entityInPin)

    entity:resetCycleAnimation()

    runtime:impulse(self.impulseOutPin)
end

return ResetCycleAnimationNode