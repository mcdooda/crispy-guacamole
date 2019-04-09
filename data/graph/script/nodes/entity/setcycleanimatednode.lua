local ScriptNode = flat.require 'graph/script/scriptnode'
local PinTypes = flat.require 'graph/pintypes'

local SetCycleAnimatedNode = ScriptNode:inherit 'Set Cycle Animated'

function SetCycleAnimatedNode:buildPins()
    self.impulseInPin = self:addInputPin(PinTypes.IMPULSE, 'In')
    self.entityInPin = self:addInputPin(flat.types['CG.Entity'], 'Entity')
    self.animatedInPin = self:addInputPin(flat.types.BOOLEAN, 'Animated')

    self.impulseOutPin = self:addOutputPin(PinTypes.IMPULSE, 'Out')
end

function SetCycleAnimatedNode:execute(runtime, inputPin)
    assert(inputPin == self.impulseInPin)
    local entity = runtime:readPin(self.entityInPin)
    local animated = runtime:readPin(self.animatedInPin)

    entity:setCycleAnimated(animated)

    runtime:impulse(self.impulseOutPin)
end

return SetCycleAnimatedNode