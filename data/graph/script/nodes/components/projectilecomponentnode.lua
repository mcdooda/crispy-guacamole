local FunctionalScriptNode = flat.require 'graph/script/functionalscriptnode'
local PinTypes = flat.require 'graph/pintypes'

local ProjectileComponentNode = FunctionalScriptNode:inherit 'Projectile Component'

function ProjectileComponentNode:buildPins()
    self.weightInPin = self:addInputPin(PinTypes.NUMBER, 'Weight')
    self.speedInPin = self:addInputPin(PinTypes.NUMBER, 'Speed')
    self.onCollideInPin = self:addInputPin(PinTypes.FUNCTION, 'On Collide')

    self.componentOutPin = self:addOutputPin(PinTypes.TABLE, 'Component')
end

function ProjectileComponentNode:execute(runtime)
    local weight = runtime:readPin(self.weightInPin)
    local speed = runtime:readPin(self.speedInPin)
    local onCollide = runtime:readPin(self.onCollideInPin)

    local component = {
        weight = weight,
        speed = speed,
        collidedCallback = onCollide
    }

    runtime:writePin(self.componentOutPin, component)
end

return ProjectileComponentNode