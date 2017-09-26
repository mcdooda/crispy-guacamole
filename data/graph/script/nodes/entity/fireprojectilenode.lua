local ScriptNode = flat.require 'graph/script/scriptnode'
local PinTypes = flat.require 'graph/pintypes'

local ProjectileHelper = require 'data/scripts/componenthelpers/projectile'

local FireProjectileNode = ScriptNode:inherit 'Fire Projectile'

function FireProjectileNode:buildPins()
    self.impulseInPin = self:addInputPin(PinTypes.IMPULSE, 'In')
    self.entityInPin = self:addInputPin(PinTypes.USERDATA, 'Entity')
    self.targetInPin = self:addInputPin(PinTypes.USERDATA, 'Target')
    self.projectileNameInPin = self:addInputPin(PinTypes.STRING, 'Projectile')
    self.attachPointPin = self:addInputPin(PinTypes.STRING, 'Attach Point')
    self.delayInPin = self:addInputPin(PinTypes.NUMBER, 'Delay')

    self.impulseOutPin = self:addOutputPin(PinTypes.IMPULSE, 'Out')
end

function FireProjectileNode:execute(runtime, inputPin)
    assert(inputPin == self.impulseInPin)
    local entity = runtime:readPin(self.entityInPin)
    local target = runtime:readPin(self.targetInPin)
    local projectileName = runtime:readPin(self.projectileNameInPin)
    local attachPoint = runtime:readPin(self.attachPointPin)
    local delay = runtime:readPin(self.delayInPin)

    Timer.start(delay, nil, function()
        if not entity:isValid() or not target:isValid() then
            return
        end

        local spawnProjectile = ProjectileHelper.createSpawnerFromEntity(projectileName)
        spawnProjectile(entity, attachPoint, target)
    end)

    runtime:impulse(self.impulseOutPin)
end

return FireProjectileNode