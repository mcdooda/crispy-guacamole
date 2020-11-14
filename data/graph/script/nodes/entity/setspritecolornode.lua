local ScriptNode = flat.require 'graph/script/scriptnode'
local PinTypes = flat.require 'graph/pintypes'

local SetSpriteColorNode = ScriptNode:inherit 'Set Sprite Color'

function SetSpriteColorNode:buildPins()
    self.impulseInPin = self:addInputPin(PinTypes.IMPULSE, 'In')
    self.entityInPin = self:addInputPin(flat.types['CG.Entity'], 'Entity')
    self.colorInPin = self:addInputPin(flat.types.NUMBER, 'Color')

    self.impulseOutPin = self:addOutputPin(PinTypes.IMPULSE, 'Out')
end

function SetSpriteColorNode:execute(runtime, inputPin)
    assert(inputPin == self.impulseInPin)
    local entity = runtime:readPin(self.entityInPin)
    local color = runtime:readPin(self.colorInPin)
    
    entity:setSpriteColor(color)

    runtime:impulse(self.impulseOutPin)
end

return SetSpriteColorNode