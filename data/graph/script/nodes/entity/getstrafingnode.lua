local FunctionalScriptNode = flat.require 'graph/script/functionalscriptnode'
local PinTypes = flat.require 'graph/pintypes'

local GetStrafingNode = FunctionalScriptNode:inherit 'Get Strafing'

function GetStrafingNode:buildPins()
    self.entityInPin = self:addInputPin(flat.types['CG.Entity'], 'Entity')

    self.strafingOutPin = self:addOutputPin(flat.types.BOOLEAN, 'Strafing')
end

function GetStrafingNode:execute(runtime)
    local entity = runtime:readPin(self.entityInPin)

    local strafing = entity:getIsStrafing()

    runtime:writePin(self.strafingOutPin, strafing)
end

return GetStrafingNode