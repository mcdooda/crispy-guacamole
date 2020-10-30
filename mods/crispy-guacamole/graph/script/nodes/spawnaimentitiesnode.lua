local ScriptNode = flat.require 'graph/script/scriptnode'
local PinTypes = flat.require 'graph/pintypes'

local SpawnAimEntitiesNode = ScriptNode:inherit 'Spawn Aim Entities'

function SpawnAimEntitiesNode:buildPins()
    self.impulseInPin = self:addInputPin(PinTypes.IMPULSE, 'In')
    self.playerEntityInPin = self:addInputPin(flat.types['CG.Entity'], 'Player Entity')
    
    self.impulseOutPin = self:addOutputPin(PinTypes.IMPULSE, 'Out')
    self.groupEntitiesOutPin = self:addOutputPin(flat.types.TABLE, 'Group Entities')
end

function SpawnAimEntitiesNode:execute(runtime, inputPin)
    assert(inputPin == self.impulseInPin)
    local playerEntity = runtime:readPin(self.playerEntityInPin)
    
    local groupEntities = playerEntity:getExtraData().groupEntities

    runtime:writePin(self.groupEntitiesOutPin, groupEntities)
    runtime:impulse(self.impulseOutPin)
end

return SpawnAimEntitiesNode