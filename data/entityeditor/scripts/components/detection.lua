local UiSettings = require 'data/scripts/ui/uisettings'
local Icon = require 'data/scripts/ui/icon'

return function(componentDetailsPanel, entityTemplatePath, componentTemplate, getEntity)
    do
        local label = Widget.makeText(
            'Vision range: ' .. componentTemplate.visionRange,
            table.unpack(UiSettings.defaultFont)
        )
        label:setMargin(2)
        componentDetailsPanel:addChild(label)
    end
end