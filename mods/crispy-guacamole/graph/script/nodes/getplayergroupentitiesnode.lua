local ScriptNode = flat.require 'graph/script/scriptnode'
local PinTypes = flat.require 'graph/pintypes'

local GetPlayerGroupEntitiesNode = ScriptNode:inherit 'Get Player Group Entities'

function GetPlayerGroupEntitiesNode:buildPins()
    self.impulseInPin = self:addInputPin(PinTypes.IMPULSE, 'In')
    self.playerEntityInPin = self:addInputPin(flat.types['CG.Entity'], 'Player Entity')
    
    self.impulseOutPin = self:addOutputPin(PinTypes.IMPULSE, 'Out')
    self.groupEntitiesOutPin = self:addOutputPin(flat.types.TABLE, 'Group Entities')
end

function GetPlayerGroupEntitiesNode:execute(runtime, inputPin)
    assert(inputPin == self.impulseInPin)
    local playerEntity = runtime:readPin(self.playerEntityInPin)
    
    local groupEntities = assert(playerEntity:getExtraData().groupEntities, tostring(playerEntity) .. ' has no group entities?')

    -- remove invalid group entities, could be done somewhere else maybe?
    flat.arrayRemoveIf(groupEntities, function(value)
        return not value:isValid() or value:isDespawnPending()
    end)

    runtime:writePin(self.groupEntitiesOutPin, groupEntities)
    runtime:impulse(self.impulseOutPin)
end

return GetPlayerGroupEntitiesNode