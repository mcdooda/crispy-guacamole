local Checkbox = require 'data/scripts/ui/checkbox'

return function(addContainer, makeSeparator, font)
	local fogDebugContainer = addContainer 'Fog'

    do
        local enableFogCheckbox = Checkbox:new(false, 'Enable Fog')
        enableFogCheckbox:onValueChanged(function(_, value)
            if value then
                Map.setFogType(Map.FogType.HARD)
            else
                Map.setFogType(Map.FogType.NONE)
            end
        end)
        fogDebugContainer:addChild(enableFogCheckbox.container)
    end
end