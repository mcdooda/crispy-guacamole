local require = require

-- mod
local getModPath = Mod.getPath
local getCurrentMapName = Mod.getCurrentMapName

local function getModFilePath(file)
    return getModPath() .. '/' .. file
end

local function requireModFile(file)
    return require(getModFilePath(file))
end

-- map
local function getMapPath()
    return getModPath() .. '/maps/' .. getCurrentMapName()
end

local function getMapFilePath(file)
    return getMapPath() .. '/' .. file
end

local function requireMapFile(file)
    return require(getMapFilePath(file))
end

local function requireMapConfig()
    return requireMapFile 'map'
end

-- entity
local function getEntityPath(templateName)
    return getModPath() .. '/entities/' .. templateName
end

local function getEntityFilePath(entityTemplateName, file)
    return getEntityPath(entityTemplateName) .. '/' .. file
end

local function getComponentPath(entityTemplateName, componentTemplateName)
    return getEntityFilePath(entityTemplateName, componentTemplateName)
end

local function requireComponentTemplate(entityTemplateName, componentTemplateName, forceReload)
    local componentPath = getComponentPath(entityTemplateName, componentTemplateName)
    if forceReload then
        package.loaded[componentPath] = nil
    end
    return require(componentPath)
end

local function requireComponentTemplateIfExists(entityTemplateName, componentTemplateName, forceReload)
    local componentExists, componentTemplate = pcall(function()
        return requireComponentTemplate(entityTemplateName, componentTemplateName, forceReload)
    end)
    if componentExists then
        return componentTemplate
    end
end

-- props
local function getPropPath(propName)
    return getModPath() .. '/props/' .. propName
end

local function getPropFilePath(propName, propFile)
    return getPropPath(propName) .. '/' .. propFile
end

local function requirePropFile(propName, propFile)
    return require(getPropFilePath(propName, propFile))
end

local function requirePropConfig(propName)
    return requirePropFile(propName, 'prop')
end

-- tiles
local function getTilePath(tileName)
    return getModPath() .. '/tiles/' .. tileName
end

local function getTileFilePath(tileName, tileFile)
    return getTilePath(tileName) .. '/' .. tileFile
end

local function requireTileFile(tileName, tileFile)
    return require(getTileFilePath(tileName, tileFile))
end

local function requireTileConfig(tileName)
    return requireTileFile(tileName, 'tile')
end

return {
    getModPath                       = getModPath,
    getModFilePath                   = getModFilePath,
    requireModFile                   = requireModFile,

    getMapPath                       = getMapPath,
    getMapFilePath                   = getMapFilePath,
    requireMapFile                   = requireMapFile,
    requireMapConfig                 = requireMapConfig,

    getEntityPath                    = getEntityPath,
    getEntityFilePath                = getEntityFilePath,
    getComponentPath                 = getComponentPath,
    requireComponentTemplate         = requireComponentTemplate,
    requireComponentTemplateIfExists = requireComponentTemplateIfExists,

    getPropPath                      = getPropPath,
    getPropFilePath                  = getPropFilePath,
    requirePropFile                  = requirePropFile,
    requirePropConfig                = requirePropConfig,

    getTilePath                      = getTilePath,
    getTileFilePath                  = getTileFilePath,
    requireTileFile                  = requireTileFile,
    requireTileConfig                = requireTileConfig
}