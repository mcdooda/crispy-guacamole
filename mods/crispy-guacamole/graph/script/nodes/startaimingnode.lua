local ScriptNode = flat.require 'graph/script/scriptnode'
local PinTypes = flat.require 'graph/pintypes'

local cg = require (Mod.getFilePath 'scripts/cg')
local EntityData = require (Mod.getFilePath 'scripts/entitydata')

local StartAimingNode = ScriptNode:inherit 'Start Aiming'

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

    local entityTemplateForButton = cg.entityNamePerButton[buttonName]
    local entityTemplateAssetForButton = assert(Asset.findFromName('entity', entityTemplateForButton), 'Could not find entity asset ' .. entityTemplateForButton)
    local entityData = EntityData.get(entityTemplateAssetForButton:getPath())
    local aimMode = entityData.aimMode

    local aimingEntities = {}
    local groupEntities = playerEntity:getExtraData().groupEntities
    for i = 1, #groupEntities do
        local groupEntity = groupEntities[i]
        if groupEntity:isValid() and groupEntity:getTemplateName() == entityTemplateForButton then
            aimingEntities[#aimingEntities + 1] = groupEntity
        end
    end

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