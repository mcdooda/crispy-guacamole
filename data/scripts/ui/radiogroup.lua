local RadioGroup   = {}
RadioGroup.__index = RadioGroup

function RadioGroup:new(radioButtons, opts)
    local o
    local radioButtonsByValue = {}
    for i = 1, #radioButtons do
        local radioButton = radioButtons[i]
        radioButtonsByValue[radioButton:getValue()] = radioButton
        radioButton.container:click(function(c)
            o:radioButtonClicked(radioButton)
        end)
    end
    o = setmetatable({
        radioButtonsByValue = radioButtonsByValue,
        value               = nil,
        valueChanged        = flat.Slot:new()
    }, self)
    return o
end

function RadioGroup:getValue()
    return self.value
end

function RadioGroup:setValue(value)
    assert(self.radioButtonsByValue[value] ~= nil)
    if value ~= self.value then
        self.value = value
        self.valueChanged(value)
        for radioButtonValue, radioButton in pairs(self.radioButtonsByValue) do
            radioButton:setVisible(value == radioButtonValue)
        end
    end
end

function RadioGroup:radioButtonClicked(radioButton)
    if radioButton:getState() ~= 'disabled' then
        self:setValue(radioButton:getValue())
    end
end

return RadioGroup