local FunctionalScriptNode = flat.require 'graph/script/functionalscriptnode'

local PropComponentNode = FunctionalScriptNode:inherit 'Prop Component'

function PropComponentNode:buildPins()
    self.componentOutPin = self:addOutputPin(flat.types.TABLE, 'Component')
end

function PropComponentNode:execute(runtime)
    local component = {}

    runtime:writePin(self.componentOutPin, component)
end

return PropComponentNode