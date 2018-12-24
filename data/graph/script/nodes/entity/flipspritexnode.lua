local ScriptNode = flat.require 'graph/script/scriptnode'
local PinTypes = flat.require 'graph/pintypes'

local FlipSpriteXNode = ScriptNode:inherit 'Flip Sprite X'

function FlipSpriteXNode:buildPins()
    self.impulseInPin = self:addInputPin(PinTypes.IMPULSE, 'In')
    self.entityInPin = self:addInputPin(flat.types['CG.Entity'], 'Entity')
    self.flipInPin = self:addInputPin(flat.types.BOOLEAN, 'Flip')

    self.impulseOutPin = self:addOutputPin(PinTypes.IMPULSE, 'Out')
end

function FlipSpriteXNode:execute(runtime, inputPin)
    assert(inputPin == self.impulseInPin)
    local entity = runtime:readPin(self.entityInPin)
    local flip = runtime:readPin(self.flipInPin)
    
    entity:flipSpriteX(flip)

    runtime:impulse(self.impulseOutPin)
end

return FlipSpriteXNode