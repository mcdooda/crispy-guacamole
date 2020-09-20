local ScriptNode = flat.require 'graph/script/scriptnode'
local PinTypes = flat.require 'graph/pintypes'

local EachEntityInRangeNode = ScriptNode:inherit 'Each Entity In Range'

function EachEntityInRangeNode:buildPins()
    self.impulseInPin = self:addInputPin(PinTypes.IMPULSE, 'In')
    self.entityInPin = self:addInputPin(flat.types['CG.Entity'], 'Entity')
    self.rangeInPin = self:addInputPin(flat.types.NUMBER, 'Range')

    self.impulseOutPin = self:addOutputPin(PinTypes.IMPULSE, 'Out')
    self.loopOutPin = self:addOutputPin(PinTypes.IMPULSE, 'Loop')
    self.entityOutPin = self:addOutputPin(flat.types['CG.Entity'], 'Entity')
end

function EachEntityInRangeNode:execute(runtime)
    local entity = runtime:readPin(self.entityInPin)
    local range = runtime:readPin(self.rangeInPin)

    local entitiesInRange = Map.getEntitiesInRange(entity:getPosition():toVector2(), range)
    for i = 1, #entitiesInRange do
        local entityInRange = entitiesInRange[i]
        runtime:writePin(self.entityOutPin, entityInRange)
        runtime:impulse(self.loopOutPin)
    end

    runtime:impulse(self.impulseOutPin)
end

return EachEntityInRangeNode