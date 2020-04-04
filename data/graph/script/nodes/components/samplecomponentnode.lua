local FunctionalScriptNode = flat.require 'graph/script/functionalscriptnode'

local SampleComponentNode = FunctionalScriptNode:inherit 'Sample Component'

function SampleComponentNode:buildPins()
    self.componentOutPin = self:addOutputPin(flat.types.TABLE, 'Component')
end

function SampleComponentNode:execute(runtime)
    local component = {}

    runtime:writePin(self.componentOutPin, component)
end

return SampleComponentNode