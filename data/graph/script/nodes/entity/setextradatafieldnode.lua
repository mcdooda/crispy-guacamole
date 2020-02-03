local ScriptNode = flat.require 'graph/script/scriptnode'
local PinTypes = flat.require 'graph/pintypes'

local SetExtraDataFieldNode = ScriptNode:inherit 'Set Extra Data Field'

function SetExtraDataFieldNode:buildPins()
    self.impulseInPin = self:addInputPin(PinTypes.IMPULSE, 'In')
    self.entityInPin = self:addInputPin(flat.types['CG.Entity'], 'Entity')
    self.keyInPin = self:addInputPin(flat.types.STRING, 'Key')
    self.valueInPin = self:addInputPin(PinTypes.ANY, 'Value')

    self.impulseOutPin = self:addOutputPin(PinTypes.IMPULSE, 'Out')
end

function SetExtraDataFieldNode:execute(runtime, inputPin)
    assert(inputPin == self.impulseInPin)
    local entity = runtime:readPin(self.entityInPin)
    local key = runtime:readPin(self.keyInPin)
    local value = runtime:readPin(self.valueInPin)

    entity:getExtraData()[key] = value

    runtime:impulse(self.impulseOutPin)
end

return SetExtraDataFieldNode