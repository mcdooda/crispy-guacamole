local FunctionalScriptNode = flat.require 'graph/script/functionalscriptnode'
local PinTypes = flat.require 'graph/pintypes'

local GetExtraDataFieldNode = FunctionalScriptNode:inherit 'Get Extra Data Field'

function GetExtraDataFieldNode:buildPins()
    self.entityInPin = self:addInputPin(flat.types['CG.Entity'], 'Entity')
    self.keyInPin = self:addInputPin(flat.types.STRING, 'Key')
    
    self.valueOutPin = self:addOutputPin(PinTypes.ANY, 'Value')
end

function GetExtraDataFieldNode:execute(runtime)
    local entity = runtime:readPin(self.entityInPin)
    local key = runtime:readPin(self.keyInPin)

    local value = assert(entity:getExtraData(key), tostring(entity) .. ' has no \'' .. tostring(key) .. '\' extra data field')

    runtime:writePin(self.valueOutPin, value)
end

return GetExtraDataFieldNode