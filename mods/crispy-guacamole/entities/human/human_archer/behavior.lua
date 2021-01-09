local ProjectileHelper = require 'data/scripts/componenthelpers/projectile'
local BasicUnitBehavior = require(Mod.getFilePath 'scripts/basicunitbehavior')

local states = BasicUnitBehavior.basicUnit()

function states:useAbility(archer)
    local extraData = archer:getExtraData()
    local abilityTargetPosition = assert(extraData.abilityTargetPosition)
    extraData.abilityTargetPosition = nil
    archer:playAnimation('shoot', 1, false)
    do
        local projectileName = 'human_archer_explosive_arrow'
        local projectileAsset = assert(Asset.findFromName('entity', projectileName), 'Could not find entity asset ' .. projectileName)
        local spawnProjectile = ProjectileHelper.createSpawnerFromEntityToPositionIgnoreWeight(projectileAsset:getPath())
        spawnProjectile(archer, 'crossbow', abilityTargetPosition)
    end
    return extraData.currentLoopingState
end

return states