local Theme     = require 'mods/dragons-lair/ui/theme'
local Money     = require 'mods/dragons-lair/maps/dragons-lair/money'
local Life      = require 'mods/dragons-lair/maps/dragons-lair/life'
local Inventory = require 'mods/dragons-lair/maps/dragons-lair/inventory'
local Items     = require 'mods/dragons-lair/maps/dragons-lair/items'
local ItemIcon  = require 'mods/dragons-lair/maps/dragons-lair/itemicon'
local User      = require 'mods/dragons-lair/maps/dragons-lair/user'
local Path      = require 'data/scripts/path'

local ShopKeeper = require 'mods/dragons-lair/maps/dragons-lair/shopkeeper'

local root      = Widget.getRoot()

local function makeFrame(widget)
    local container = Widget.makeLineFlow()
    container:setSizePolicy(Widget.SizePolicy.COMPRESS_X + Widget.SizePolicy.EXPAND_Y)
    do
        local left = Widget.makeImage(Path.getModFilePath 'ui/user/left-frame.png')
        left:setSize(16, 56)
        container:addChild(left)
        local center = Widget.makeImage(Path.getModFilePath 'ui/user/center-frame.png')
        center:setSize(150, 56)
        center:addChild(widget)
        container:addChild(center)
        local right = Widget.makeImage(Path.getModFilePath 'ui/user/right-frame.png')
        right:setSize(16, 56)
        container:addChild(right)
    end

    return container
end


local function makeInventory()
    local inventoryPanel = Widget.makeLineFlow()
    inventoryPanel:setPositionPolicy(Widget.PositionPolicy.CENTER_X + Widget.PositionPolicy.BOTTOM)
    inventoryPanel:setSizePolicy(Widget.SizePolicy.COMPRESS)
    local itemPanel = Widget.makeLineFlow()
    itemPanel:setSizePolicy(Widget.SizePolicy.COMPRESS)
    local damages = Widget.makeText('Damages: ' .. math.floor(User:computeDamages()), table.unpack(Theme.UI_FONT))

    local function setItemsInventory()
        itemPanel:removeAllChildren()
        for i = 1, Inventory:getMaxItems() do
            local frame = Widget.makeImage(Path.getModFilePath 'ui/user/item-frame.png')
            frame:setSize(56, 56)
            if i <= #Inventory.items then
                local item = Inventory.items[i]
                ItemIcon:new(item, frame)
            end
            itemPanel:addChild(frame)
        end
        damages:setText('Damages: ' .. math.floor(User:computeDamages()), table.unpack(Theme.UI_FONT))
        return itemPanel
    end
    setItemsInventory()

    inventoryPanel:addChild(itemPanel)

    -- stats panel
    local statsPanel = Widget.makeColumnFlow()
    statsPanel:setMargin(10, 0, 0, 0)
    statsPanel:setSizePolicy(Widget.SizePolicy.COMPRESS)
    statsPanel:addChild(damages)
    local frame = makeFrame(statsPanel)
    inventoryPanel:addChild(frame)
    Inventory:onItemsChanged(setItemsInventory)
    return inventoryPanel
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

            local moneyAmountLabel = Widget.makeText('0', table.unpack(Theme.UI_FONT))
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
        ShopKeeper:addItem(Items[1])
        ShopKeeper:addItem(Items[2])
        ShopKeeper:addItem(Items[3])
        ShopKeeper:addItem(Items[4])
        ShopKeeper:addItem(Items[3])
        ShopKeeper:addItem(Items[4])

        local inventoryPanel = makeInventory()
        root:addChild(inventoryPanel)
    end
end

