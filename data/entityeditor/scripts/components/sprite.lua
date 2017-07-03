local UiSettings = require 'data/scripts/ui/uisettings'
local Preview = require 'data/scripts/preview'
local Icon = require 'data/scripts/ui/icon'

return function(componentDetailsPanel, entityTemplateName, componentTemplate, entity)
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
                playIcon.container:setMargin(0, 0, 7, 0)
                playIcon.container:click(function()
                    entity:playAnimationAsync(animationName)
                end)

                animationLine:addChild(playIcon.container)
            end

            componentDetailsPanel:addChild(animationLine)
        end
    end
end