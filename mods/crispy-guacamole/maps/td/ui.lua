local Theme = require 'mods/crispy-guacamole/maps/td/theme'
local BuildingIcon = require 'mods/crispy-guacamole/maps/td/buildingicon'

local root = Widget.getRoot()

-- right panel
local function makeRightPanelContainer(title)
    local container = Widget.makeColumnFlow()
    container:setSizePolicy(Widget.SizePolicy.EXPAND_X + Widget.SizePolicy.COMPRESS_Y)
    container:setPadding(5)
    container:setMargin(0, 0, 5, 0)
    container:setBackgroundColor(Theme.BACKGROUND_COLOR)

    do
        local label = Widget.makeText(title, table.unpack(Theme.defaultFont))
        label:setTextColor(Theme.TEXT_COLOR)
        container:addChild(label)
    end

    return container
end

do
    local rightPanel = Widget.makeColumnFlow()
    rightPanel:setPositionPolicy(Widget.PositionPolicy.RIGHT + Widget.PositionPolicy.CENTER_Y)
    rightPanel:setSizePolicy(Widget.SizePolicy.FIXED_X + Widget.SizePolicy.COMPRESS_Y)
    rightPanel:setSize(70, 0)

    -- buildings
    do
        local buildingsContainer = makeRightPanelContainer 'Buildings'

        do
            BuildingIcon:new('tower', buildingsContainer)
            BuildingIcon:new('sleeping_dragon', buildingsContainer)
        end

        rightPanel:addChild(buildingsContainer)
    end

    -- money
    do
        local moneyContainer = makeRightPanelContainer 'Money'

        do
            local moneyAmountLabel = Widget.makeText('100 gold', table.unpack(Theme.defaultFont))
            moneyAmountLabel:setTextColor(Theme.TEXT_COLOR)
            moneyContainer:addChild(moneyAmountLabel)
        end

        rightPanel:addChild(moneyContainer)
    end

    root:addChild(rightPanel)
end

