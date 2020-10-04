local ScriptNode = flat.require 'graph/script/scriptnode'
local PinTypes = flat.require 'graph/pintypes'

local SetPosition2dNode = ScriptNode:inherit 'Set Position 2D'

function SetPosition2dNode:buildPins()
    self.impulseInPin = self:addInputPin(PinTypes.IMPULSE, 'In')
    self.entityInPin = self:addInputPin(flat.types['CG.Entity'], 'Entity')
    self.positionInPin = self:addInputPin(flat.types['flat.Vector2'], 'Position')

    self.impulseOutPin = self:addOutputPin(PinTypes.IMPULSE, 'Out')
end

function SetPosition2dNode:execute(runtime)
    local entity = runtime:readPin(self.entityInPin)
    local position = runtime:readPin(self.positionInPin)

    entity:setPosition2d(position)

    runtime:impulse(self.impulseOutPin)
end

return SetPosition2dNode