local ScriptNode = flat.require 'graph/script/scriptnode'
local PinTypes = flat.require 'graph/pintypes'

local StopAimingNode = ScriptNode:inherit 'Stop Aiming'

function StopAimingNode:buildPins()
    self.impulseInPin = self:addInputPin(PinTypes.IMPULSE, 'In')
    self.playerEntityInPin = self:addInputPin(flat.types['CG.Entity'], 'Player Entity')
    self.buttonNameInPin = self:addInputPin(flat.types.STRING, 'Button Name')
    
    self.impulseOutPin = self:addOutputPin(PinTypes.IMPULSE, 'Out')
    self.mainAimingPositionOutPin = self:addOutputPin(flat.types['flat.Vector3'], 'Main Aiming Position')
end

function StopAimingNode:execute(runtime, inputPin)
    assert(inputPin == self.impulseInPin)
    local playerEntity = runtime:readPin(self.playerEntityInPin)
    local buttonName = runtime:readPin(self.buttonNameInPin)
    
    local extraData = playerEntity:getExtraData()

    local aimPositionKey = 'aimPosition' .. buttonName
    local mainAimingPosition = extraData[aimPositionKey]
    extraData[aimPositionKey] = nil

    local aimEntitiesKey = 'aimEntities' .. buttonName
    local aimEntities = extraData[aimEntitiesKey]
    for i = 1, #aimEntities do
        aimEntities[i]:despawn()
    end
    extraData[aimEntitiesKey] = nil

    runtime:writePin(self.mainAimingPositionOutPin, mainAimingPosition)
    runtime:impulse(self.impulseOutPin)
end

return StopAimingNode