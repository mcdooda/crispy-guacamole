local Slider = require 'data/scripts/ui/slider'
local UiSettings = require 'data/scripts/ui/uisettings'

return function(addContainer, makeSeparator, font)
	local audioContainer = addContainer 'Audio'

    do
        local volumeSlider = Slider:new(146, 15, 0, 1, 1)
        volumeSlider:valueChanged(function(widget, value)
            game.debug_setVolume(value)
        end)
        audioContainer:addChild(volumeSlider.container)
    end
end