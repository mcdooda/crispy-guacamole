local Path = require 'data/scripts/path'

local sqrt = math.sqrt

local function getTemplate(entity)
    return Path.requireComponentTemplateIfExists(entity:getTemplateName(), 'collision')
end 

local function getRadius(entity)
    local template = getTemplate(entity)
    if template then
        return true, template.radius
    else
        return false, 0
    end
end

local function distanceMinusRadius(a, b)
    local _, aRadius = getRadius(a)
    local _, bRadius = getRadius(b)
    local aX, aY = a:getPosition()
    local bX, bY = b:getPosition()
    return sqrt((aX - bX) * (aX - bX) + (aY - bY) * (aY - bY)) - aRadius - bRadius
end

return {
    getTemplate         = getTemplate,

    getRadius           = getRadius,
    distanceMinusRadius = distanceMinusRadius
}