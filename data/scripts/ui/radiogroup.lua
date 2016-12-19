
local radiobutton  = require 'data/scripts/ui/radiobutton'

local RadioGroup   = {}
RadioGroup.__index = RadioGroup

local function triggerValueChanged(radioGroup, value)
    local callbacks = radioGroup.valueChangedCallbacks
    for i = 1, #callbacks do
        callbacks[i](radioGroup, value)
    end
end


local function clicked(radioGroup, radioButton)
    if radioButton:getState() ~= 'disabled' then
        radioGroup:setValue(radioButton:getValue())
        for k,v in pairs(radioGroup.allRadiosButtons) do
            v:setVisible(v:getValue() == radioButton:getValue())
        end
    end

    return true
end

function RadioGroup:new(radioButtonArray, opts)

    local valueChangedCallbacks = opts.valueChangedCallbacks or {}
    local disposition           = opts.disposition  or 'horizontal'
    local container             = disposition == 'horizontal' and Widget.makeLineFlow() or Widget.makeColumnFlow()
    local value                 = false
    local allRadiosButtons      = {}
    container:setBackgroundColor(0xFF000000)
    local o = setmetatable({
        container             = container,
        allRadiosButtons      = allRadiosButtons,
        value                 = value,
        valueChangedCallbacks = valueChangedCallbacks
    }, self)


    for i = 1, #radioButtonArray do
        radioButtonArray[i].value = radioButtonArray[i].value or radioButtonArray[i].text
        local rb = radiobutton:new(
            radioButtonArray[i].value,
            radioButtonArray[i].text,
            radioButtonArray[i].state
        )
        allRadiosButtons[radioButtonArray[i].value] = rb
        if not value and radioButtonArray[i].value then
            value = radioButtonArray[i].text
        end
        rb.container:click(function(c) clicked(o, rb) end)

        container:addChild(rb.container)
    end
    return o

end

function RadioGroup:getValue()
    return self.value
end

function RadioGroup:getRadio(value)
    return self.allRadiosButtons[value]
end


function RadioGroup:setValue(value)
    assert(self.allRadiosButtons[value] ~= nil)
    self.value = value
    triggerValueChanged(self, value)
end

return RadioGroup