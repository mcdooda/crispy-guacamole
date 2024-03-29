local ScriptNode = flat.require 'graph/script/scriptnode'
local PinTypes = flat.require 'graph/pintypes'

local SetRotationNode = ScriptNode:inherit 'Set Sprite Rotation X'

function SetRotationNode:buildPins()
    self.impulseInPin = self:addInputPin(PinTypes.IMPULSE, 'In')
    self.entityInPin = self:addInputPin(flat.types['CG.Entity'], 'Entity')
    self.rotationInPin = self:addInputPin(flat.types.NUMBER, 'Rotation X')

    self.impulseOutPin = self:addOutputPin(PinTypes.IMPULSE, 'Out')
end

function SetRotationNode:execute(runtime, inputPin)
    assert(inputPin == self.impulseInPin)
    local entity = runtime:readPin(self.entityInPin)
    local rotation = runtime:readPin(self.rotationInPin)
    
    entity:setSpriteRotationX(rotation)

    runtime:impulse(self.impulseOutPin)
end

return SetRotationNode