
local Dialog = require 'data/scripts/ui/dialog'
local Button = require 'data/scripts/ui/button'
local UiSettings = require 'data/scripts/ui/uisettings'
local icon = require 'data/scripts/ui/icon'
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


local root = Widget.getRoot()
do

    local function showIconDialog()
        local content = Widget.makeColumnFlow()
        local lineWidget = Widget.makeLineFlow()
        lineWidget:setMargin(15)
        local i = 1
        for iconName, text in pairs(UiSettings.icons) do
            if i%10 == 0 then
                content:addChild(lineWidget)
                lineWidget = Widget.makeLineFlow()
                lineWidget:setMargin(15)
            end
            local preview = Widget.makeColumnFlow()
            local iconNameWidget = Widget.makeText(iconName, table.unpack(UiSettings.defaultFont))
            iconNameWidget:setTextColor(COLORS.lightBlack)
            local iconWidget = icon:new(iconName, 20)
            iconWidget.container:setMargin(5)
            iconWidget.container:setTextColor(COLORS.lightBlack)
            preview:addChild(iconWidget.container)
            preview:addChild(iconNameWidget)
            preview:setMargin(5)
            lineWidget:addChild(preview)
            i = i + 1
        end
        content:addChild(lineWidget)

        local lineWidget2 = Widget.makeLineFlow()
        lineWidget2:setMargin(15)
        i = 1
        for iconName, text in pairs(UiSettings.customIcons) do
            if i%10 == 0 then
                content:addChild(lineWidget2)
                lineWidget2 = Widget.makeLineFlow()
                lineWidget2:setMargin(15)
            end
            local preview2 = Widget.makeColumnFlow()
            local iconNameWidget = Widget.makeText(iconName, table.unpack(UiSettings.defaultFont))
            iconNameWidget:setTextColor(COLORS.lightBlack)
            local iconWidget = icon:new(iconName, 20)
            iconWidget.container:setMargin(5)
            iconWidget.container:setTextColor(COLORS.lightBlack)
            preview2:addChild(iconWidget.container)
            preview2:addChild(iconNameWidget)
            preview2:setMargin(5)
            lineWidget2:addChild(preview2)
            i = i + 1
        end
        content:addChild(lineWidget2)
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

    local okIcon = icon:new("palette")

    okIcon.container:setMargin(0,5,0,0)
    okIcon.container:setTextColor(COLORS.white)
    btnText:addChild(okIcon.container)

    local btnConfirm = Button:new(btnText)
    local btnFlatTest = Button:new(Widget.makeText("test", table.unpack(UiSettings.defaultFont)))
    topPanel:addChild(btnConfirm.container)
    topPanel:addChild(btnFlatTest.container)
    btnConfirm.container:click(showIconDialog)
    btnFlatTest.container:click(function()

        -- local content = Widget.makeLineFlow()
        -- flat.node.editor.open(content, Path.getComponentPath("Zombie", "attack"))

        -- local widgetTitle = Widget.makeText("Icones", table.unpack(UiSettings.titleFont))
        -- widgetTitle:setMargin(15)
        -- local dialog = Dialog:new(content, {confirm = false, cancel = true, title=widgetTitle})
    end)
    root:addChild(topPanel)

end

