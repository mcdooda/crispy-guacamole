
local font = { 'data/misc/fonts/LucidaSansRegular.ttf', 12 }

local Radiobutton = {}
Radiobutton.__index = Radiobutton

local function triggerValueChanged(radiobutton, value)
    local callbacks = radiobutton.valueChangedCallbacks
    for i = 1, #callbacks do
        callbacks[i](radiobutton, value)
    end
end

local function containerClicked(radiobutton)
    radiobutton.value = not radiobutton.value

    radiobutton.checkMark:setVisible(radiobutton.value)
    print(radiobutton.value)
    triggerValueChanged(radiobutton, value)
    return true
end

function Radiobutton:new(value, width, text, path)
    value = value or 0
    width = width or 20
    height = width
    path = path or 'data/editor/interface/radiobutton/'
    assert(width > 0)

    local checkMarkContainer = Widget.makeFixedSize(width, height)
    checkMarkContainer:setBackground(path .. 'background.png')

    local checkMark = Widget.makeFixedSize(width, height)
    checkMark:setBackground(path .. 'foreground.png')
    checkMark:setPosition(0, 0)
    checkMark:setVisible(value)
    checkMarkContainer:addChild(checkMark)

    local container = Widget.makeLineFlow()
    container:setSizePolicy(Widget.SizePolicy.COMPRESS_X + Widget.SizePolicy.COMPRESS_Y)
    container:addChild(checkMarkContainer)
    container:setBackgroundColor(0xFFFFFFFF)

    if (text) then
        local label = Widget.makeText(text, unpack(font))
        label:setMargin(0, 0, 0, 7)
        container:addChild(label)
    end

    local o = setmetatable({
        container             = container,
        checkMark             = checkMark,
        value                 = value,
        valueChangedCallbacks = {}
    }, self)

    local click = function(c) containerClicked(o) end
    container:click(click)
    -- container:mouseMove(click)

    return o
end

function Radiobutton:getValue()
    return self.value
end

function Radiobutton:setValue(value)
    assert(value == true or value == false)
    self.value = value
    self.checkMark:setVisible(self.value)
    triggerValueChanged(self, value)
end

return Radiobutton