local HealthBar = require 'mods/crispy-guacamole/ui/entity/healthbar_short'
local ProgressBar = require 'mods/crispy-guacamole/ui/entity/progressbar_short'

local ui = setmetatable({}, { __index = HealthBar })

function ui.addedToMap(entity, widget)
    HealthBar.addedToMap(entity, widget, 34)
    ProgressBar.addedToMap(entity, widget, 0)
end

return ui