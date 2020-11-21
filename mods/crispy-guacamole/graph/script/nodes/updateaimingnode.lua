local ScriptNode = flat.require 'graph/script/scriptnode'
local PinTypes = flat.require 'graph/pintypes'

local cg = require (Mod.getFilePath 'scripts/cg')
local EntityData = require (Mod.getFilePath 'scripts/entitydata')

local UpdateAimingNode = ScriptNode:inherit 'Update Aiming'

function UpdateAimingNode:buildPins()
    self.impulseInPin = self:addInputPin(PinTypes.IMPULSE, 'In')
    self.playerEntityInPin = self:addInputPin(flat.types['CG.Entity'], 'Player Entity')
    self.buttonNameInPin = self:addInputPin(flat.types.STRING, 'Button Name')
    
    self.impulseOutPin = self:addOutputPin(PinTypes.IMPULSE, 'Out')
end

local function rotate(x, y, a)
    local s = math.sin(a)
    local c = math.cos(a)
    return (c * x) - (s * y), (s * x) + (c * y)
end

local function getStickDirection(entity)
    local gamepadIndex = entity:getGamepadIndex()
    local rawX, rawY = Gamepads.getLeftStickValue(gamepadIndex)
    return flat.Vector2(rotate(-rawX, rawY, -math.pi / 4))
end

function UpdateAimingNode:execute(runtime, inputPin)
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

    local aimPositionKey = 'aimPosition' .. buttonName
    local mainAimingPosition = extraData[aimPositionKey]

    local stickDirection = getStickDirection(playerEntity)

    mainAimingPosition = mainAimingPosition + flat.Vector3(stickDirection:x(), stickDirection:y(), 0) * game.getDT() * 15
    extraData[aimPositionKey] = mainAimingPosition

    playerEntity:lookAtPosition(mainAimingPosition)
    
    local aimEntitiesKey = 'aimEntities' .. buttonName
    local aimEntities = extraData[aimEntitiesKey]

    local aimPositions = aimMode.computeAimPositions(playerEntity, mainAimingPosition, aimingEntities)
    for i = 1, #aimPositions do
        aimEntities[i]:setPosition2d(aimPositions[i]:toVector2())
    end

    runtime:impulse(self.impulseOutPin)
end

return UpdateAimingNode