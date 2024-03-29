local ScriptNode = flat.require 'graph/script/scriptnode'
local PinTypes = flat.require 'graph/pintypes'

local GetExtraDataFieldNode = ScriptNode:inherit 'Get Extra Data Field'

function GetExtraDataFieldNode:buildPins()
    self.impulseInPin = self:addInputPin(PinTypes.IMPULSE, 'In')
    self.entityInPin = self:addInputPin(flat.types['CG.Entity'], 'Entity')
    self.keyInPin = self:addInputPin(flat.types.STRING, 'Key')
    
    self.impulseOutPin = self:addOutputPin(PinTypes.IMPULSE, 'Out')
    self.keyExistsOutPin = self:addOutputPin(flat.types.BOOLEAN, 'Key Exists')
    self.valueOutPin = self:addOutputPin(PinTypes.ANY, 'Value')
end

function GetExtraDataFieldNode:execute(runtime)
    local entity = runtime:readPin(self.entityInPin)
    local key = runtime:readPin(self.keyInPin)

    local value = entity:getExtraData()[key]
    local keyExists = value ~= nil

    runtime:writeOptionalPin(self.valueOutPin, value)
    runtime:writePin(self.keyExistsOutPin, keyExists)
    runtime:impulse(self.impulseOutPin)
end

return GetExtraDataFieldNode