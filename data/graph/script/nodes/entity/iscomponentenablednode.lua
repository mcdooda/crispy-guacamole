local FunctionalScriptNode = flat.require 'graph/script/functionalscriptnode'
local PinTypes = flat.require 'graph/pintypes'

local IsComponentEnabledNode = FunctionalScriptNode:inherit 'Is Component Enabled'

function IsComponentEnabledNode:buildPins()
    self.entityInPin = self:addInputPin(flat.types['CG.Entity'], 'Entity')
    self.componentNameInPin = self:addInputPin(flat.types.STRING, 'Component Name')

    self.isEnabledOutPin = self:addOutputPin(flat.types.BOOLEAN, 'Is Enabled')
end

function IsComponentEnabledNode:execute(runtime)
    local entity = runtime:readPin(self.entityInPin)
    local componentName = runtime:readPin(self.componentNameInPin)

    local isEnabled = entity:isComponentEnabled(Component[componentName])

    runtime:writePin(self.isEnabledOutPin, isEnabled)
end

return IsComponentEnabledNode