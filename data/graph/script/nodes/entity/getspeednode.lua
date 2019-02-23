local FunctionalScriptNode = flat.require 'graph/script/functionalscriptnode'
local PinTypes = flat.require 'graph/pintypes'

local GetSpeedNode = FunctionalScriptNode:inherit 'Get Speed'

function GetSpeedNode:buildPins()
    self.entityInPin = self:addInputPin(flat.types['CG.Entity'], 'Entity')
    
    self.speedOutPin = self:addOutputPin(flat.types.NUMBER, 'Speed')
end

function GetSpeedNode:execute(runtime)
    local entity = runtime:readPin(self.entityInPin)

    local speed = entity:getSpeed()

    runtime:writePin(self.speedOutPin, speed)
end

return GetSpeedNode