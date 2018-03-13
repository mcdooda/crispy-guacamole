local Money     = require 'mods/dragons-lair/maps/dragon\'s lair/money'
local Inventory = require 'mods/dragons-lair/maps/dragon\'s lair/inventory'

local ShopKeeper = {
    stock = {},
    stockChanged = flat.Slot:new()
}

function ShopKeeper:onStockChanged(stockChanged)
    self.stockChanged:on(stockChanged)
end

function ShopKeeper:addItem(item)
    self.stock[#self.stock + 1] = item
    self.stockChanged(self.stock)
end

function ShopKeeper:removeItemAt(index)
    table.remove(self.stock, index)
    self.stockChanged(self.stock)
end

function ShopKeeper:clear()
    self.stock = {}
    self.stockChanged(self.stock)
end

function ShopKeeper:getStock()
    return self.stock
end

function ShopKeeper:buyItem(index)
    if self.stock[index].cost <= Money:getAmount() then
        Money:sub(self.stock[index].cost)
        Inventory:addItem(self.stock[index])
        return true
    end
    return false
end

return ShopKeeper