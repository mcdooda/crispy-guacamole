local ScriptNode = flat.require 'graph/script/scriptnode'
local PinTypes = flat.require 'graph/pintypes'

local SetProjectileSpeedNode = ScriptNode:inherit 'Set Projectile Speed'

function SetProjectileSpeedNode:buildPins()
    self.impulseInPin = self:addInputPin(PinTypes.IMPULSE, 'In')
    self.entityInPin = self:addInputPin(flat.types['CG.Entity'], 'Entity')
    self.speedInPin = self:addInputPin(flat.types['flat.Vector3'], 'Speed')

    self.impulseOutPin = self:addOutputPin(PinTypes.IMPULSE, 'Out')
end

function SetProjectileSpeedNode:execute(runtime)
    local entity = runtime:readPin(self.entityInPin)
    local speed = runtime:readPin(self.speedInPin)

    entity:setProjectileSpeed(speed)

    runtime:impulse(self.impulseOutPin)
end

return SetProjectileSpeedNode