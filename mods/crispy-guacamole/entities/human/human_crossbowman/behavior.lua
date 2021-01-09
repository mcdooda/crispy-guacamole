local ProjectileHelper = require 'data/scripts/componenthelpers/projectile'
local BasicUnitBehavior = require(Mod.getFilePath 'scripts/basicunitbehavior')

local states = BasicUnitBehavior.basicUnit()

function states:useAbility(crossbowman)
    local extraData = crossbowman:getExtraData()
    local abilityTargetPosition = assert(extraData.abilityTargetPosition)
    extraData.abilityTargetPosition = nil
    crossbowman:playAnimation('special', 1, false)
    do
        local projectileName = 'explosive_arrow'
        local projectileAsset = assert(Asset.findFromName('entity', projectileName), 'Could not find entity asset ' .. projectileName)
        local spawnProjectile = ProjectileHelper.createSpawnerFromEntityToPositionIgnoreWeight(projectileAsset:getPath())
        spawnProjectile(crossbowman, 'crossbow', abilityTargetPosition)
    end
    return extraData.currentLoopingState
end

return states