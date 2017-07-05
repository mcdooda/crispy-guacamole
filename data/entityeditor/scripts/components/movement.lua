local UiSettings = require 'data/scripts/ui/uisettings'
local Preview = require 'data/scripts/preview'
local Icon = require 'data/scripts/ui/icon'

return function(componentDetailsPanel, entityTemplateName, componentTemplate, getEntity)
    do
        local label = Widget.makeText(
            'Speed: ' .. componentTemplate.speed,
            table.unpack(UiSettings.defaultFont)
        )
        label:setMargin(2)
        componentDetailsPanel:addChild(label)
    end
    do
        local label = Widget.makeText(
            'Jump force: ' .. componentTemplate.jumpForce,
            table.unpack(UiSettings.defaultFont)
        )
        label:setMargin(2)
        componentDetailsPanel:addChild(label)
    end
    do
        local label = Widget.makeText(
            'Weight: ' .. componentTemplate.weight,
            table.unpack(UiSettings.defaultFont)
        )
        label:setMargin(2)
        componentDetailsPanel:addChild(label)
    end
    do
        local jumpLine = Widget.makeLineFlow()
        jumpLine:setMargin(2)

        do
            local jumpLabel = Widget.makeText(
                'Jump',
                table.unpack(UiSettings.defaultFont)
            )
            jumpLine:addChild(jumpLabel)
        end

        do
            local playIcon = Icon:new('play', 10)
            playIcon.container:setMargin(0, 0, 1, 3)
            playIcon.container:click(function()
                local entity = getEntity()
                local jumped, errorMessage = pcall(function()
                    entity:jump(false)
                end)
                if not jumped then
                    print(errorMessage)
                end
            end)
            jumpLine:addChild(playIcon.container)
        end

        componentDetailsPanel:addChild(jumpLine)
    end
end