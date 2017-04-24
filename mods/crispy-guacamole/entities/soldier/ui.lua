local HealthBar = require 'mods/crispy-guacamole/ui/entity/healthbar'

local ui = setmetatable({}, { __index = HealthBar })

function ui.update(entity, widget)
    HealthBar.update(entity, widget, 34)
end

return ui