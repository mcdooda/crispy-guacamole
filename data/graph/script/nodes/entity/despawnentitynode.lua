local ScriptNode = flat.require 'graph/script/scriptnode'
local PinTypes = flat.require 'graph/pintypes'

local DespawnEntityNode = ScriptNode:inherit 'Despawn Entity'

function DespawnEntityNode:buildPins()
    self.impulseInPin = self:addInputPin(PinTypes.IMPULSE, 'In')
    self.entityInPin = self:addInputPin(flat.types['CG.Entity'], 'Entity')

    self.impulseOutPin = self:addOutputPin(PinTypes.IMPULSE, 'Out')
end

function DespawnEntityNode:execute(runtime, inputPin)
    assert(inputPin == self.impulseInPin)
    local entity = runtime:readPin(self.entityInPin)

    entity:despawn()

    runtime:impulse(self.impulseOutPin)
end

return DespawnEntityNode