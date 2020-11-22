local BehaviorHelper = require 'data/scripts/componenthelpers/behavior'
local states = BehaviorHelper.basicAttacker()
local BarkSystem = require(Mod.getFilePath 'scripts/barksystem')

local followDistance = 2

function states:init(paladin)

end

function states:followPlayer(soldier)
    local extraData = soldier:getExtraData()
    if extraData.previousLoopingState ~= 'followPlayer' then
        extraData.previousLoopingState = 'followPlayer'
        BarkSystem:requestBark(Mod.getFilePath 'sounds/human_male1_acknowledge')
    end
    soldier:clearPath()
    local playerToFollow = assert(extraData.playerToFollow)
    soldier:setSpeed(playerToFollow:getSpeed())
    while true do
        if playerToFollow:isValid() then
            local soldierPosition = soldier:getPosition():toVector2()

            local playerToFollowPosition = playerToFollow:getPosition():toVector2()
            if playerToFollow:isFollowingPath() then
                local playerMovementDirection = playerToFollow:getCurrentMovementDirection()
                local hit, endPosition = Map.navigationRaycast(playerToFollowPosition, playerMovementDirection, followDistance + soldier:getRadius(), 0.5, Map.Navigability.GROUND)
                if hit then
                    playerToFollowPosition = endPosition - playerMovementDirection * soldier:getRadius()
                else
                    playerToFollowPosition = endPosition
                end
            end

            if (soldierPosition - playerToFollowPosition):length2() > followDistance * followDistance then
                soldier:moveTo(playerToFollowPosition)
            else
                coroutine.yield()
            end
        else
            return
        end
    end
end

function states:useAbilityA(soldier)
    local extraData = soldier:getExtraData()
    soldier:playAnimation("cast", 1, false)

    local abilityTargetPosition = assert(extraData.abilityTargetPosition)
    extraData.abilityTargetPosition = nil
    do
        Entity.spawn('fx_fist', abilityTargetPosition)
    end
    return extraData.previousLoopingState
end

return states