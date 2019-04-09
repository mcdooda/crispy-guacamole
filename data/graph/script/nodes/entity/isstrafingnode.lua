local FunctionalScriptNode = flat.require 'graph/script/functionalscriptnode'
local PinTypes = flat.require 'graph/pintypes'

local IsStrafingNode = FunctionalScriptNode:inherit 'Is Strafing'

function IsStrafingNode:buildPins()
    self.entityInPin = self:addInputPin(flat.types['CG.Entity'], 'Entity')

    self.strafingOutPin = self:addOutputPin(flat.types.BOOLEAN, 'Strafing')
end

function IsStrafingNode:execute(runtime)
    local entity = runtime:readPin(self.entityInPin)

    local strafing = entity:isStrafing()

    runtime:writePin(self.strafingOutPin, strafing)
end

return IsStrafingNode