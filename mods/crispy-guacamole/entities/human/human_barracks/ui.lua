local HealthBar = require 'mods/crispy-guacamole/ui/entity/healthbar_long'
local ProgressBar = require 'mods/crispy-guacamole/ui/entity/progressbar_long'
local BuildingAction = require 'mods/crispy-guacamole/ui/building/buildingaction'

local EntityData = require 'mods/crispy-guacamole/scripts/entitydata'

local ui = setmetatable({}, { __index = HealthBar })

function ui.addedToMap(entity, widget)
    HealthBar.addedToMap(entity, widget, 34)
    ProgressBar.addedToMap(entity, widget, 80)

    local container = Widget.makeFixedSize(400, 500)
    local width, height = container:getSize()

    local buildingData = EntityData.get(entity:getTemplatePath())
    local buildingAction = BuildingAction:new(container, entity, buildingData)

	entity:setUiOffset(flat.Vector2(-width / 2, -height / 2 + 50))
    widget:addChild(container)

    entity:selected(function(entity)
        entity:setUiVisible(true)
        buildingAction:animateWidgets()
	end)

	entity:deselected(function(entity)
        entity:setUiVisible(false)
    end)
    entity:setUiVisible(false)
end

return ui