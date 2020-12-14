local require = require

-- mod
local getModPath = Mod.getPath
local getCurrentMapPath = Mod.getCurrentMapPath

local function getModFilePath(file)
    return getModPath() .. '/' .. file
end

local function requireModFile(file)
    return require(getModFilePath(file))
end

-- map
local function getMapFilePath(file)
    return getCurrentMapPath() .. '/' .. file
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

-- cache component file paths that do not exist
local nonExistingComponentTemplates = {}

local function requireComponentTemplateIfExists(entityTemplatePath, componentTemplateName, forceReload)
    local componentPath = getComponentPath(entityTemplatePath, componentTemplateName)
    local componentTemplate = package.loaded[componentPath]
    if componentTemplate then
        if forceReload then
            package.loaded[componentPath] = nil
            componentTemplate = require(componentPath)
        end
    else
        if not forceReload and nonExistingComponentTemplates[componentPath] then
            return nil
        end
        local searchers = package.searchers
        for i = 1, #searchers do
            local searcher = searchers[i]
            local loader = searcher(componentPath)
            if type(loader) == 'function' then
                package.preload[componentPath] = loader
                componentTemplate = require(componentPath)
                assert(type(componentTemplate) == 'table')
                return componentTemplate
            end
        end
        nonExistingComponentTemplates[componentPath] = true
        return nil
    end
    assert(type(componentTemplate) == 'table')
    return componentTemplate
end

-- props
local function getPropFilePath(propTemplatePath, propFile)
    return propTemplatePath .. '/' .. propFile
end

local function requirePropFile(propTemplatePath, propFile)
    return require(getPropFilePath(propTemplatePath, propFile))
end

local function requirePropConfig(propTemplatePath)
    return requirePropFile(propTemplatePath, 'prop')
end

-- tiles
local function getTileFilePath(tileTemplatePath, tileFile)
    return tileTemplatePath .. '/' .. tileFile
end

local function requireTileFile(tileTemplatePath, tileFile)
    return require(getTileFilePath(tileTemplatePath, tileFile))
end

local function requireTileConfig(tileTemplatePath)
    return requireTileFile(tileTemplatePath, 'tile')
end

return {
    getModPath                       = getModPath,
    getModFilePath                   = getModFilePath,
    requireModFile                   = requireModFile,

    getMapFilePath                   = getMapFilePath,
    requireMapFile                   = requireMapFile,
    requireMapConfig                 = requireMapConfig,

    getEntityFilePath                = getEntityFilePath,
    getComponentPath                 = getComponentPath,
    componentFileExists              = componentFileExists,
    requireComponentTemplate         = requireComponentTemplate,
    requireComponentTemplateIfExists = requireComponentTemplateIfExists,

    getPropFilePath                  = getPropFilePath,
    requirePropFile                  = requirePropFile,
    requirePropConfig                = requirePropConfig,

    getTileFilePath                  = getTileFilePath,
    requireTileFile                  = requireTileFile,
    requireTileConfig                = requireTileConfig
}