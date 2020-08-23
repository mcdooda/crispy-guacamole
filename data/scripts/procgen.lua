local Button = require 'data/scripts/ui/button'
local Slider = require 'data/scripts/ui/slider'
local Checkbox = require 'data/scripts/ui/checkbox'

return function(addContainer, makeSeparator, font)
    local procGenContainer = addContainer 'Proc Gen'

    local widthInput
    local heightInput

    do
        local widthLabel = Widget.makeText('Width', table.unpack(font))
        widthLabel:setTextColor(0x000000FF)
        procGenContainer:addChild(widthLabel)

        widthInput = Widget.makeNumberInput(table.unpack(font))
        widthInput:setValue(10)
        widthInput:setSize(40, 15)
        widthInput:setTextColor(0x000000FF)
        widthInput:setSizePolicy(Widget.SizePolicy.FIXED_X + Widget.SizePolicy.FIXED_Y)
        widthInput:setMin(1)
        widthInput:setMax(1000)
        procGenContainer:addChild(widthInput)
    end

    do
        local heightLabel = Widget.makeText('Height', table.unpack(font))
        heightLabel:setTextColor(0x000000FF)
        procGenContainer:addChild(heightLabel)

        heightInput = Widget.makeNumberInput(table.unpack(font))
        heightInput:setValue(10)
        heightInput:setSize(40, 15)
        heightInput:setTextColor(0x000000FF)
        heightInput:setSizePolicy(Widget.SizePolicy.FIXED_X + Widget.SizePolicy.FIXED_Y)
        heightInput:setMin(1)
        heightInput:setMax(1000)
        procGenContainer:addChild(heightInput)
    end

    procGenContainer:addChild(makeSeparator())

    do
        local noiseFrequencyLabel = Widget.makeText('Noise Frequency', table.unpack(font))
        noiseFrequencyLabel:setTextColor(0x000000FF)
        procGenContainer:addChild(noiseFrequencyLabel)

        local noiseFrequencySlider = Slider:new(146, 15, 1, 10, 2)
        procGenContainer:addChild(noiseFrequencySlider.container)
    end

    procGenContainer:addChild(makeSeparator())

    do
        local grainLabel = Widget.makeText('Elevation Grain', table.unpack(font))
        grainLabel:setTextColor(0x000000FF)
        procGenContainer:addChild(grainLabel)

        local grainSlider = Slider:new(146, 15, 1, 4, 2.5)
        procGenContainer:addChild(grainSlider.container)
    end

    procGenContainer:addChild(makeSeparator())

    do
        local grainLabel = Widget.makeText('Biomes Grain', table.unpack(font))
        grainLabel:setTextColor(0x000000FF)
        procGenContainer:addChild(grainLabel)

        local grainSlider = Slider:new(146, 15, 1, 4, 2.5)
        procGenContainer:addChild(grainSlider.container)
    end

    procGenContainer:addChild(makeSeparator())

    do
        local redistributeAltitudeLabel = Widget.makeText('Redistribute Altitude', table.unpack(font))
        redistributeAltitudeLabel:setTextColor(0x000000FF)
        procGenContainer:addChild(redistributeAltitudeLabel)

        local redistributeAltitude = Slider:new(146, 15, 0.01, 10, 1)
        procGenContainer:addChild(redistributeAltitude.container)
    end

    procGenContainer:addChild(makeSeparator())

    do
        local waterLevelLabel = Widget.makeText('Step Height', table.unpack(font))
        waterLevelLabel:setTextColor(0x000000FF)
        procGenContainer:addChild(waterLevelLabel)

        local stepHeightSlider = Slider:new(146, 15, 0, 10, 1)
        procGenContainer:addChild(stepHeightSlider.container)
    end

    procGenContainer:addChild(makeSeparator())

    do
        local waterLevelLabel = Widget.makeText('Water Level', table.unpack(font))
        waterLevelLabel:setTextColor(0x000000FF)
        procGenContainer:addChild(waterLevelLabel)

        local waterLevelSlider = Slider:new(146, 15, 0, 100, 1)
        procGenContainer:addChild(waterLevelSlider.container)
    end

    procGenContainer:addChild(makeSeparator())

    do
        local generateSingleIslandCheckbox = Checkbox:new(true, 'Generate Single Island')
        procGenContainer:addChild(generateSingleIslandCheckbox.container)
    end

    procGenContainer:addChild(makeSeparator())

    do
        local generateButton = Button:new(Widget.makeText('Generate', table.unpack(font)))
        generateButton.container:click(function()
            print 'Generating...'
            Map.reset(widthInput:getValue(), heightInput:getValue(), 'mods/crispy-guacamole/tiles/grass')
            print 'Generated!'
        end)
        procGenContainer:addChild(generateButton.container)
    end
end