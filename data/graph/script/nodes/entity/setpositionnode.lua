local ScriptNode = flat.require 'graph/script/scriptnode'
local PinTypes = flat.require 'graph/pintypes'

local SetPositionNode = ScriptNode:inherit 'Set Position'

function SetPositionNode:buildPins()
    self.impulseInPin = self:addInputPin(PinTypes.IMPULSE, 'In')
    self.entityInPin = self:addInputPin(flat.types['CG.Entity'], 'Entity')
    self.positionInPin = self:addInputPin(flat.types['flat.Vector3'], 'Position')

    self.impulseOutPin = self:addOutputPin(PinTypes.IMPULSE, 'Out')
end

function SetPositionNode:execute(runtime)
    local entity = runtime:readPin(self.entityInPin)
    local position = runtime:readPin(self.positionInPin)

    entity:setPosition(position)

    runtime:impulse(self.impulseOutPin)
end

return SetPositionNode