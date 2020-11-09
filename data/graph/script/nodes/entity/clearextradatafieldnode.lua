local ScriptNode = flat.require 'graph/script/scriptnode'
local PinTypes = flat.require 'graph/pintypes'

local ClearExtraDataFieldNode = ScriptNode:inherit 'Clear Extra Data Field'

function ClearExtraDataFieldNode:buildPins()
    self.impulseInPin = self:addInputPin(PinTypes.IMPULSE, 'In')
    self.entityInPin = self:addInputPin(flat.types['CG.Entity'], 'Entity')
    self.keyInPin = self:addInputPin(flat.types.STRING, 'Key')

    self.impulseOutPin = self:addOutputPin(PinTypes.IMPULSE, 'Out')
end

function ClearExtraDataFieldNode:execute(runtime, inputPin)
    assert(inputPin == self.impulseInPin)
    local entity = runtime:readPin(self.entityInPin)
    local key = runtime:readPin(self.keyInPin)

    entity:getExtraData()[key] = nil

    runtime:impulse(self.impulseOutPin)
end

return ClearExtraDataFieldNode