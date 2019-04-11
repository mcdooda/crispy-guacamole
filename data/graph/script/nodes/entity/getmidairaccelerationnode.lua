local FunctionalScriptNode = flat.require 'graph/script/functionalscriptnode'
local PinTypes = flat.require 'graph/pintypes'

local GetMidairAccelerationNode = FunctionalScriptNode:inherit 'Get Midair Acceleration'

function GetMidairAccelerationNode:buildPins()
    self.entityInPin = self:addInputPin(flat.types['CG.Entity'], 'Entity')
    
    self.accelerationOutPin = self:addOutputPin(flat.types['flat.Vector3'], 'Acceleration')
end

function GetMidairAccelerationNode:execute(runtime)
    local entity = runtime:readPin(self.entityInPin)

    local acceleration = entity:getMidairAcceleration()

    runtime:writePin(self.accelerationOutPin, acceleration)
end

return GetMidairAccelerationNode