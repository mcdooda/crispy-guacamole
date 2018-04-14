local ProjectileHelper = require 'data/scripts/componenthelpers/projectile'
local cos = math.cos
local sin = math.sin
local sqrt = math.sqrt
local atan2 = math.atan

return {
    attackRange = 15,
    attackCooldown = 1,
    autoAttack = true,
    moveDuringAttack = false,
    attack = function(tower)
        local target = tower:getAttackTarget()

        if not target or not target:isValid() then
            return
        end

        local towerPosition = tower:getPosition()
        local targetPosition = target:getPosition()

        local heading = atan2(targetPosition:y() - towerPosition:y(), targetPosition:x() - towerPosition:x())
        local spawnPosition = flat.Vector3(
            towerPosition:x() + cos(heading) * 1 / 2,
            towerPosition:y() + sin(heading) * 1 / 2,
            towerPosition:z()
        )
        Entity.spawn('fx_explosion', spawnPosition)
        Entity.spawn('fire_arrow', spawnPosition, heading, elevation)
    end
}