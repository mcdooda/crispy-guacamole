local healthBarWidgetWidth = 40
local healthBarWidgetHeight = 6

local bgpicto = 'mods/crispy-guacamole/ui/entity/healthbarbg.png'
local fgpicto = 'mods/crispy-guacamole/ui/entity/healthbarfg.png'

local Healthbar = require 'mods/crispy-guacamole/ui/entity/healthbar'

return {
    addedToMap = function(entity, widget, offsetY)
        Healthbar.addHealthBarToMap(entity, widget, offsetY, bgpicto, fgpicto, healthBarWidgetWidth, healthBarWidgetHeight)
    end,

    removedFromMap = function(entity, widget)
        widget:removeFromParent()
    end
}
