local Button = require 'data/scripts/ui/button'
local Slider = require 'data/scripts/ui/slider'
local Checkbox = require 'data/scripts/ui/checkbox'
local Perlin = require 'data/scripts/perlin'

return function(addContainer, makeSeparator, font)
    local procGenContainer = addContainer 'Proc Gen'

    local widthInput
    local heightInput
    local noiseFrequencySlider
    local altitudeFactorSlider
    local redistributeAltitudeSlider
    local grainSlider
    local stepHeightSlider
    local waterLevelSlider
    local generateSingleIslandCheckbox

    do
        local widthLabel = Widget.makeText('Width', table.unpack(font))
        widthLabel:setTextColor(0x000000FF)
        procGenContainer:addChild(widthLabel)

        widthInput = Widget.makeNumberInput(table.unpack(font))
        widthInput:setValue(50)
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
        heightInput:setValue(50)
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

        noiseFrequencySlider = Slider:new(146, 15, 0, 0.2, 0.1)
        procGenContainer:addChild(noiseFrequencySlider.container)
    end

    procGenContainer:addChild(makeSeparator())

    do
        local altitudeFactorLabel = Widget.makeText('Altitude Factor', table.unpack(font))
        altitudeFactorLabel:setTextColor(0x000000FF)
        procGenContainer:addChild(altitudeFactorLabel)

        altitudeFactorSlider = Slider:new(146, 15, 0, 10, 5)
        procGenContainer:addChild(altitudeFactorSlider.container)
    end

    procGenContainer:addChild(makeSeparator())

    do
        local redistributeAltitudeLabel = Widget.makeText('Redistribute Altitude', table.unpack(font))
        redistributeAltitudeLabel:setTextColor(0x000000FF)
        procGenContainer:addChild(redistributeAltitudeLabel)

        redistributeAltitudeSlider = Slider:new(146, 15, 0.01, 7, 1)
        procGenContainer:addChild(redistributeAltitudeSlider.container)
    end

    procGenContainer:addChild(makeSeparator())

    do
        local grainLabel = Widget.makeText('Elevation Grain', table.unpack(font))
        grainLabel:setTextColor(0x000000FF)
        procGenContainer:addChild(grainLabel)

        grainSlider = Slider:new(146, 15, 1, 4, 2)
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
        local stepHeightLabel = Widget.makeText('Step Height', table.unpack(font))
        stepHeightLabel:setTextColor(0x000000FF)
        procGenContainer:addChild(stepHeightLabel)

        stepHeightSlider = Slider:new(146, 15, 0, 3, 1)
        procGenContainer:addChild(stepHeightSlider.container)
    end

    procGenContainer:addChild(makeSeparator())

    do
        local waterLevelLabel = Widget.makeText('Water Level', table.unpack(font))
        waterLevelLabel:setTextColor(0x000000FF)
        procGenContainer:addChild(waterLevelLabel)

        waterLevelSlider = Slider:new(146, 15, 0, 10, 1)
        procGenContainer:addChild(waterLevelSlider.container)
    end

    procGenContainer:addChild(makeSeparator())

    do
        generateSingleIslandCheckbox = Checkbox:new(true, 'Generate Single Island')
        procGenContainer:addChild(generateSingleIslandCheckbox.container)
    end

    procGenContainer:addChild(makeSeparator())

    do
        local generateButton = Button:new(Widget.makeText('Generate', table.unpack(font)))
        generateButton.container:click(function()
            print 'Generating...'
            local width = widthInput:getValue()
            local height = heightInput:getValue()
            local noiseFrequency = noiseFrequencySlider:getValue()
            print('noiseFrequency=', noiseFrequency)
            local altitudeFactor = altitudeFactorSlider:getValue()
            print('altitudeFactor=', altitudeFactor)
            local redistributeAltitude = redistributeAltitudeSlider:getValue()
            print('redistributeAltitude=', redistributeAltitude)
            local grain = grainSlider:getValue()
            print('grain=', grain)
            local stepHeight = stepHeightSlider:getValue()
            print('stepHeight=', stepHeight)
            local waterLevel = waterLevelSlider:getValue()
            if stepHeight ~= 0 then
                waterLevel = math.floor(waterLevel / stepHeight + 0.5) * stepHeight
            end
            print('waterLevel=', waterLevel)
            local generateSingleIsland = generateSingleIslandCheckbox:getValue()

            Map.reset(width, height, 'mods/crispy-guacamole/tiles/grass')
            Map.eachTile(function(tile)
                local x, y = Map.getTilePosition(tile)
                local z = 0

                local noiseLevel = 1
                local levelGrain = grain
                while levelGrain > 1 do
                    local p = 2 ^ (noiseLevel - 1)
                    z = z + (Perlin:noise(x * noiseFrequency * p, y * noiseFrequency * p) * 0.5 + 0.5) / p
                    levelGrain = levelGrain - 1
                    noiseLevel = noiseLevel + 1
                end
                if levelGrain > 0 then
                    noiseLevel = noiseLevel + levelGrain
                    local p = 2 ^ (noiseLevel - 1)
                    z = z + (Perlin:noise(x * noiseFrequency * p, y * noiseFrequency * p) * 0.5 + 0.5) / p
                end
                z = z ^ redistributeAltitude
                z = z * altitudeFactor

                if generateSingleIsland then
                    local distanceToCenter = flat.Vector2(x, y):length()
                    local distanceToCenterNormalized = distanceToCenter / (math.max(width, height) / 2)
                    local islandAltitudeFactor = 1 - math.sqrt(distanceToCenterNormalized)
                    z = z * islandAltitudeFactor
                end

                if stepHeight ~= 0 then
                    z = math.floor(z / stepHeight + 0.5) * stepHeight
                end

                if z >= waterLevel then
                    Map.setTileZ(tile, z)
                else
                    Map.setTileZ(tile, waterLevel)
                    Map.setTileTemplate(tile, 'mods/crispy-guacamole/tiles/water')
                end
            end)
            local cameraOrigin = game.convertToCameraPosition(flat.Vector3(0, 0, 0))
            game.setCameraCenter(cameraOrigin)
            print 'Generated!'
        end)
        procGenContainer:addChild(generateButton.container)
    end
end