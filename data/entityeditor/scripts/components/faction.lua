local UiSettings = require 'data/scripts/ui/uisettings'

return function(componentDetailsPanel, entityTemplatePath, componentTemplate, getEntity)
    do
        local label = Widget.makeText(
            'Faction: ' .. componentTemplate.faction,
            table.unpack(UiSettings.defaultFont)
        )
        label:setMargin(2)
        componentDetailsPanel:addChild(label)
    end
end