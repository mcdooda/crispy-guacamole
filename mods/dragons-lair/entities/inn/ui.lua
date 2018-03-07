local Dialog = require 'mods/dragons-lair/ui/entity/dialog'

local ui = setmetatable({}, { __index = Dialog })

function ui.addedToMap(entity, widget)
	Dialog.addedToMap(entity, widget)
	entity:selected(function(entity)
		entity:setUiVisible(true)
	end)
	entity:deselected(function(entity) 			
		entity:setUiVisible(false)
	end)
end

return ui