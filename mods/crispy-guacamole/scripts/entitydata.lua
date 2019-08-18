local Path = require 'data/scripts/path'

local function get(entityTemplateName)
    return Path.requireComponentTemplateIfExists(entityTemplateName, 'data')
end

return {
    get = get
}