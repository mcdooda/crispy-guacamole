local Dialog = require 'mods/dragons-lair/ui/entity/dialog'

local ui = setmetatable({}, { __index = Dialog })

function ui.addedToMap(entity, widget)
    Dialog.addedToMap(entity, widget)
end

return ui