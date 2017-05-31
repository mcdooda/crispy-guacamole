local sqrt = math.sqrt
local atan = math.atan
local atan2 = atan

local Path = require 'data/scripts/path'

local function spawn(templateName, template, x, y, z, tx, ty, tz)
    -- compute initial heading and elevation
    local heading = atan2(ty - y, tx - x)
    local dXY = sqrt((tx - x) * (tx - x) + (ty - y) * (ty - y))
    local dZ = tz - z
    local speedXY = template.speed
    local weight = template.weight
    local speedXY2 = speedXY * speedXY
    local speedXY4 = speedXY2 * speedXY2
    local elevation = atan(
        (speedXY2 - sqrt(speedXY4 - weight * (weight * dXY * dXY + 2 * dZ * speedXY2)))
        / (weight * dXY)
    )

    -- TODO: elevation is NaN -> entity's attack distance should be computed from the projectile's template
    if elevation ~= elevation then
        return
    end

    return Entity.spawn(templateName, x, y, z, heading, elevation)
end

local function createSpawner(templateName)
    local template = Path.requireComponentTemplate(templateName, 'projectile')
    return function(x, y, z, target)
        local tx, ty, tz = target:getPosition()
        return spawn(templateName, template, x, y, z, tx, ty, tz)
    end
end

local function spawnFromEntity(templateName, template, entity, attachPoint, target)
    local x, y, z = entity:getAttachPoint(attachPoint)
    local tx, ty, tz = target:getPosition()
    return spawn(templateName, template, x, y, z, tx, ty, tz)
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