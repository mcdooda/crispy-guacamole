local ScriptNode = flat.require 'graph/script/scriptnode'
local PinTypes = flat.require 'graph/pintypes'

local MoveToNode = ScriptNode:inherit 'Move To'

function MoveToNode:buildPins()
    self.impulseInPin = self:addInputPin(PinTypes.IMPULSE, 'In')
    self.entityInPin = self:addInputPin(flat.types['CG.Entity'], 'Entity')
    self.positionInPin = self:addInputPin(flat.types['flat.Vector2'], 'Position')
    self.yieldInPin = self:addInputPin(flat.types.BOOLEAN, 'Yield')

    self.impulseOutPin = self:addOutputPin(PinTypes.IMPULSE, 'Out')
end

function MoveToNode:execute(runtime, inputPin)
    assert(inputPin == self.impulseInPin)
    local entity = runtime:readPin(self.entityInPin)
    local position = runtime:readPin(self.positionInPin)
    local yield = runtime:readPin(self.yieldInPin)

    entity:moveTo(position, yield)

    runtime:impulse(self.impulseOutPin)
end

return MoveToNode