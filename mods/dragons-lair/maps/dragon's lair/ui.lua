local Theme     = require 'mods/dragons-lair/maps/dragon\'s lair/theme'
local Money     = require 'mods/dragons-lair/maps/dragon\'s lair/money'
local Life      = require 'mods/dragons-lair/maps/dragon\'s lair/life'
local Inventory = require 'mods/dragons-lair/maps/dragon\'s lair/inventory'
local Items     = require 'mods/dragons-lair/maps/dragon\'s lair/items'
local ItemIcon  = require 'mods/dragons-lair/maps/dragon\'s lair/itemicon'
local Path      = require 'data/scripts/path'
local root      = Widget.getRoot()

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
    -- life and gold
    do
        local topPanel = Widget.makeLineFlow()
        topPanel:setPositionPolicy(Widget.PositionPolicy.LEFT + Widget.PositionPolicy.TOP)
        topPanel:setSizePolicy(Widget.SizePolicy.COMPRESS)

        do
            local moneyWidget = Widget.makeFixedSize(180, 111)
            moneyWidget:setPositionPolicy(Widget.PositionPolicy.LEFT + Widget.PositionPolicy.CENTER_Y)
            moneyWidget:setBackground(Path.getModFilePath 'ui/user/money-frame.png')

            local moneyAmountLabel = Widget.makeText('0', table.unpack(Theme.gameFont))
            moneyAmountLabel:setPositionPolicy(Widget.PositionPolicy.CENTER)
            moneyAmountLabel:setTextColor(Theme.TEXT_COLOR)
            moneyAmountLabel:setMargin(0, -50, 0 , 0)
            moneyWidget:addChild(moneyAmountLabel)
            topPanel:addChild(moneyWidget)

            Money:onAmountChanged(function()
                moneyAmountLabel:setText(Money:getAmount())
            end)
        end

        do
            local lifeAmount = Widget.makeLineFlow()
            lifeAmount:setPositionPolicy(Widget.PositionPolicy.RIGHT + Widget.PositionPolicy.TOP)
            lifeAmount:setSizePolicy(Widget.SizePolicy.FIXED_Y + Widget.SizePolicy.COMPRESS_X)
            lifeAmount:setSize(0, 100)
            topPanel:addChild(lifeAmount)
            local drawHearts = function()
                lifeAmount:removeAllChildren()
                for i = 1, Life:getAmount() do
                    local heart = Widget.makeImage(Path.getModFilePath 'ui/user/heart.png')
                    heart:setPositionPolicy(Widget.PositionPolicy.CENTER)
                    heart:setSize(48, 48)
                    lifeAmount:addChild(heart)
                end
            end
            Life:onAmountChanged(drawHearts)
            drawHearts()
        end

        root:addChild(topPanel)
    end

    -- items
    do
        Inventory:addItem(Items[1])
        Inventory:addItem(Items[2])
        Inventory:addItem(Items[3])
        Inventory:addItem(Items[4])
        local itemPanel = Widget.makeLineFlow()
        itemPanel:setPositionPolicy(Widget.PositionPolicy.CENTER_X + Widget.PositionPolicy.BOTTOM)
        itemPanel:setSizePolicy(Widget.SizePolicy.COMPRESS)
        for i = 1, Inventory:getMaxItems() do
            local frame = Widget.makeImage(Path.getModFilePath 'ui/user/item-frame.png')
            frame:setSize(56, 56)
            if i <= #Inventory.items then
                local item = Inventory.items[i]
                ItemIcon:new(item, frame)
            end
            itemPanel:addChild(frame)
        end
        root:addChild(itemPanel)
    end
end

