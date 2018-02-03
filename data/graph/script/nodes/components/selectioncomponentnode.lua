local FunctionalScriptNode = flat.require 'graph/script/functionalscriptnode'
local PinTypes = flat.require 'graph/pintypes'

local SelectionComponentNode = FunctionalScriptNode:inherit 'Selection Component'

function SelectionComponentNode:buildPins()
    self.canBeSelectionInPin = self:addInputPin(flat.types.BOOLEAN, 'Can Be Selected')

    self.componentOutPin = self:addOutputPin(flat.types.TABLE, 'Component')
end

function SelectionComponentNode:execute(runtime)
    local canBeSelected = runtime:readPin(self.canBeSelectionInPin)

    local component = {
        canBeSelected = canBeSelected
    }

    runtime:writePin(self.componentOutPin, component)
end

return SelectionComponentNode