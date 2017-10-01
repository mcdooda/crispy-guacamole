local FunctionalScriptNode = flat.require 'graph/script/functionalscriptnode'
local PinTypes = flat.require 'graph/pintypes'

local IsValidEntityNode = FunctionalScriptNode:inherit 'Is Valid Entity'

function IsValidEntityNode:buildPins()
    self.entityInPin = self:addInputPin(flat.types['CG.Entity'], 'Entity')

    self.isValidOutPin = self:addOutputPin(flat.types.BOOLEAN, 'Is Valid')
end

function IsValidEntityNode:execute(runtime)
    local entity = runtime:readOptionalPin(self.entityInPin)

    local isValid = entity ~= nil and entity:isValid()

    runtime:writePin(self.isValidOutPin, isValid)
end

return IsValidEntityNode