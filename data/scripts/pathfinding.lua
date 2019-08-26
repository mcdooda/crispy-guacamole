local Checkbox = require 'data/scripts/ui/checkbox'

return function(addContainer, makeSeparator, font)
	local navigabilityDebugContainer = addContainer 'Pathfinding'

    do
        local navigabilityCheckbox = Checkbox:new(false, "Navigability")
        navigabilityCheckbox:onValueChanged(function(_, value)
            Map.debug_enableNavigabilityDebug(value)
        end)
        navigabilityDebugContainer:addChild(navigabilityCheckbox.container)
    end

    do
        local tileIndicesCheckbox = Checkbox:new(false, "Tile Indices")
        tileIndicesCheckbox:onValueChanged(function(_, value)
            Map.debug_enableTileIndicesDebug(value)
        end)
        navigabilityDebugContainer:addChild(tileIndicesCheckbox.container)
    end

    do
        local simplifyPathsCheckbox = Checkbox:new(true, "Simplify paths")
        simplifyPathsCheckbox:onValueChanged(function(_, value)
            Map.debug_enableSimplifyPath(value)
        end)
        navigabilityDebugContainer:addChild(simplifyPathsCheckbox.container)
    end

    navigabilityDebugContainer:addChild(makeSeparator())
end