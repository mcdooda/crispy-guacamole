local Camera = require 'data/game/scripts/camera'

local cos = math.cos
local sin = math.sin
local coyield = coroutine.yield

local players = Map.getEntitiesOfType('player')
local player = players[1]

local updatePeriod = 0.2

local cameraDistanceToPlayer = player:getSpeed() * 3

local playerHeading = player:getHeading()
local previousCameraPosition = player:getPosition() + flat.Vector3(cos(playerHeading), sin(playerHeading), 0) * cameraDistanceToPlayer

while player:isValid() do
    local playerHeading = player:getHeading()
    local cameraTargetPosition = player:getPosition() + flat.Vector3(cos(playerHeading), sin(playerHeading), 0) * cameraDistanceToPlayer

    if (previousCameraPosition - cameraTargetPosition):length2() > (cameraDistanceToPlayer * cameraDistanceToPlayer) then
        Camera.lockAndMoveTo(cameraTargetPosition, updatePeriod)
        previousCameraPosition = cameraTargetPosition
        local endTime = game.getTime() + updatePeriod
        while game.getTime() < endTime do
            coroutine.yield()
        end
    else
        coroutine.yield()
    end
end