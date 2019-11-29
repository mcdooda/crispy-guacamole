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
            'Weight: ' .. componentTemplate.weight,
            table.unpack(UiSettings.defaultFont)
        )
        label:setMargin(2)
        componentDetailsPanel:addChild(label)
    end
    do
        local label = Widget.makeText(
            'Rotate Sprite: ' .. tostring(componentTemplate.rotateSprite),
            table.unpack(UiSettings.defaultFont)
        )
        label:setMargin(2)
        componentDetailsPanel:addChild(label)
    end
    do
        local shootContainer = Widget.makeColumnFlow()
        shootContainer:setSizePolicy(Widget.SizePolicy.EXPAND_X + Widget.SizePolicy.COMPRESS_Y)
        shootContainer:setMargin(2)
        shootContainer:setBackgroundColor(0x888888FF)
        do
            local elevationInput = Widget.makeNumberInput(table.unpack(UiSettings.defaultFont))
            local headingInput = Widget.makeNumberInput(table.unpack(UiSettings.defaultFont))

            do
                local elevationLine = Widget.makeLineFlow()
                elevationLine:setMargin(2)

                local elevationLabel = Widget.makeText(
                    'Elevation:',
                    table.unpack(UiSettings.defaultFont)
                )
                elevationLabel:setMargin(2)
                elevationLabel:setTextColor(0xFFFFFFFF)

                local elevationContainer = Widget.makeFixedSize(40, 19)
                elevationContainer:setBackgroundColor(0xFFFFFFFF)
                elevationContainer:setMargin(0, 0, 0, 2)
                elevationInput:setValue(0)
                elevationInput:setPrecision(2)
                elevationInput:setMargin(2)
                elevationInput:setSize(40, 15)
                elevationInput:setTextColor(0x000000FF)
                elevationInput:setSizePolicy(Widget.SizePolicy.FIXED_X + Widget.SizePolicy.FIXED_Y)
                elevationInput:setMin(-math.pi)
                elevationInput:setMax(math.pi)

                elevationInput:valueChanged(function(widget, value)
                    local entity = getEntity()
                    if not entity:isComponentEnabled(Component.projectile) then
                        entity:setElevation(value)
                    end
                end)

                elevationContainer:addChild(elevationInput)
                elevationLine:addChild(elevationLabel)
                elevationLine:addChild(elevationContainer)
                shootContainer:addChild(elevationLine)
            end

            do
                local headingLine = Widget.makeLineFlow()
                headingLine:setMargin(2)

                local headingLabel = Widget.makeText(
                    'Heading:  ',
                    table.unpack(UiSettings.defaultFont)
                )
                headingLabel:setMargin(2)
                headingLabel:setTextColor(0xFFFFFFFF)

                local headingContainer = Widget.makeFixedSize(40, 19)
                headingContainer:setBackgroundColor(0xFFFFFFFF)
                headingContainer:setMargin(0, 0, 0, 2)
                headingInput:setValue(0)
                headingInput:setPrecision(2)
                headingInput:setMargin(2)
                headingInput:setSize(40, 15)
                headingInput:setTextColor(0x000000FF)
                headingInput:setSizePolicy(Widget.SizePolicy.FIXED_X + Widget.SizePolicy.FIXED_Y)
                headingInput:setMin(0);
                headingInput:setMax(2 * math.pi)

                headingInput:valueChanged(function(widget, value)
                    local entity = getEntity()
                    if not entity:isComponentEnabled(Component.projectile) then
                        entity:setHeading(value)
                    end
                end)

                headingContainer:addChild(headingInput)
                headingLine:addChild(headingLabel)
                headingLine:addChild(headingContainer)
                shootContainer:addChild(headingLine)
            end

            do
                local shootLine = Widget.makeLineFlow()
                shootLine:setMargin(2)
                shootLine:setPositionPolicy(Widget.PositionPolicy.RIGHT + Widget.PositionPolicy.CENTER_Y)

                local buttonContainer = Widget.makeFixedSize(40, 19)
                buttonContainer:setBackgroundColor(0xDDDDDDFF)
                local shootLabel = Widget.makeText(
                    'Shoot',
                    table.unpack(UiSettings.defaultFont)
                )
                shootLabel:setMargin(2)
                shootLabel:setTextColor(0x000000FF)

                shootLabel:click(function()
                    local entity = getEntity()
                    if not entity:isComponentEnabled(Component.projectile) then
                        entity:setHeading(headingInput:getValue())
                        entity:setElevation(elevationInput:getValue())
                        entity:decComponentDisableLevel(Component.projectile)
                    end
                end)
                buttonContainer:addChild(shootLabel)
                shootLine:addChild(buttonContainer)
                shootContainer:addChild(shootLine)
            end
        end
        componentDetailsPanel:addChild(shootContainer)
    end
end