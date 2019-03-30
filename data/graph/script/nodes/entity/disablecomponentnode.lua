local ScriptNode = flat.require 'graph/script/scriptnode'
local PinTypes = flat.require 'graph/pintypes'

local DisableComponentNode = ScriptNode:inherit 'Disable Component'

function DisableComponentNode:buildPins()
	self.impulseInPin = self:addInputPin(PinTypes.IMPULSE, 'In')
    self.entityInPin = self:addInputPin(flat.types['CG.Entity'], 'Entity')
    self.componentNameInPin = self:addInputPin(flat.types.STRING, 'Component Name')

	self.impulseOutPin = self:addOutputPin(PinTypes.IMPULSE, 'Out')
end

function DisableComponentNode:execute(runtime, inputPin)
	assert(inputPin == self.impulseInPin)
    local entity = runtime:readPin(self.entityInPin)
    local componentName = runtime:readPin(self.componentNameInPin)

    entity:incComponentDisableLevel(Component[componentName])

    runtime:impulse(self.impulseOutPin)
end

return DisableComponentNode