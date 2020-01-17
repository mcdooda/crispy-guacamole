
local UiSettings = require 'data/scripts/ui/uisettings'
local Icon = require 'data/scripts/ui/icon'
local Dialog = require 'data/scripts/ui/dialog'
return function(addContainer, makeSeparator, font)

    local iconsContainer = addContainer 'Show Icons'


    local buttonsContainer = Widget.makeLineFlow()
    do

        local iconButton = Icon:new("palette", 12)
        iconButton:setColor(0x000000FF)
        iconButton.container:click(function()
            local content = Widget.makeColumnFlow()
            local lineWidget = Widget.makeLineFlow()
            lineWidget:setMargin(15)
            local i = 1
            for i = 1, #UiSettings.fontIcons do
                local fontIcon = UiSettings.fontIcons[i]
                local icons = require(Icon.iconFontsPath .. '/' .. fontIcon)
                for iconName in flat.sortedPairs(icons) do
                    if i%10 == 0 then
                        content:addChild(lineWidget)
                        lineWidget = Widget.makeLineFlow()
                        lineWidget:setMargin(15)
                    end
                    local preview = Widget.makeColumnFlow()
                    local iconNameWidget = Widget.makeText(iconName, table.unpack(UiSettings.defaultFont))
                    iconNameWidget:setTextColor(0x222222FF)
                    local iconWidget = Icon:new(iconName, 20)
                    iconWidget.container:setMargin(5)
                    iconWidget.text:mouseEnter(function(c) mouseEnter(iconWidget) end)
                    iconWidget.text:mouseLeave(function(c) mouseLeave(iconWidget) end)
                    iconWidget.container:click(function(c) mouseLeave(iconWidget) end)
                    iconWidget:setColor(0x222222FF)
                    preview:addChild(iconWidget.container)
                    preview:addChild(iconNameWidget)
                    preview:setMargin(5)
                    lineWidget:addChild(preview)
                    i = i + 1
                end
            end

            content:addChild(lineWidget)
            content:setSize(600,800)
            content:setSizePolicy(Widget.SizePolicy.FIXED)
            content:setAllowScrollY(true)
            local widgetTitle = Widget.makeText("Icons", table.unpack(UiSettings.titleFont))
            widgetTitle:setMargin(15)

            Dialog:new(content, {confirm = false, cancel = true, title=widgetTitle})
        end)
        buttonsContainer:addChild(iconButton.container)
    end

    iconsContainer:addChild(buttonsContainer)
end