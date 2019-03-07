local Theme = require 'mods/crispy-guacamole/ui/theme'
local Money = require 'mods/crispy-guacamole/scripts/money'

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

    -- money
    do
        local moneyContainer = makeRightPanelContainer 'Minerals'

        do
            local moneyAmountLabel = Widget.makeText(Money:getAmount(), table.unpack(Theme.defaultFont))
            moneyAmountLabel:setPositionPolicy(Widget.PositionPolicy.TOP_RIGHT)
            moneyAmountLabel:setTextColor(Theme.TEXT_COLOR)
            moneyContainer:addChild(moneyAmountLabel)

            Money:onAmountChanged(function(amount)
                moneyAmountLabel:setText(amount)
            end)
        end

        rightPanel:addChild(moneyContainer)
    end

    root:addChild(rightPanel)
end

