local Inventory = {
    maxItems = 6,
    items ={},
    maxItemsChanged = {},
    itemsChanged = {}
}

function Inventory:addItem(item)
    if #self.items < self.maxItems then
        self.items[#self.items + 1] = item
        for i = 1, #self.itemsChanged do
            self.itemsChanged[i]()
        end
    end
end

function Inventory:onItemsChanged(itemsChanged)
    self.itemsChanged[#self.itemsChanged + 1] = itemsChanged
end

function Inventory:onMaxItemsChanged(maxItemsChanged)
    self.maxItemsChanged[#self.maxItemsChanged + 1] = maxItemsChanged
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