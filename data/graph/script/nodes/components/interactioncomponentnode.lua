local FunctionalScriptNode = flat.require 'graph/script/functionalscriptnode'

local InteractionComponentNode = FunctionalScriptNode:inherit 'Interaction Component'

function InteractionComponentNode:buildPins()
    self.behaviorStateNameInPin = self:addInputPin(flat.types.STRING, 'Behavior State')

    self.componentOutPin = self:addOutputPin(flat.types.TABLE, 'Component')
end

function InteractionComponentNode:execute(runtime)
    local behaviorStateName = runtime:readPin(self.behaviorStateNameInPin)

    local component = {
        behaviorStateName = behaviorStateName
    }

    runtime:writePin(self.componentOutPin, component)
end

return InteractionComponentNode