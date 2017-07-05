local UiSettings = require 'data/scripts/ui/uisettings'
local Preview = require 'data/scripts/preview'
local Icon = require 'data/scripts/ui/icon'

return function(componentDetailsPanel, entityTemplateName, componentTemplate, getEntity)
    do
        local label = Widget.makeText(
            'Collision type: ' .. componentTemplate.type,
            table.unpack(UiSettings.defaultFont)
        )
        label:setMargin(2)
        componentDetailsPanel:addChild(label)
    end
    do
        local label = Widget.makeText(
            'Radius: ' .. componentTemplate.radius,
            table.unpack(UiSettings.defaultFont)
        )
        label:setMargin(2)
        componentDetailsPanel:addChild(label)
    end
    do
        if componentTemplate.type == 'cylinder' then
            local label = Widget.makeText(
                'Height: ' .. componentTemplate.height,
                table.unpack(UiSettings.defaultFont)
            )
            label:setMargin(2)
            componentDetailsPanel:addChild(label)
        end
    end
    do
        local label = Widget.makeText(
            'Separate: ' .. (componentTemplate.separate and 'true' or 'false'),
            table.unpack(UiSettings.defaultFont)
        )
        label:setMargin(2)
        componentDetailsPanel:addChild(label)
    end
end