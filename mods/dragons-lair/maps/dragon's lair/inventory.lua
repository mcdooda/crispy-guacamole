local Inventory = {
    maxItems = 6,
    items ={},
    maxItemsChanged = flat.Slot:new(),
    itemsChanged = flat.Slot:new()
}

function Inventory:addItem(item)
    if #self.items < self.maxItems then
        self.items[#self.items + 1] = item
        self.itemsChanged()
    end
end

function Inventory:onItemsChanged(itemsChanged)
    self.itemsChanged:on(itemsChanged)
end

function Inventory:onMaxItemsChanged(maxItemsChanged)
    self.maxItemsChanged:on(maxItemsChanged)
end

function Inventory:setMaxItems(amount)
    assert(amount >= 0)
    self.maxItems = amount
    for i = 1, #self.maxItemsChanged do
        self.maxItemsChanged[i]()
    end
end

function Inventory:getMaxItems()
    return self.maxItems
end

return Inventory