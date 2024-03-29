local ScriptNode = flat.require 'graph/script/scriptnode'
local PinTypes = flat.require 'graph/pintypes'

local ProjectileHelper = require 'data/scripts/componenthelpers/projectile'

local FireProjectileNode = ScriptNode:inherit 'Fire Projectile'

function FireProjectileNode:buildPins()
    self.impulseInPin = self:addInputPin(PinTypes.IMPULSE, 'In')
    self.entityInPin = self:addInputPin(flat.types['CG.Entity'], 'Entity')
    self.targetInPin = self:addInputPin(flat.types['CG.Entity'], 'Target')
    self.projectileNameInPin = self:addInputPin(flat.types.STRING, 'Projectile')
    self.attachPointPin = self:addInputPin(flat.types.STRING, 'Attach Point')
    self.delayInPin = self:addInputPin(flat.types.NUMBER, 'Delay')
    self.followTargetInPin = self:addInputPin(flat.types.BOOLEAN, 'Follow Target')

    self.impulseOutPin = self:addOutputPin(PinTypes.IMPULSE, 'Out')
    self.firedOutPin = self:addOutputPin(PinTypes.IMPULSE, 'Fired')
    self.projectileOutPin = self:addOutputPin(flat.types['CG.Entity'], 'Projectile')
    self.failedOutPin = self:addOutputPin(PinTypes.IMPULSE, 'Failed')
end

function FireProjectileNode:execute(runtime, inputPin)
    assert(inputPin == self.impulseInPin)
    local entity = runtime:readPin(self.entityInPin)
    local target = runtime:readPin(self.targetInPin)
    local projectileName = runtime:readPin(self.projectileNameInPin)
    local attachPoint = runtime:readPin(self.attachPointPin)
    local delay = runtime:readPin(self.delayInPin)
    local followTarget = runtime:readPin(self.followTargetInPin)

    local projectileAsset = assert(Asset.findFromName('entity', projectileName), 'Could not find entity asset ' .. projectileName)

    local function fireProjectile()
        if not entity:isValid() or not target:isValid() then
            return
        end

        local spawnProjectile = ProjectileHelper.createSpawnerFromEntityToPositionIgnoreWeight(projectileAsset:getPath())
        local projectile = spawnProjectile(entity, attachPoint, target:getCollisionCenter())

        if projectile then
            if followTarget then
                projectile:setProjectileTarget(target)
            end

            runtime:writePin(self.projectileOutPin, projectile)
            runtime:impulse(self.firedOutPin)
        else
            runtime:impulse(self.failedOutPin)
        end
    end

    if delay > 0 then
        local timer = game.Timer()
        timer:onEnd(fireProjectile)
        timer:start(delay)
    else
        fireProjectile()
    end

    runtime:impulse(self.impulseOutPin)
end

return FireProjectileNode