local UiSettings = require 'data/scripts/ui/uisettings'

local TextInput = {}
TextInput.__index = TextInput

function TextInput:new(value)
    local textInputWidget = Widget.makeTextInput(table.unpack(UiSettings.defaultFont))
    textInputWidget:setText(value)
    textInputWidget:setMargin(2, 4, 2, 4)

    local container = Widget.makeLineFlow()
    container:setBackgroundColor(0x848484FF)

    do
        local background = Widget.makeLineFlow()
        background:setBackgroundColor(0x444444FF)
        background:setMargin(1)
        container:addChild(background)

        do
            background:addChild(textInputWidget)
        end
    end


    local o = setmetatable({
        textInputWidget = textInputWidget,
        container = container
    }, self)

    return o
end

function TextInput:setValue(value)
     self.textInputWidget:getText(value)
end

function TextInput:getValue()
    return self.textInputWidget:getText()
end

return TextInput