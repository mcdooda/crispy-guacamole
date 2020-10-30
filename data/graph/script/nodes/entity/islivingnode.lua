local FunctionalScriptNode = flat.require 'graph/script/functionalscriptnode'
local PinTypes = flat.require 'graph/pintypes'

local IsLivingNode = FunctionalScriptNode:inherit 'Is Living'

function IsLivingNode:buildPins()
    self.entityInPin = self:addInputPin(flat.types['CG.Entity'], 'Entity')

    self.isLivingOutPin = self:addOutputPin(flat.types.BOOLEAN, 'Is Living')
end

function IsLivingNode:execute(runtime)
    local entity = runtime:readPin(self.entityInPin)

    local isLiving = entity:isLiving()

    runtime:writePin(self.isLivingOutPin, isLiving)
end

return IsLivingNode