local FunctionalScriptNode = flat.require 'graph/script/functionalscriptnode'

local PropComponentNode = FunctionalScriptNode:inherit 'Prop Component'

function PropComponentNode:buildPins()
    self.sizeInPin = self:addInputPin(flat.types['flat.Vector2'], 'Size')

    self.componentOutPin = self:addOutputPin(flat.types.TABLE, 'Component')
end

function PropComponentNode:execute(runtime)
    local size = runtime:readPin(self.sizeInPin)

    local component = {
        size = size
    }

    runtime:writePin(self.componentOutPin, component)
end

return PropComponentNode