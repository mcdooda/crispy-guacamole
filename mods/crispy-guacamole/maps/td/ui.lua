local Theme = require 'mods/crispy-guacamole/maps/td/theme'
local BuildingIcon = require 'mods/crispy-guacamole/maps/td/buildingicon'
local TowersData = require 'mods/crispy-guacamole/maps/td/towersdata'
local Money = require 'mods/crispy-guacamole/maps/td/money'
local Score = require 'mods/crispy-guacamole/maps/td/score'

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
        label:setPositionPolicy(Widget.PositionPolicy.CENTER_X + Widget.PositionPolicy.TOP)
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

        for i = 1, #TowersData do
            local towerData = TowersData[i]
            BuildingIcon:new(towerData, buildingsContainer)
        end

        rightPanel:addChild(buildingsContainer)
    end

    -- money
    do
        local moneyContainer = makeRightPanelContainer 'Gold'

        do
            local moneyAmountLabel = Widget.makeText('0', table.unpack(Theme.defaultFont))
            moneyAmountLabel:setPositionPolicy(Widget.PositionPolicy.TOP_RIGHT)
            moneyAmountLabel:setTextColor(Theme.TEXT_COLOR)
            moneyContainer:addChild(moneyAmountLabel)

            Money:onAmountChanged(function()
                moneyAmountLabel:setText(Money:getAmount())
            end)
        end

        rightPanel:addChild(moneyContainer)
    end

    -- score
    do
        local scoreContainer = makeRightPanelContainer 'Score'

        do
            local scoreKillsLabel = Widget.makeText('Kills: 0', table.unpack(Theme.defaultFont))
            scoreKillsLabel:setTextColor(Theme.TEXT_COLOR)
            scoreContainer:addChild(scoreKillsLabel)

            local scoreLeaksLabel = Widget.makeText('Deaths: 0', table.unpack(Theme.defaultFont))
            scoreLeaksLabel:setTextColor(Theme.TEXT_COLOR)
            scoreContainer:addChild(scoreLeaksLabel)

            Score:onScoreChanged(function(kills, leaks)
                scoreKillsLabel:setText('Kills: ' .. kills)
                scoreLeaksLabel:setText('Leaks: ' .. leaks)
            end)
        end

        rightPanel:addChild(scoreContainer)
    end

    root:addChild(rightPanel)
end

