local ScriptNode = flat.require 'graph/script/scriptnode'
local PinTypes = flat.require 'graph/pintypes'

local SpawnEntityNode = ScriptNode:inherit 'Spawn Entity'

function SpawnEntityNode:buildPins()
    self.impulseInPin = self:addInputPin(PinTypes.IMPULSE, 'In')
    self.entityNameInPin = self:addInputPin(flat.types.STRING, 'Entity Name')
    self.positionInPin = self:addInputPin(flat.types['flat.Vector3'], 'Position')
    self.headingInPin = self:addInputPin(flat.types.NUMBER, 'Heading')
    self.elevationInPin = self:addInputPin(flat.types.NUMBER, 'Elevation')
    self.instigatorInPin = self:addInputPin(flat.types['CG.Entity'], 'Instigator')

    self.impulseOutPin = self:addOutputPin(PinTypes.IMPULSE, 'Out')
    self.entityOutPin = self:addOutputPin(flat.types['CG.Entity'], 'Entity')
end

function SpawnEntityNode:execute(runtime, inputPin)
    assert(inputPin == self.impulseInPin)
    local entityName = runtime:readPin(self.entityNameInPin)
    local position = runtime:readPin(self.positionInPin)
    local heading = runtime:readPin(self.headingInPin)
    local elevation = runtime:readPin(self.elevationInPin)
    local instigator = runtime:readOptionalPin(self.instigatorInPin)

    local entity = Entity.spawn(entityName, position, heading, elevation, instigator)

    runtime:writePin(self.entityOutPin, entity)
    runtime:impulse(self.impulseOutPin)
end

return SpawnEntityNode