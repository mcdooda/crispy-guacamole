local FunctionalScriptNode = flat.require 'graph/script/functionalscriptnode'
local PinTypes = flat.require 'graph/pintypes'

local BehaviorComponentNode = FunctionalScriptNode:inherit 'Behavior Component'

function BehaviorComponentNode:buildPins()
    self.componentOutPin = self:addOutputPin(flat.types.TABLE, 'Component')
    self.onInitOutPin = self:addOutputPin(PinTypes.IMPULSE, 'Init')
    self.entityOutPin = self:addOutputPin(flat.types['CG.Entity'], 'Entity')
end

function BehaviorComponentNode:execute(runtime)
    local component = {
        init = function(_, entity)
            runtime:writePin(self.entityOutPin, entity)
            runtime:impulse(self.onInitOutPin)
        end
    }

    runtime:writePin(self.componentOutPin, component)
end

return BehaviorComponentNode