local ScriptNode = flat.require 'graph/script/scriptnode'
local PinTypes = flat.require 'graph/pintypes'

local MoveToNode = ScriptNode:inherit 'Look At Entity'

function MoveToNode:buildPins()
    self.impulseInPin = self:addInputPin(PinTypes.IMPULSE, 'In')
    self.entityInPin = self:addInputPin(flat.types['CG.Entity'], 'Entity')
    self.positionInPin = self:addInputPin(flat.types['flat.Vector2'], 'Position')

    self.impulseOutPin = self:addOutputPin(PinTypes.IMPULSE, 'Out')
end

function MoveToNode:execute(runtime, inputPin)
    assert(inputPin == self.impulseInPin)
    local entity = runtime:readPin(self.entityInPin)
    local position = runtime:readPin(self.positionInPin)

    entity:moveTo(position)

    runtime:impulse(self.impulseOutPin)
end

return MoveToNode