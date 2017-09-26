local ScriptNode = flat.require 'graph/script/scriptnode'
local PinTypes = flat.require 'graph/pintypes'

local SpawnEntityNode = ScriptNode:inherit 'Spawn Entity'

function SpawnEntityNode:buildPins()
    self.impulseInPin = self:addInputPin(PinTypes.IMPULSE, 'In')
    self.entityNameInPin = self:addInputPin(PinTypes.STRING, 'Entity Name')
    self.positionInPin = self:addInputPin(PinTypes.USERDATA, 'Position')
    self.headingInPin = self:addInputPin(PinTypes.NUMBER, 'Heading')

    self.impulseOutPin = self:addOutputPin(PinTypes.IMPULSE, 'Out')
    self.entityOutPin = self:addOutputPin(PinTypes.USERDATA, 'Entity')
end

function SpawnEntityNode:execute(runtime, inputPin)
    assert(inputPin == self.impulseInPin)
    local entityName = runtime:readPin(self.entityNameInPin)
    local position = runtime:readPin(self.positionInPin)
    local heading = runtime:readPin(self.headingInPin)

    local entity = Entity.spawn(entityName, position, heading)

    runtime:writePin(self.entityOutPin, entity)
    runtime:impulse(self.impulseOutPin)
end

return SpawnEntityNode