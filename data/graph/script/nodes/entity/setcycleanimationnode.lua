local ScriptNode = flat.require 'graph/script/scriptnode'
local PinTypes = flat.require 'graph/pintypes'

local SetCycleAnimationNode = ScriptNode:inherit 'Set Cycle Animation'

function SetCycleAnimationNode:buildPins()
    self.impulseInPin = self:addInputPin(PinTypes.IMPULSE, 'In')
    self.entityInPin = self:addInputPin(flat.types['CG.Entity'], 'Entity')
    self.animationNameInPin = self:addInputPin(flat.types.STRING, 'Animation Name')

    self.impulseOutPin = self:addOutputPin(PinTypes.IMPULSE, 'Out')
end

function SetCycleAnimationNode:execute(runtime, inputPin)
    assert(inputPin == self.impulseInPin)
    local entity = runtime:readPin(self.entityInPin)
    local animationName = runtime:readPin(self.animationNameInPin)

    entity:setCycleAnimation(animationName)

    runtime:impulse(self.impulseOutPin)
end

return SetCycleAnimationNode