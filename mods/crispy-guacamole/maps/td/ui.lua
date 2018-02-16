local Preview = require 'data/scripts/preview'
local UiSettings = require 'data/scripts/ui/uisettings'

local root = Widget.getRoot()

local Theme = {
    BACKGROUND_COLOR = 0xFFFFFFCC,
    TEXT_COLOR       = 0x111111FF
}

-- right panel
do
    local rightPanel = Widget.makeColumnFlow()
    rightPanel:setPositionPolicy(Widget.PositionPolicy.RIGHT + Widget.PositionPolicy.CENTER_Y)
    rightPanel:setBackgroundColor(Theme.BACKGROUND_COLOR)
    rightPanel:setPadding(10)

    do
        local buildingTitleLabel = Widget.makeText('Buildings', table.unpack(UiSettings.defaultFont))
        buildingTitleLabel:setTextColor(Theme.TEXT_COLOR)
        rightPanel:addChild(buildingTitleLabel)
    end

    do
        local previewContainer = Widget.makeColumnFlow()
        previewContainer:setSizePolicy(Widget.SizePolicy.EXPAND_X + Widget.SizePolicy.COMPRESS_Y)
        do
            local preview = Preview.entity('tower')
            preview:setPositionPolicy(Widget.PositionPolicy.CENTER)
            previewContainer:addChild(preview)
        end

        do
            local tooltip = Widget.makeColumnFlow()
            tooltip:setPositionPolicy(Widget.PositionPolicy.TOP_RIGHT)
            tooltip:setBackgroundColor(Theme.BACKGROUND_COLOR)
            
            do
                local tooltipLabel = Widget.makeText('Tower', table.unpack(UiSettings.defaultFont))
                tooltipLabel:setTextColor(Theme.TEXT_COLOR)
                tooltip:addChild(tooltipLabel)
            end

            do
                previewContainer:mouseEnter(function()
                    print 'mouseEnter'
                    root:addChild(tooltip)
                end)

                previewContainer:mouseLeave(function()
                    print 'mouseLeave'
                    tooltip:removeFromParent()
                end)

                previewContainer:mouseMove(function()
                    local x, y = Mouse.getPosition()
                    print(x, y)
                    tooltip:setPosition(x, y)
                end)
            end
        end

        rightPanel:addChild(previewContainer)
    end

    root:addChild(rightPanel)
end