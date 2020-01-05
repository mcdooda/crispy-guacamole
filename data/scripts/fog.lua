local Checkbox = require 'data/scripts/ui/checkbox'
local RadioButton = require 'data/scripts/ui/radiobutton'
local RadioGroup = require 'data/scripts/ui/radiogroup'

return function(addContainer, makeSeparator, font)
	local fogDebugContainer = addContainer 'Fog'

    do
        local noFogRadioButton = RadioButton:new(Map.FogType.NONE, 'No fog')
        local hardFogRadioButton = RadioButton:new(Map.FogType.HARD, 'Hard fog')
        local softFogRadioButton = RadioButton:new(Map.FogType.SOFT, 'Soft fog')
        fogDebugContainer:addChild(noFogRadioButton.container)
        fogDebugContainer:addChild(hardFogRadioButton.container)
        fogDebugContainer:addChild(softFogRadioButton.container)

        local fogRadioGroup = RadioGroup:new{ noFogRadioButton, hardFogRadioButton, softFogRadioButton }
        fogRadioGroup:setValue(Map.getFogType())
        fogRadioGroup.valueChanged:on(function(value)
            Map.setFogType(value)
        end)
        
        -- monitor external changes
        -- TODO: plug a callback into the code or something better than this
        flat.ui.task(function()
            while true do
                print(Map.getFogType())
                fogRadioGroup:setValue(Map.getFogType())
                coroutine.yield()
            end
        end)()
    end
end