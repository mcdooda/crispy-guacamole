local UiSettings = require 'data/scripts/ui/uisettings'
local Preview = require 'data/scripts/preview'

return function(componentDetailsPanel, entityTemplateName, componentTemplate)
    do
        local label = Widget.makeText(
            'Origin: ' .. componentTemplate.origin[1] .. ', ' .. componentTemplate.origin[2],
            table.unpack(UiSettings.defaultFont)
        )
        label:setMargin(2)
        componentDetailsPanel:addChild(label)
    end
    do
        local label = Widget.makeText(
            'Size: ' .. componentTemplate.size[1] .. ', ' .. componentTemplate.size[2],
            table.unpack(UiSettings.defaultFont)
        )
        label:setMargin(2)
        componentDetailsPanel:addChild(label)
    end
    do
        local label = Widget.makeText(
            'Animations',
            table.unpack(UiSettings.defaultFont)
        )
        label:setMargin(2)
        componentDetailsPanel:addChild(label)

        local animationsSorted = {}
        for animationName, animation in pairs(componentTemplate.animations) do
            animationsSorted[#animationsSorted + 1] = animationName
        end
        table.sort(animationsSorted)

        for i = 1, #animationsSorted do
            local animationName = animationsSorted[i]
            local animation = componentTemplate.animations[animationName]
            
            local animationLine = Widget.makeLineFlow()

            local animationPreview = Preview.entity(entityTemplateName, animationName, true)
            animationPreview:setMargin(2)
            animationLine:addChild(animationPreview)

            local animationDetails = Widget.makeColumnFlow()
            animationDetails:setMargin(2)

            local animationNameLabel = Widget.makeText(animationName, table.unpack(UiSettings.defaultFont))
            animationDetails:addChild(animationNameLabel)

            local animationFramesLabel = Widget.makeText(
                animation.numFrames .. ' x ' .. animation.frameDuration .. 's',
                table.unpack(UiSettings.defaultFont)
            )
            animationDetails:addChild(animationFramesLabel)

            animationLine:addChild(animationDetails)

            componentDetailsPanel:addChild(animationLine)
        end
    end
end