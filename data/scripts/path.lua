local require = require

local modPath = Mod.getPath()
local mapPath = modPath .. '/maps/' .. Mod.getCurrentMapName()

-- mod
local function getModPath()
    return modPath
end

local function getModFilePath(file)
    return modPath .. '/' .. file
end

local function requireModFile(file)
    return require(getModFilePath(file))
end

-- map
local function getMapPath()
    return mapPath
end

local function getMapFilePath(file)
    return mapPath .. '/' .. file
end

local function requireMapFile(file)
    return require(getMapFilePath(file))
end

local function requireMapConfig()
    return requireMapFile 'map'
end

-- entity
local function getEntityPath(templateName)
    return modPath .. '/entities/' .. templateName
end

local function getComponentPath(entityTemplateName, componentTemplateName)
    return getEntityPath(entityTemplateName) .. '/' .. componentTemplateName
end

local function requireComponentTemplate(entityTemplateName, componentTemplateName)
    return require(getComponentPath(entityTemplateName, componentTemplateName))
end

-- props
local function getPropPath(propName)
    return modPath .. '/props/' .. propName
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
    return modPath .. '/tiles/' .. tileName
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
    getModPath               = getModPath,
    getModFilePath           = getModFilePath,
    requireModFile           = requireModFile,

    getMapPath               = getMapPath,
    getMapFilePath           = getMapFilePath,
    requireMapFile           = requireMapFile,
    requireMapConfig         = requireMapConfig,

    getEntityPath            = getEntityPath,
    getComponentPath         = getComponentPath,
    requireComponentTemplate = requireComponentTemplate,

    getPropPath              = getPropPath,
    getPropFilePath          = getPropFilePath,
    requirePropFile          = requirePropFile,
    requirePropConfig        = requirePropConfig,

    getTilePath              = getTilePath,
    getTileFilePath          = getTileFilePath,
    requireTileFile          = requireTileFile,
    requireTileConfig        = requireTileConfig
}