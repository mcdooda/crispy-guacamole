local HealthBar = require 'mods/crispy-guacamole/ui/entity/healthbar'

local ui = setmetatable({}, { __index = HealthBar })

function ui.addedToMap(entity, widget)
    HealthBar.addedToMap(entity, widget, 34)
end

return ui