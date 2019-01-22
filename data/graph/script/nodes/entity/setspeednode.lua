local ScriptNode = flat.require 'graph/script/scriptnode'
local PinTypes = flat.require 'graph/pintypes'

local SetSpeedNode = ScriptNode:inherit 'Set Speed'

function SetSpeedNode:buildPins()
    self.impulseInPin = self:addInputPin(PinTypes.IMPULSE, 'In')
    self.entityInPin = self:addInputPin(flat.types['CG.Entity'], 'Entity')
    self.speedInPin = self:addInputPin(flat.types.NUMBER, 'Speed')

    self.impulseOutPin = self:addOutputPin(PinTypes.IMPULSE, 'Out')
end

function SetSpeedNode:execute(runtime)
    local entity = runtime:readPin(self.entityInPin)
    local speed = runtime:readPin(self.speedInPin)

    entity:setSpeed(speed)

    runtime:impulse(self.impulseOutPin)
end

return SetSpeedNode