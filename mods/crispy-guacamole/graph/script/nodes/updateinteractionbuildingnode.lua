local ScriptNode = flat.require 'graph/script/scriptnode'
local PinTypes = flat.require 'graph/pintypes'

local FindInteractionBuildingEQSTemplate = require(Mod.getFilePath 'eqs/findinteractionbuilding')

local UpdateInteractionBuildingNode = ScriptNode:inherit 'Update Interaction Building'

function UpdateInteractionBuildingNode:buildPins()
    self.impulseInPin = self:addInputPin(PinTypes.IMPULSE, 'In')
    self.playerEntityInPin = self:addInputPin(flat.types['CG.Entity'], 'Player Entity')

    self.impulseOutPin = self:addOutputPin(PinTypes.IMPULSE, 'Out')
    self.buildingEntityOutPin = self:addOutputPin(flat.types['CG.Entity'], 'Building Entity')
end

function UpdateInteractionBuildingNode:execute(runtime)
    local playerEntity = runtime:readPin(self.playerEntityInPin)

    local buildingEntityItem = flat.eqs.run(FindInteractionBuildingEQSTemplate, playerEntity, flat.eqs.RunMode.SingleBestItem)
    local buildingEntity
    if buildingEntityItem then
        buildingEntity = buildingEntityItem.item
    end
    assert(not buildingEntity or flat.type(buildingEntity) == flat.types['CG.Entity'])

    local playerExtraData = playerEntity:getExtraData()
    local isNewBuilding = buildingEntity ~= playerExtraData.interactionBuildingEntity

    if playerExtraData.interactionBuildingEntity and isNewBuilding then
        local oldBuilding = playerExtraData.interactionBuildingEntity
        oldBuilding:setSelected(false)
        playerExtraData.interactionBuildingEntity = nil
    end

    if buildingEntity and isNewBuilding then
        playerExtraData.interactionBuildingEntity = buildingEntity
        buildingEntity:setSelected(true)
    end

    runtime:writeOptionalPin(self.buildingEntityOutPin, buildingEntity)
    runtime:impulse(self.impulseOutPin)
end

return UpdateInteractionBuildingNode