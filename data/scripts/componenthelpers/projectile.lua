local sqrt = math.sqrt
local atan = math.atan
local atan2 = atan

local Path = require 'data/scripts/path'

local function spawn(templatePath, template, position, targetPosition, instigator)
    -- compute initial heading and elevation
    local heading = atan2(targetPosition:y() - position:y(), targetPosition:x() - position:x())
    local dXY = (targetPosition - position):toVector2():length()
    local dZ = targetPosition:z() - position:z()
    local speedXY = template.speed
    local weight = template.weight
    local speedXY2 = speedXY * speedXY
    local speedXY4 = speedXY2 * speedXY2
    local elevation = atan(
        --        + to get the opposite angle
        (speedXY2 - sqrt(speedXY4 - weight * (weight * dXY * dXY + 2 * dZ * speedXY2)))
        / (weight * dXY)
    )

    -- TODO: elevation is NaN -> entity's attack distance should be computed from the projectile's template
    if elevation ~= elevation then
        return
    end

    return Entity.spawn(templatePath, position, heading, elevation, instigator)
end

local function spawnWithSpeed(templatePath, position, targetPosition, instigator, speed)
    -- compute initial heading and weight
    local heading = atan2(targetPosition:y() - position:y(), targetPosition:x() - position:x())
    local dXY = (targetPosition - position):toVector2():length()
    local dZ = targetPosition:z() - position:z()
    local speedXY = speed
    local speedZ = speedXY
    local duration = dXY / speedXY
    local elevation = math.pi * 0.25
    local initialVelocity = math.sqrt(speedXY * speedXY + speedZ * speedZ)
    local slopeAngle = math.atan(dZ / dXY)

    --[[
    See https://courses.lumenlearning.com/boundless-physics/chapter/projectile-motion/
    T = (2⋅u⋅sin(θ−α))/(g⋅cos(α)) => g = (2⋅u⋅sin(θ−α))/(T⋅cos(α))
    ]]
    local weight = (2 * initialVelocity * math.sin(elevation - slopeAngle)) / (duration * math.cos(slopeAngle))

    local projectile = Entity.spawn(templatePath, position, heading, elevation, instigator)
    projectile:setPosition(position)
    projectile:setHeading(heading)
    projectile:setElevation(elevation)
    projectile:setProjectileWeight(weight)
    projectile:setProjectileSpeed(flat.Vector3(math.cos(heading) * speedXY, math.sin(heading) * speedXY, speedZ))

    return projectile
end

local function createSpawner(templatePath)
    local template = Path.requireComponentTemplate(templatePath, 'projectile')
    return function(position, targetEntity)
        return spawn(templatePath, template, position, targetEntity:getCenter())
    end
end

local function createSpawnerFromEntity(templatePath)
    local template = Path.requireComponentTemplate(templatePath, 'projectile')
    return function(entity, attachPoint, targetEntity)
        return spawn(
            templatePath,
            template,
            entity:getAttachPoint(attachPoint),
            targetEntity:getCenter()
        )
    end
end

local function createSpawnerFromEntityToPosition(templatePath)
    local template = Path.requireComponentTemplate(templatePath, 'projectile')
    return function(entity, attachPoint, targetPosition)
        return spawn(
            templatePath,
            template,
            entity:getAttachPoint(attachPoint),
            targetPosition,
            entity
        )
    end
end

local function createSpawnerFromEntityToPositionWithSpeed(templatePath)
    return function(entity, attachPoint, targetPosition, speed)
        return spawnWithSpeed(
            templatePath,
            entity:getAttachPoint(attachPoint),
            targetPosition,
            entity,
            speed
        )
    end
end

return {
    spawn                             = spawn,
    createSpawner                     = createSpawner,

    spawnFromEntity                   = spawnFromEntity,
    createSpawnerFromEntity           = createSpawnerFromEntity,
    createSpawnerFromEntityToPosition = createSpawnerFromEntityToPosition,

    createSpawnerFromEntityToPositionWithSpeed = createSpawnerFromEntityToPositionWithSpeed
}