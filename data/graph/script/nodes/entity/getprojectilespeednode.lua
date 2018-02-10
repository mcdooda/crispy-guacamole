local FunctionalScriptNode = flat.require 'graph/script/functionalscriptnode'
local PinTypes = flat.require 'graph/pintypes'

local GetProjectileSpeedNode = FunctionalScriptNode:inherit 'Get Projectile Speed'

function GetProjectileSpeedNode:buildPins()
    self.entityInPin = self:addInputPin(flat.types['CG.Entity'], 'Entity')

    self.speedOutPin = self:addOutputPin(flat.types['flat.Vector3'], 'Speed')
end

function GetProjectileSpeedNode:execute(runtime)
    local entity = runtime:readPin(self.entityInPin)

    local speed = entity:getProjectileSpeed()

    runtime:writePin(self.speedOutPin, speed)
end

return GetProjectileSpeedNode