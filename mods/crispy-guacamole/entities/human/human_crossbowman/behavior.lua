local BehaviorHelper = require 'data/scripts/componenthelpers/behavior'
local ProjectileHelper = require 'data/scripts/componenthelpers/projectile'
local BarkSystem = require(Mod.getFilePath 'scripts/barksystem')

local states = BehaviorHelper.basicAttacker()
local followDistance = 2

function states:init(paladin)

end

function states:followPlayer(crossbowman)
    local extraData = crossbowman:getExtraData()
    if extraData.previousLoopingState ~= 'followPlayer' then
        extraData.previousLoopingState = 'followPlayer'
        BarkSystem:requestBark(Mod.getFilePath 'sounds/human_male1_acknowledge')
    end
    crossbowman:clearPath()
    local playerToFollow = assert(extraData.playerToFollow)
    crossbowman:setSpeed(playerToFollow:getSpeed())
    while true do
        if playerToFollow:isValid() then
            local crossbowmanPosition = crossbowman:getPosition():toVector2()

            local playerToFollowPosition = playerToFollow:getPosition():toVector2()
            if playerToFollow:isFollowingPath() then
                local playerMovementDirection = playerToFollow:getCurrentMovementDirection()
                local hit, endPosition = Map.navigationRaycast(playerToFollowPosition, playerMovementDirection, followDistance + crossbowman:getRadius(), 0.5, Map.Navigability.GROUND)
                if hit then
                    playerToFollowPosition = endPosition - playerMovementDirection * crossbowman:getRadius()
                else
                    playerToFollowPosition = endPosition
                end
            end

            if (crossbowmanPosition - playerToFollowPosition):length2() > followDistance * followDistance then
                crossbowman:moveTo(playerToFollowPosition)
            else
                coroutine.yield()
            end
        else
            return
        end
    end
end

function states:useAbility(crossbowman)
    local extraData = crossbowman:getExtraData()
    local abilityTargetPosition = assert(extraData.abilityTargetPosition)
    extraData.abilityTargetPosition = nil
    crossbowman:playAnimation("special", 1, false)
    do
        local projectileName = 'explosive_arrow'
        local projectileAsset = assert(Asset.findFromName('entity', projectileName), 'Could not find entity asset ' .. projectileName)
        local spawnProjectile = ProjectileHelper.createSpawnerFromEntityToPosition(projectileAsset:getPath())
        spawnProjectile(crossbowman, 'crossbow', abilityTargetPosition)
    end
    return extraData.previousLoopingState
end

return states