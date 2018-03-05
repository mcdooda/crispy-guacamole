local Life = {
    amount = 2,
    amountChanged = {}
}

function Life:add(amount)
    self:setAmount(self.amount + amount)
end

function Life:sub(amount)
    self:setAmount(self.amount - amount)
end

function Life:onAmountChanged(amountChanged)
    self.amountChanged[#self.amountChanged + 1] = amountChanged
end

function Life:setAmount(amount)
    assert(amount >= 0)
    self.amount = amount
    for i = 1, #self.amountChanged do
        self.amountChanged[i]()
    end
end

function Life:getAmount()
    return self.amount
end

return Life