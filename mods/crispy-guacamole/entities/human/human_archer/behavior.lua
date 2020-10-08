local BehaviorHelper = require 'data/scripts/componenthelpers/behavior'
local ProjectileHelper = require 'data/scripts/componenthelpers/projectile'
local BarkSystem = require(Mod.getFilePath 'scripts/barksystem')

local states = BehaviorHelper.basicAttacker()

function states:init(archer)

end

function states:followPlayer(archer)
    local extraData = archer:getExtraData()
    if extraData.previousLoopingState ~= 'followPlayer' then
        extraData.previousLoopingState = 'followPlayer'
        BarkSystem:requestBark(Mod.getFilePath 'sounds/human_male1_acknowledge')
    end
    archer:clearPath()
    local playerToFollow = assert(extraData.playerToFollow)
    archer:setSpeed(playerToFollow:getSpeed())
    local followDistance = 1 + math.random()
    while true do
        if playerToFollow:isValid() then
            local archerPosition = archer:getPosition():toVector2()

            local playerToFollowPosition = playerToFollow:getPosition():toVector2()
            if playerToFollow:isFollowingPath() then
                playerToFollowPosition = playerToFollowPosition + playerToFollow:getCurrentMovementDirection() * followDistance
            end

            if (archerPosition - playerToFollowPosition):length2() > followDistance * followDistance then
                archer:moveTo(playerToFollowPosition)
            else
                coroutine.yield()
            end
        else
            return
        end
    end
end

function states:useAbility(archer)
    local extraData = archer:getExtraData()
    local abilityTargetPosition = assert(extraData.abilityTargetPosition)
    extraData.abilityTargetPosition = nil
    do
        local projectileName = 'bouncing_arrow'
        local projectileAsset = assert(Asset.findFromName('entity', projectileName), 'Could not find entity asset ' .. projectileName)
        local spawnProjectile = ProjectileHelper.createSpawnerFromEntityToPosition(projectileAsset:getPath())
        spawnProjectile(archer, 'crossbow', abilityTargetPosition)
    end
    return extraData.previousLoopingState
end

return states