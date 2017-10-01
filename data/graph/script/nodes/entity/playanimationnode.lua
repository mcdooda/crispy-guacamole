local ScriptNode = flat.require 'graph/script/scriptnode'
local PinTypes = flat.require 'graph/pintypes'

local PlayAnimationNode = ScriptNode:inherit 'Play Animation'

function PlayAnimationNode:buildPins()
    self.impulseInPin = self:addInputPin(PinTypes.IMPULSE, 'In')
    self.entityInPin = self:addInputPin(flat.types['CG.Entity'], 'Entity')
    self.animationNameInPin = self:addInputPin(flat.types.STRING, 'Animation Name')
    self.loopCountInPin = self:addInputPin(flat.types.NUMBER, 'Loop Count')
    self.yieldInPin = self:addInputPin(flat.types.BOOLEAN, 'Yield')

    self.impulseOutPin = self:addOutputPin(PinTypes.IMPULSE, 'Out')
end

function PlayAnimationNode:execute(runtime, inputPin)
    assert(inputPin == self.impulseInPin)
    local entity = runtime:readPin(self.entityInPin)
    local animationName = runtime:readPin(self.animationNameInPin)
    local loopCount = runtime:readPin(self.loopCountInPin)
    local yield = runtime:readPin(self.yieldInPin)

    entity:playAnimation(animationName, loopCount, yield)

    runtime:impulse(self.impulseOutPin)
end

return PlayAnimationNode