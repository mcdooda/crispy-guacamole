local BottomPanel = require 'mods/crispy-guacamole/ui/bottompanel'

local function selected(building)
    BottomPanel.setBuilding(building)
end

local function deselected(building)
    BottomPanel.clearBuilding(building)
end

local function init(building)
    building:selected(selected)
    building:deselected(deselected)
end

return {
    init = init
}