local Money = {
    amount = 0,
    amountChanged = {}
}

function Money:add(amount)
    self:setAmount(self.amount + amount)
end

function Money:sub(amount)
    self:setAmount(self.amount - amount)
end

function Money:onAmountChanged(amountChanged)
    self.amountChanged[#self.amountChanged + 1] = amountChanged
end

function Money:setAmount(amount)
    assert(amount >= 0)
    self.amount = amount
    for i = 1, #self.amountChanged do
        self.amountChanged[i]()
    end
end

function Money:getAmount()
    return self.amount
end

return Money