local UiSettings = require 'data/scripts/ui/uisettings'

local defaultMod = 'mods/crispy-guacamole'
local maps = require(defaultMod .. '/maps/maps')

local root = Widget.getRoot()
do
    local box = Widget.makeColumnFlow()
    box:setBackgroundColor(0x666666FF)
    box:setSizePolicy(Widget.SizePolicy.FIXED_X + Widget.SizePolicy.COMPRESS_Y)
    box:setSize(160, 0)
    box:setPositionPolicy(Widget.PositionPolicy.CENTER)
    for i = 1, #maps do
        local mapName = maps[i]
        local mapLine = Widget.makeLineFlow()
        mapLine:setSizePolicy(Widget.SizePolicy.EXPAND_X + Widget.SizePolicy.COMPRESS_Y)

        local mapLabel = Widget.makeText(mapName, table.unpack(UiSettings.titleFont))
        mapLabel:setMargin(5)
        mapLine:addChild(mapLabel)

        local function click()
            Game.openMap(defaultMod, mapName)
            return true
        end
        local function mouseEnter()
            mapLabel:setTextColor(0x000000FF)
        end
        local function mouseLeave()
            mapLabel:setTextColor(0xFFFFFFFF)
        end

        mapLine:click(click)
        mapLine:mouseEnter(mouseEnter)
        mapLabel:mouseEnter(mouseEnter)
        mapLine:mouseLeave(mouseLeave)
        mapLabel:mouseLeave(mouseLeave)

        box:addChild(mapLine)
    end
    root:addChild(box)
end