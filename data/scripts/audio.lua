local UiSettings = require 'data/scripts/ui/uisettings'

return function(addContainer, makeSeparator, font)
	local audioContainer = addContainer 'Audio'
    local buttonsContainer = Widget.makeLineFlow()
    do
        local audioInput = Widget.makeNumberInput(table.unpack(font))
        audioInput:setValue(0)
        audioInput:setPrecision(2)
        audioInput:setMargin(2)
        audioInput:setSize(40, 15)
        audioInput:setTextColor(0x000000FF)
        audioInput:setSizePolicy(Widget.SizePolicy.FIXED_X + Widget.SizePolicy.FIXED_Y)
        audioInput:setMin(-math.pi)
        audioInput:setMax(math.pi)

        audioInput:valueChanged(function(widget, value)
            game.debug_setVolume(value)
        end)

        buttonsContainer:addChild(audioInput)
    end

    audioContainer:addChild(buttonsContainer)
end