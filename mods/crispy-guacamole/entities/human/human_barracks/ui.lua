local HealthBar = require 'mods/crispy-guacamole/ui/entity/healthbar_long'
local ProgressBar = require 'mods/crispy-guacamole/ui/entity/progressbar_long'
local BuildingAction = require 'mods/crispy-guacamole/ui/building/building_action'
local EntityData = require 'mods/crispy-guacamole/scripts/entitydata'

local ui = setmetatable({}, { __index = HealthBar })

local unitData = {
    {
        template = Mod.getFilePath('entities/human/human_soldier'),
        type = 'sword',
        price = 10
    },
    {
        template = Mod.getFilePath('entities/human/human_archer'),
        type = 'bow',
        price = 20
    },
    {
        template = Mod.getFilePath('entities/human/human_crossbowman'),
        type = 'bow',
        price = 70
    },
    {
        template = Mod.getFilePath('entities/human/human_paladin'),
        type = 'wooden_shield',
        price = 120
    }
}

function ui.addedToMap(entity, widget)
    HealthBar.addedToMap(entity, widget, 34)
    ProgressBar.addedToMap(entity, widget, 80)

    local container = Widget.makeFixedSize(400, 500)
    local width, height = container:getSize()

    local buildingData = EntityData.get(entity:getTemplatePath())
    flat.dump(buildingData)
    BuildingAction.buildWidgets(container, entity, buildingData)

	entity:setUiOffset(flat.Vector2(-width / 2, -height / 2 + 50))
    widget:addChild(container)

    entity:selected(function(entity)
        entity:setUiVisible(true)
        entity:setSpriteColor(0x0000FFFF)
	end)

	entity:deselected(function(entity)
        entity:setUiVisible(false)
        entity:clearSpriteColor()
    end)
    entity:setUiVisible(false)
end

return ui