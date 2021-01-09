local PinTypes = flat.require 'graph/pintypes'
local AimingNodeBase = require(Mod.getFilePath 'graph/script/nodes/aimingnodebase')

local StopAimingNode = AimingNodeBase:inherit 'Stop Aiming'

function StopAimingNode:buildPins()
    self.impulseInPin = self:addInputPin(PinTypes.IMPULSE, 'In')
    self.playerEntityInPin = self:addInputPin(flat.types['CG.Entity'], 'Player Entity')
    self.buttonNameInPin = self:addInputPin(flat.types.STRING, 'Button Name')
    
    self.impulseOutPin = self:addOutputPin(PinTypes.IMPULSE, 'Out')
end

function StopAimingNode:execute(runtime, inputPin)
    assert(inputPin == self.impulseInPin)
    local playerEntity = runtime:readPin(self.playerEntityInPin)
    local buttonName = runtime:readPin(self.buttonNameInPin)

    local aimingEntities, aimMode = self:getAimingEntities(playerEntity, buttonName)
    
    local extraData = playerEntity:getExtraData()

    local aimPositionKey = 'aimPosition' .. buttonName
    local mainAimingPosition = extraData[aimPositionKey]
    extraData[aimPositionKey] = nil

    local aimPositions = {}

    local aimEntitiesKey = 'aimEntities' .. buttonName
    local aimEntities = extraData[aimEntitiesKey]
    for i = 1, #aimEntities do
        aimPositions[i] = aimEntities[i]:getPosition()
        aimEntities[i]:despawn()
    end
    extraData[aimEntitiesKey] = nil

    -- all entities enter combat, not only the currently aiming ones
    if aimMode.triggersCombat then
        print 'Triggering combat'
        local groupEntities = extraData.groupEntities
        for i = 1, #groupEntities do
            local groupEntity = groupEntities[i]
            if groupEntity:isValid() and groupEntity:getExtraData().currentLoopingState ~= 'combat' then
                groupEntity:enterState 'combat'
            end
        end
    else
        print 'Not Triggering combat'
    end

    aimMode.useAbility(aimingEntities, aimPositions)

    runtime:impulse(self.impulseOutPin)
end

return StopAimingNode