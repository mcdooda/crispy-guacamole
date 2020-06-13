local HealthBar = require 'mods/crispy-guacamole/ui/entity/healthbar_long'
local ProgressBar = require 'mods/crispy-guacamole/ui/entity/progressbar_long'

local ui = setmetatable({}, { __index = HealthBar })

function ui.addedToMap(entity, widget)
    HealthBar.addedToMap(entity, widget, 34)
    ProgressBar.addedToMap(entity, widget, 80)
end

return ui