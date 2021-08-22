local ScriptNode = flat.require 'graph/script/scriptnode'
local PinTypes = flat.require 'graph/pintypes'
local BasicUnitBehavior = require(Mod.getFilePath 'scripts/basicunitbehavior').basicUnit()

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

    if entity:implementsBehavior(BasicUnitBehavior) then
        assert(playerEntity ~= entity)

        local groupEntities = playerEntity:getExtraData().groupEntities
        local addedToGroup = flat.arrayAddUnique(groupEntities, entity)

        entity:getExtraData().playerToFollow = playerEntity
        entity:enterState('followPlayer', true)

        if addedToGroup then
            playerEntity:getExtraData().entityJoinedGroup(entity)
        end
    end

    runtime:impulse(self.impulseOutPin)
end

return JoinPlayerGroupNode