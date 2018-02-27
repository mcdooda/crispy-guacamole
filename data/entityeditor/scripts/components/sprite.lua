local UiSettings = require 'data/scripts/ui/uisettings'
local Preview = require 'data/scripts/preview'
local Icon = require 'data/scripts/ui/icon'

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
        local label = Widget.makeText(
            'Size: ' .. componentTemplate.size:x() .. ', ' .. componentTemplate.size:y(),
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

            do
                local animationPreview = Preview.entity(entityTemplateName, animationName, true)
                animationPreview:setMargin(2)
                animationLine:addChild(animationPreview)
            end

            do
                local animationDetails = Widget.makeColumnFlow()
                animationDetails:setSizePolicy(Widget.SizePolicy.FIXED_X + Widget.SizePolicy.COMPRESS_Y)
                animationDetails:setSize(60, 0)
                animationDetails:setMargin(2)

                do
                    local animationNameLabel = Widget.makeText(animationName, table.unpack(UiSettings.defaultFont))
                    animationDetails:addChild(animationNameLabel)
                end

                do
                    local animationFramesLabel = Widget.makeText(
                        animation.numFrames .. ' x ' .. animation.frameDuration .. 's',
                        table.unpack(UiSettings.defaultFont)
                    )
                    animationDetails:addChild(animationFramesLabel)
                end

                animationLine:addChild(animationDetails)
            end

            do
                local playIcon = Icon:new 'play'
                playIcon.container:setPositionPolicy(Widget.PositionPolicy.CENTER)
                playIcon.container:click(function()
                    local entity = getEntity()
                    entity:playAnimation(animationName, 1, false)
                end)

                animationLine:addChild(playIcon.container)
            end

            componentDetailsPanel:addChild(animationLine)
        end
    end
end