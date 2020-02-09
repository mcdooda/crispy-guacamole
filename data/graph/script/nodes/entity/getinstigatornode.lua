local FunctionalScriptNode = flat.require 'graph/script/functionalscriptnode'
local PinTypes = flat.require 'graph/pintypes'

local GetInstigatorNode = FunctionalScriptNode:inherit 'Get Instigator'

function GetInstigatorNode:buildPins()
    self.entityInPin = self:addInputPin(flat.types['CG.Entity'], 'Entity')
    
    self.instigatorOutPin = self:addOutputPin(flat.types['CG.Entity'], 'Instigator')
end

function GetInstigatorNode:execute(runtime)
    local entity = runtime:readPin(self.entityInPin)

    local instigator = entity:getInstigator()

    runtime:writeOptionalPin(self.instigatorOutPin, instigator)
end

return GetInstigatorNode