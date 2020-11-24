local PinTypes = flat.require 'graph/pintypes'
local AimingNodeBase = require(Mod.getFilePath 'graph/script/nodes/aimingnodebase')

local StartAimingNode = AimingNodeBase:inherit 'Start Aiming'

function StartAimingNode:buildPins()
    self.impulseInPin = self:addInputPin(PinTypes.IMPULSE, 'In')
    self.playerEntityInPin = self:addInputPin(flat.types['CG.Entity'], 'Player Entity')
    self.buttonNameInPin = self:addInputPin(flat.types.STRING, 'Button Name')
    
    self.impulseOutPin = self:addOutputPin(PinTypes.IMPULSE, 'Out')
end

function StartAimingNode:execute(runtime, inputPin)
    assert(inputPin == self.impulseInPin)
    local playerEntity = runtime:readPin(self.playerEntityInPin)
    local buttonName = runtime:readPin(self.buttonNameInPin)

    local aimingEntities, aimMode = self:getAimingEntities(playerEntity, buttonName)

    local extraData = playerEntity:getExtraData()

    local aimInitialPosition = aimMode.computeInitialAimPosition(playerEntity, aimingEntities)
    local aimPositionKey = 'aimPosition' .. buttonName
    extraData[aimPositionKey] = aimInitialPosition

    local aimEntitiesKey = 'aimEntities' .. buttonName
    local aimEntities = {}

    local aimPositions = aimMode.computeAimPositions(playerEntity, aimInitialPosition, aimingEntities)
    for i = 1, #aimPositions do
        aimEntities[#aimEntities + 1] = Entity.spawn('player_aim_small', aimPositions[i])
    end
    extraData[aimEntitiesKey] = aimEntities

    runtime:impulse(self.impulseOutPin)
end

return StartAimingNode