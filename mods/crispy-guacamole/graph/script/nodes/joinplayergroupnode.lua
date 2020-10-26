local ScriptNode = flat.require 'graph/script/scriptnode'
local PinTypes = flat.require 'graph/pintypes'

local JoinPlayerGroupNode = ScriptNode:inherit 'Join Player Group'

function JoinPlayerGroupNode:buildPins()
    self.impulseInPin = self:addInputPin(PinTypes.IMPULSE, 'In')
    self.playerEntityInPin = self:addInputPin(flat.types['CG.Entity'], 'Player Entity')
    self.entityInPin = self:addInputPin(flat.types['CG.Entity'], 'Entity')

    self.impulseOutPin = self:addOutputPin(PinTypes.IMPULSE, 'Out')
end

function JoinPlayerGroupNode:execute(runtime, inputPin)
    assert(inputPin == self.impulseInPin)
    local playerEntity = runtime:readPin(self.playerEntityInPin)
    local entity = runtime:readPin(self.entityInPin)

    local groupEntities = playerEntity:getExtraData().groupEntities
    groupEntities[#groupEntities + 1] = entity

    entity:getExtraData().playerToFollow = playerEntity
    entity:enterState('followPlayer', true)
    
    runtime:impulse(self.impulseOutPin)
end

return JoinPlayerGroupNode