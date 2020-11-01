local BehaviorHelper = require 'data/scripts/componenthelpers/behavior'
local BarkSystem = require(Mod.getFilePath 'scripts/barksystem')

local followDistance = 4

local states = {}

function states:init(dog)
    
end

function states:followPlayer(dog)
    BarkSystem:requestBark(Mod.getFilePath 'sounds/dog_bark')
    dog:clearPath()
    local playerToFollow = assert(dog:getExtraData().playerToFollow)
    dog:setSpeed(playerToFollow:getSpeed())
    while true do
        if playerToFollow:isValid() then
            local dogPosition = dog:getPosition():toVector2()

            local playerToFollowPosition = playerToFollow:getPosition():toVector2()
            if playerToFollow:isFollowingPath() then
                local playerMovementDirection = playerToFollow:getCurrentMovementDirection()
                local hit, endPosition = Map.navigationRaycast(playerToFollowPosition, playerMovementDirection, followDistance + dog:getRadius(), 0.5, Map.Navigability.GROUND)
                if hit then
                    playerToFollowPosition = endPosition - playerMovementDirection * dog:getRadius()
                else
                    playerToFollowPosition = endPosition
                end
            end

            if (dogPosition - playerToFollowPosition):length2() > followDistance * followDistance then
                dog:moveTo(playerToFollowPosition)
            else
                coroutine.yield()
            end
        else
            return
        end
    end
end

return states