local ScriptNode = flat.require 'graph/script/scriptnode'
local PinTypes = flat.require 'graph/pintypes'

local SetMidairAccelerationNode = ScriptNode:inherit 'Set Midair Acceleration'

function SetMidairAccelerationNode:buildPins()
    self.impulseInPin = self:addInputPin(PinTypes.IMPULSE, 'In')
    self.entityInPin = self:addInputPin(flat.types['CG.Entity'], 'Entity')
    self.accelerationInPin = self:addInputPin(flat.types['flat.Vector3'], 'Acceleration')

    self.impulseOutPin = self:addOutputPin(PinTypes.IMPULSE, 'Out')
end

function SetMidairAccelerationNode:execute(runtime, inputPin)
    assert(inputPin == self.impulseInPin)
    local entity = runtime:readPin(self.entityInPin)
    local acceleration = runtime:readPin(self.accelerationInPin)

    entity:setMidairAcceleration(acceleration)

    runtime:impulse(self.impulseOutPin)
end

return SetMidairAccelerationNode