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
    local aPos = a:getPosition()
    local bPos = b:getPosition()
    return (aPos - bPos):length() - aRadius - bRadius
end

return {
    getTemplate         = getTemplate,

    getRadius           = getRadius,
    distanceMinusRadius = distanceMinusRadius
}