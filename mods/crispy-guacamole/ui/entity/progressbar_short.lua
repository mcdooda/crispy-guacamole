

local buildBarWidgetWidth = 40
local buildBarWidgetHeight = 6

local bgpicto = 'mods/crispy-guacamole/ui/entity/progressbarbg_short.png'
local fgpicto = 'mods/crispy-guacamole/ui/entity/progressbarfg_short.png'

local ProgressBar = require 'mods/crispy-guacamole/ui/entity/progressbar'

return {
    addedToMap = function(entity, widget, offsetY)
        ProgressBar.addProgressBarToMap(entity, widget, offsetY, bgpicto, fgpicto, buildBarWidgetWidth, buildBarWidgetHeight)
    end,

    removedFromMap = function(entity, widget)
        widget:removeFromParent()
    end
}
