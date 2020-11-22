local BehaviorHelper = require 'data/scripts/componenthelpers/behavior'
local states = BehaviorHelper.basicAttacker()
local BarkSystem = require(Mod.getFilePath 'scripts/barksystem')

local followDistance = 2

function states:init(paladin)

end

function states:followPlayer(paladin)
    local extraData = paladin:getExtraData()
    if extraData.previousLoopingState ~= 'followPlayer' then
        extraData.previousLoopingState = 'followPlayer'
        BarkSystem:requestBark(Mod.getFilePath 'sounds/human_male1_acknowledge')
    end
    paladin:clearPath()
    local playerToFollow = assert(extraData.playerToFollow)
    paladin:setSpeed(playerToFollow:getSpeed())
    while true do
        if playerToFollow:isValid() then
            local paladinPosition = paladin:getPosition():toVector2()

            local playerToFollowPosition = playerToFollow:getPosition():toVector2()
            if playerToFollow:isFollowingPath() then
                local playerMovementDirection = playerToFollow:getCurrentMovementDirection()
                local hit, endPosition = Map.navigationRaycast(playerToFollowPosition, playerMovementDirection, followDistance + paladin:getRadius(), 0.5, Map.Navigability.GROUND)
                if hit then
                    playerToFollowPosition = endPosition - playerMovementDirection * paladin:getRadius()
                else
                    playerToFollowPosition = endPosition
                end
            end

            if (paladinPosition - playerToFollowPosition):length2() > followDistance * followDistance then
                paladin:moveTo(playerToFollowPosition)
            else
                coroutine.yield()
            end
        else
            return
        end
    end
end

function states:useAbility(paladin)
    local extraData = paladin:getExtraData()
    paladin:playAnimation("cast", 1, false)

    local abilityTargetPosition = assert(extraData.abilityTargetPosition)
    extraData.abilityTargetPosition = nil
    do
        Entity.spawn('fx_fist', abilityTargetPosition)
    end
    return extraData.previousLoopingState
end

return states