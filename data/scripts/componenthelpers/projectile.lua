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

local function createSpawner(templatePath)
    local template = Path.requireComponentTemplate(templatePath, 'projectile')
    return function(position, target)
        return spawn(templatePath, template, position, target:getCenter())
    end
end

local function spawnFromEntity(templatePath, template, entity, attachPoint, target)
    return spawn(templatePath, template, entity:getAttachPoint(attachPoint), target:getCenter(), entity)
end

local function createSpawnerFromEntity(templatePath)
    local template = Path.requireComponentTemplate(templatePath, 'projectile')
    return function(entity, attachPoint, target)
        return spawnFromEntity(templatePath, template, entity, attachPoint, target)
    end
end

return {
    spawn                   = spawn,
    createSpawner           = createSpawner,

    spawnFromEntity         = spawnFromEntity,
    createSpawnerFromEntity = createSpawnerFromEntity
}