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
local function getEntityFilePath(entityTemplatePath, file)
    return entityTemplatePath .. '/' .. file
end

local function getComponentPath(entityTemplatePath, componentTemplateName)
    return getEntityFilePath(entityTemplatePath, componentTemplateName)
end

local function componentFileExists(entityTemplatePath, componentTemplateName)
    local file = io.open(getComponentPath(entityTemplatePath, componentTemplateName) .. '.lua', 'r')
    if file then
        file:close()
        return true
    else
        return false
    end
end

local function requireComponentTemplate(entityTemplatePath, componentTemplateName, forceReload)
    local componentPath = getComponentPath(entityTemplatePath, componentTemplateName)
    if forceReload then
        package.loaded[componentPath] = nil
    end
    return require(componentPath)
end

local function requireComponentTemplateIfExists(entityTemplatePath, componentTemplateName, forceReload)
    local componentExists, componentTemplate = pcall(function()
        return requireComponentTemplate(entityTemplatePath, componentTemplateName, forceReload)
    end)
    if componentExists then
        assert(type(componentTemplate) == 'table')
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

    getEntityFilePath                = getEntityFilePath,
    getComponentPath                 = getComponentPath,
    componentFileExists              = componentFileExists,
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