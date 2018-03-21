local Life = {
    amount = 3,
    amountChanged = flat.Slot:new()
}

function Life:add(amount)
    self:setAmount(self.amount + amount)
end

function Life:sub(amount)
    self:setAmount(self.amount - amount)
end

function Life:onAmountChanged(amountChanged)
    self.amountChanged:on(amountChanged)
end

function Life:setAmount(amount)
    assert(amount >= 0)
    if amount ~= self.amount then
        self.amount = amount
        self.amountChanged(amount)
    end
end

function Life:getAmount()
    return self.amount
end

return Life