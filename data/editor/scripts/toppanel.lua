
local Dialog = require 'data/scripts/ui/dialog'
local Button = require 'data/scripts/ui/button'
local UiSettings = require 'data/scripts/ui/uisettings'
local Icon = require 'data/scripts/ui/icon'
local Path = require 'data/scripts/path'

local COLORS = {
    white      = 0xFFFFFFFF,
    black      = 0x000000FF,
    lightBlack = 0x1A1A1AFF,
    red        = 0xA72C2CFF,
    lightRed   = 0xEA6565FF,
    orange     = 0xC1732DFF,
    blue       = 0x133EC8FF,
    lightBlue  = 0x7C81E5FF,
    clearBlue  = 0xC7E5FAFF,
    darkBlue   = 0x232896FF,
    cyan       = 0x37BFB7FF,
    green      = 0x2AA920FF,
    lightGreen = 0x7EB86CFF,
    darkGreen  = 0x357B3AFF,
    yellow     = 0xC1AC36FF,
    grey       = 0x626262FF,
    darkGrey   = 0x3D3D3DFF,
    lightGrey  = 0x545454FF,
    transparent= 0x00000000
}

local function mouseEnter(icon)
    icon:setColor(COLORS.lightBlue)
end
local function mouseLeave(icon)
    icon:setColor(COLORS.lightBlack)
end

local root = Widget.getRoot()
do

    local function showIconDialog()
        local content = Widget.makeColumnFlow()
        local lineWidget = Widget.makeLineFlow()
        lineWidget:setMargin(15)
        local i = 1
        for i = 1, #UiSettings.fontIcons do
            local fontIcon = UiSettings.fontIcons[i]
            local icons = require(Icon.iconFontsPath .. '/' .. fontIcon)
            for iconName in flat.sortedpairs(icons) do
                if i%10 == 0 then
                    content:addChild(lineWidget)
                    lineWidget = Widget.makeLineFlow()
                    lineWidget:setMargin(15)
                end
                local preview = Widget.makeColumnFlow()
                local iconNameWidget = Widget.makeText(iconName, table.unpack(UiSettings.defaultFont))
                iconNameWidget:setTextColor(COLORS.lightBlack)
                local iconWidget = Icon:new(iconName, 20)
                iconWidget.container:setMargin(5)
                iconWidget.text:mouseEnter(function(c) mouseEnter(iconWidget) end)
                iconWidget.text:mouseLeave(function(c) mouseLeave(iconWidget) end)
                iconWidget.container:click(function(c) mouseLeave(iconWidget) end)
                iconWidget:setColor(COLORS.lightBlack)
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
        local widgetTitle = Widget.makeText("Icones", table.unpack(UiSettings.titleFont))
        widgetTitle:setMargin(15)
        local dialog = Dialog:new(content, {confirm = false, cancel = true, title=widgetTitle})

    end
    local function test()
        print("test")
    end

    local topPanel = Widget.makeLineFlow()
    topPanel:setBackgroundColor(0x444444FF)
    topPanel:setSizePolicy(Widget.SizePolicy.EXPAND_X + Widget.SizePolicy.FIXED_Y)
    topPanel:setSize(0,40)
    topPanel:setMargin(0,0,0,128)

    local btnText = Widget.makeLineFlow()

    local okIcon = Icon:new("palette")

    okIcon.container:setMargin(0,5,0,0)
    okIcon:setColor(COLORS.white)
    btnText:addChild(okIcon.container)

    local btnConfirm = Button:new(btnText)
    local btnFlatTest = Button:new(Widget.makeText("test", table.unpack(UiSettings.defaultFont)))
    topPanel:addChild(btnConfirm.container)
    topPanel:addChild(btnFlatTest.container)
    btnConfirm.container:click(showIconDialog)
    btnFlatTest.container:click(test)
    root:addChild(topPanel)

end

