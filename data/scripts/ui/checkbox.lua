

local Checkbox = {}
Checkbox.__index = Checkbox

local function triggerValueChanged(checkbox, value)
    local callbacks = checkbox.valueChangedCallbacks
    for i = 1, #callbacks do
        callbacks[i](checkbox, value)
    end
end

local function containerClicked(checkbox)
    checkbox.value = not checkbox.value

    checkbox.checkMark:setVisible(checkbox.value)
    print(checkbox.value)
    triggerValueChanged(checkbox, value)
    return true
end

function Checkbox:new(value, width, text, path)
    value = value or 0
    width = width or 20
    height = width
    path = path or 'data/editor/interface/checkbox/'
    assert(width > 0)

    local checkMarkContainer = Widget.makeFixedSize(width, height)
    checkMarkContainer:setBackground(path .. 'background.png')

    local checkMark = Widget.makeFixedSize(width - 2, height - 2)
    checkMark:setBackground(path .. 'foreground.png')
    checkMark:setPosition(1, -1)
    checkMark:setVisible(value)
    checkMarkContainer:addChild(checkMark)

    local container = Widget.makeLineFlow()
    container:setSizePolicy(Widget.SizePolicy.COMPRESS_X + Widget.SizePolicy.COMPRESS_Y)
    container:addChild(checkMarkContainer)


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

function Checkbox:getValue()
    return self.value
end

function Checkbox:setValue(value)
    assert(value == true or value == false)
    self.value = value
    self.checkMark:setVisible(self.value)
    triggerValueChanged(self, value)
end

return Checkbox