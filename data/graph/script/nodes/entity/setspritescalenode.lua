local ScriptNode = flat.require 'graph/script/scriptnode'
local PinTypes = flat.require 'graph/pintypes'

local SetSpriteScaleNode = ScriptNode:inherit 'Set Sprite Scale'

function SetSpriteScaleNode:buildPins()
    self.impulseInPin = self:addInputPin(PinTypes.IMPULSE, 'In')
    self.entityInPin = self:addInputPin(flat.types['CG.Entity'], 'Entity')
    self.scaleInPin = self:addInputPin(flat.types.NUMBER, 'Scale')

    self.impulseOutPin = self:addOutputPin(PinTypes.IMPULSE, 'Out')
end

function SetSpriteScaleNode:execute(runtime, inputPin)
    assert(inputPin == self.impulseInPin)
    local entity = runtime:readPin(self.entityInPin)
    local scale = runtime:readPin(self.scaleInPin)
    
    entity:setSpriteScale(scale)

    runtime:impulse(self.impulseOutPin)
end

return SetSpriteScaleNode