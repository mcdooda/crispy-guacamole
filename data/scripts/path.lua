local modPath = Mod.getPath()
local require = require

local function getEntityPath(templateName)
    return modPath .. '/entities/' .. templateName
end

local function getComponentPath(entityTemplateName, componentTemplateName)
    return getEntityPath(entityTemplateName) .. '/' .. componentTemplateName
end

local function requireComponentTemplate(entityTemplateName, componentTemplateName)
    return require(getComponentPath(entityTemplateName, componentTemplateName))
end

return {
    getEntityPath            = getEntityPath,
    getComponentPath         = getComponentPath,
    requireComponentTemplate = requireComponentTemplate
}