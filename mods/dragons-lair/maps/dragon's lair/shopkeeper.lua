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

return ShopKeeper