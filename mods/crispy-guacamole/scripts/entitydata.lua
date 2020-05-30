local Path = require 'data/scripts/path'

local function get(entityTemplatePath)
    return Path.requireComponentTemplateIfExists(entityTemplatePath, 'data')
end

return {
    get = get
}