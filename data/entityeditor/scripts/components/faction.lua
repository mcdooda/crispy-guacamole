local UiSettings = require 'data/scripts/ui/uisettings'
local Preview = require 'data/scripts/preview'
local Icon = require 'data/scripts/ui/icon'

return function(componentDetailsPanel, entityTemplateName, componentTemplate, getEntity)
    do
        local label = Widget.makeText(
            'Faction: ' .. componentTemplate.faction,
            table.unpack(UiSettings.defaultFont)
        )
        label:setMargin(2)
        componentDetailsPanel:addChild(label)
    end
end