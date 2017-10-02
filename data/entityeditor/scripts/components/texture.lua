local UiSettings = require 'data/scripts/ui/uisettings'
local Preview = require 'data/scripts/preview'

return function(componentDetailsPanel, entityTemplateName, componentTemplate, getEntity)
    do
        local label = Widget.makeText(
            'Origin: ' .. componentTemplate.origin:x() .. ', ' .. componentTemplate.origin:y(),
            table.unpack(UiSettings.defaultFont)
        )
        label:setMargin(2)
        componentDetailsPanel:addChild(label)
    end
    do
        local texturePreview = Preview.entity(entityTemplateName)
        texturePreview:setMargin(2)
        componentDetailsPanel:addChild(texturePreview)
    end
end