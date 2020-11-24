local ScriptNode = flat.require 'graph/script/scriptnode'
local PinTypes = flat.require 'graph/pintypes'

local cg = require(Mod.getFilePath 'scripts/cg')
local EntityData = require(Mod.getFilePath 'scripts/entitydata')

local AimingNodeBase = ScriptNode:inherit()

function AimingNodeBase:getAimingEntities(playerEntity, buttonName)
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

    return aimingEntities, aimMode
end

return AimingNodeBase