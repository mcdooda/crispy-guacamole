local sqrt = math.sqrt
local atan = math.atan
local atan2 = atan

local Path = require 'data/scripts/path'

local function spawn(templateName, template, position, targetPosition, instigator)
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

    return Entity.spawn(templateName, position, heading, elevation, instigator)
end

local function createSpawner(templateName)
    local template = Path.requireComponentTemplate(templateName, 'projectile')
    return function(position, target)
        return spawn(templateName, template, position, target:getPosition())
    end
end

local function spawnFromEntity(templateName, template, entity, attachPoint, target)
    return spawn(templateName, template, entity:getAttachPoint(attachPoint), target:getPosition(), entity)
end

local function createSpawnerFromEntity(templateName)
    local template = Path.requireComponentTemplate(templateName, 'projectile')
    return function(entity, attachPoint, target)
        return spawnFromEntity(templateName, template, entity, attachPoint, target)
    end
end

return {
    spawn                   = spawn,
    createSpawner           = createSpawner,

    spawnFromEntity         = spawnFromEntity,
    createSpawnerFromEntity = createSpawnerFromEntity
}