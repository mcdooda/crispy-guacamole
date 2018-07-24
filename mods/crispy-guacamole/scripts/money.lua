local Money = {
    amount = 0,
    income = 0,
    amountChanged = flat.Slot:new(),
    incomeChanged = flat.Slot:new()
}

function Money:add(amount)
    self:setAmount(self.amount + amount)
end

function Money:sub(amount)
    self:setAmount(self.amount - amount)
end

function Money:onAmountChanged(amountChanged)
    self.amountChanged:on(amountChanged)
end

function Money:setAmount(amount)
    assert(amount >= 0)
    if amount ~= self.amount then
        self.amount = amount
        self.amountChanged(amount)
    end
end

function Money:getAmount()
    return self.amount
end

function Money:addIncome(income)
    assert(income > 0)
    self.income = self.income + income
    self.incomeChanged(self.income)
end

return Money