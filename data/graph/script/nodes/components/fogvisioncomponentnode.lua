local FunctionalScriptNode = flat.require 'graph/script/functionalscriptnode'

local FogVisionComponentNode = FunctionalScriptNode:inherit 'Fog Vision Component'

function FogVisionComponentNode:buildPins()
    self.componentOutPin = self:addOutputPin(flat.types.TABLE, 'Component')
end

function FogVisionComponentNode:execute(runtime)
    local component = {}

    runtime:writePin(self.componentOutPin, component)
end

return FogVisionComponentNode