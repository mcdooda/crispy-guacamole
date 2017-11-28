local FunctionalScriptNode = flat.require 'graph/script/functionalscriptnode'

local DetectionComponentNode = FunctionalScriptNode:inherit 'Detection Component'

function DetectionComponentNode:buildPins()
    self.visionRangeInPin = self:addInputPin(flat.types.NUMBER, 'Vision range')

    self.componentOutPin = self:addOutputPin(flat.types.TABLE, 'Component')
end

function DetectionComponentNode:execute(runtime)
    local visionRange = runtime:readPin(self.visionRangeInPin)

    local component = {
        visionRange = visionRange
    }

    runtime:writePin(self.componentOutPin, component)
end

return DetectionComponentNode