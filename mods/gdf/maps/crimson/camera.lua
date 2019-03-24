local Camera = require 'data/game/scripts/camera'

local math = math
local cos = math.cos
local sin = math.sin
local max = math.max
local sqrt = math.sqrt
local coyield = coroutine.yield

local function clamp(value, min, max)
    if value < min then
        return min
    elseif value > max then
        return max
    else
        return value
    end
end

local players = Map.getEntitiesOfType('player')
local player = players[1]

local cameraDistanceToPlayer = player:getSpeed() * 3

local playerHeading = player:getHeading()
local cameraPosition = player:getPosition()
local cameraVelocity = flat.Vector3(0, 0, 0)

local function clampMagnitude(v, max)
    local length2 = v:length2()
    if length2 > max * max then
        local length = sqrt(length2)
        local r = max / length
        return flat.Vector3(v:x() * r, v:y() * r, v:z() * r)
    else
        return v
    end
end

local function smoothDamp(current, target, currentVelocity, smoothTime, maxSpeed, deltaTime)
    smoothTime = max(0.0001, smoothTime)
    local omega = 2 / smoothTime

    local x = omega * deltaTime
    local exp = 1 / (1 + x + 0.48 * x * x + 0.235 * x * x * x)
    local change = current - target
    local originalTo = target

    local maxChange = maxSpeed * smoothTime
    change = clampMagnitude(change, maxChange)
    target = current - change;

    local temp = (currentVelocity + change * omega) * deltaTime
    currentVelocity = (currentVelocity - temp * omega) * exp
    local output = target + (change + temp) * exp

    if (originalTo - current):dot(output - originalTo) > 0 then
        output = originalTo
        currentVelocity = (output - originalTo) / deltaTime
    end

    return output, currentVelocity
end

while player:isValid() do
    local playerHeading = player:getHeading()
    local cameraTargetPosition = player:getPosition() + flat.Vector3(cos(playerHeading), sin(playerHeading), 0) * cameraDistanceToPlayer

    local dt = game.getDT()
    if dt > 0 then
        cameraPosition, cameraVelocity = smoothDamp(cameraPosition, cameraTargetPosition, cameraVelocity, 0.5, 10000, dt)
    else
        cameraPosition = cameraTargetPosition
    end

    Camera.lockAndMoveTo(cameraPosition, 0)

    coyield()
end