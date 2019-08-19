local BottomPanel = require 'mods/crispy-guacamole/ui/bottompanel'

local function selected(unit)
    BottomPanel.setUnit(unit)
end

local function deselected(unit)
    BottomPanel.clearUnit(unit)
end

local function init(unit)
    unit:selected(selected)
    unit:deselected(deselected)
end

return {
    init = init
}