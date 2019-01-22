local ScriptNode = flat.require 'graph/script/scriptnode'
local PinTypes = flat.require 'graph/pintypes'

local SetMoveAnimationNode = ScriptNode:inherit 'Set Move Animation'

function SetMoveAnimationNode:buildPins()
    self.impulseInPin = self:addInputPin(PinTypes.IMPULSE, 'In')
    self.entityInPin = self:addInputPin(flat.types['CG.Entity'], 'Entity')
    self.animationNameInPin = self:addInputPin(flat.types.STRING, 'Animation Name')

    self.impulseOutPin = self:addOutputPin(PinTypes.IMPULSE, 'Out')
end

function SetMoveAnimationNode:execute(runtime, inputPin)
    assert(inputPin == self.impulseInPin)
    local entity = runtime:readPin(self.entityInPin)
    local animationName = runtime:readPin(self.animationNameInPin)

    entity:setMoveAnimation(animationName)

    runtime:impulse(self.impulseOutPin)
end

return SetMoveAnimationNode