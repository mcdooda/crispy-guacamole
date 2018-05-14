local UiSettings = require 'data/scripts/ui/uisettings'

local root = Widget.getRoot()

local button = require 'data/scripts/ui/button'
local loadingBar = require 'data/scripts/ui/loadingbar'
local icon = require 'data/scripts/ui/icon'

local Dialog = {}
Dialog.__index = Dialog

local root = Widget.getRoot()


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

function Dialog:cancel()
    -- self.loadingBar:setValue(0)
    self:close()
    if self.onCancel then
        self.onCancel()
    end
end


function Dialog:confirm()
    self.loadingBar:setValue(100)
    -- self:close()
    if self.onConfirm then
        self.onConfirm()
    end
end

function Dialog:close()
    local maxTimer = 0.15
    self.background:removeFromParent()
    local timer = flat.Timer()
    timer:onUpdate(function(timer, elapsedTime)

       local _, rootH = root:getSize()
       self.container:setPosition(0, (1 - (maxTimer - elapsedTime) / maxTimer) * rootH)
    end)
    timer:onEnd(function(timer)
        self.container:removeFromParent()
    end)
    timer:start(maxTimer)

end


function Dialog:new(content, options)

    local background = Widget.makeExpand()
    background:setBackgroundColor(0x000000AA)

    local footerBtn = options.footer    or {confirm = false, cancel = true}
    local onCancel  = options.onCancel
    local onConfirm = options.onConfirm

    local dialogContainer = Widget.makeColumnFlow()
    dialogContainer:setSizePolicy(Widget.SizePolicy.FIXED_X + Widget.SizePolicy.COMPRESS_Y)
    dialogContainer:setPositionPolicy(Widget.PositionPolicy.CENTER)
    dialogContainer:setPosition(0, -800)
    dialogContainer:setSize(600,500)

    local header = Widget.makeFixedSize(600, 60)
    header:setBackgroundColor(COLORS.lightBlack)

    if options.title then
        header:addChild(options.title)
    end

    local closeButton = icon:new("remove-circle", 18)
    closeButton.container:setPositionPolicy(Widget.PositionPolicy.RIGHT + Widget.PositionPolicy.CENTER_Y)
    closeButton.container:setMargin(0,10,0,0)
    header:addChild(closeButton.container)
    local body = Widget.makeColumnFlow()
    body:setSizePolicy(Widget.SizePolicy.FIXED_X + Widget.SizePolicy.COMPRESS_Y)
    body:setSize(600,0)
    local lb = loadingBar:new(600, 3, 0, "blue")
    body:setBackgroundColor(0xFFFFFFFF)
    content:setPadding(20)
    body:addChild(lb.container)
    body:addChild(content)

    local footer = Widget.makeFixedSize(600, 40)
    footer:setBackgroundColor(0xD7D7D7FF)

    local clickClose = function(c) closeDialog(o) end

    local btnContainer = Widget.makeLineFlow()
    btnContainer:setPositionPolicy(Widget.PositionPolicy.TOP_RIGHT)

    local o = setmetatable({
        container  = dialogContainer,
        background = background,
        loadingBar = lb,
        onCancel   = onCancel,
        onConfirm  = onConfirm,
    }, self)


    local cancel = function(c, x, y)
        o:cancel()
    end
    background:click(cancel)
    closeButton.container:click(cancel)

    if footerBtn.cancel then
        local btnCancel = button:new(Widget.makeText("Cancel", table.unpack(UiSettings.defaultFont)))
        btnContainer:addChild(btnCancel.container)
        btnCancel.container:click(cancel)
    end

    if footerBtn.confirm then
        local btnText = Widget.makeLineFlow()
        local ww = Widget.makeText("Ok", table.unpack(UiSettings.defaultFont))
        local okIcon = icon:new("ok")
        ww:setTextColor(COLORS.white)
        okIcon.container:setMargin(0,5,0,0)
        okIcon.container:setTextColor(COLORS.white)
        btnText:addChild(okIcon.container)
        btnText:addChild(ww)
        local btnConfirm = button:new(btnText, {color="blue"})
        btnContainer:addChild(btnConfirm.container)

        local confirm = function(c, x, y)
            o:confirm()
        end
        btnConfirm.container:click(confirm)
    end


    footer:addChild(btnContainer)
    dialogContainer:addChild(header)
    dialogContainer:addChild(body)
    dialogContainer:addChild(footer)

    root:addChild(background)
    root:addChild(dialogContainer)

    local maxTimer = 0.15
    local timer = flat.Timer()
    timer:onUpdate(function(timer, elapsedTime)
       local _, rootH = root:getSize()
       dialogContainer:setPosition(0, ((maxTimer - elapsedTime) / maxTimer) * rootH)
    end)
    timer:start(maxTimer)


    return o
end

return Dialog