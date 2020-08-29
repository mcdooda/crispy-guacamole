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
    local altitudeGrainSlider
    local biomesGrainSlider
    local stepHeightSlider
    local waterLevelSlider
    local islandCurveSlider
    local islandHeightSlider

    do
        local widthLabel = Widget.makeText('Width', table.unpack(font))
        widthLabel:setTextColor(0x000000FF)
        procGenContainer:addChild(widthLabel)

        widthInput = Widget.makeNumberInput(table.unpack(font))
        widthInput:setValue(100)
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
        heightInput:setValue(100)
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

        noiseFrequencySlider = Slider:new(146, 15, 0, 0.1, 0.05)
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
        local altitudeGrainLabel = Widget.makeText('Altitude Grain', table.unpack(font))
        altitudeGrainLabel:setTextColor(0x000000FF)
        procGenContainer:addChild(altitudeGrainLabel)

        altitudeGrainSlider = Slider:new(146, 15, 1, 4, 2)
        procGenContainer:addChild(altitudeGrainSlider.container)
    end

    procGenContainer:addChild(makeSeparator())

    do
        local biomesGrainLabel = Widget.makeText('Biomes Grain', table.unpack(font))
        biomesGrainLabel:setTextColor(0x000000FF)
        procGenContainer:addChild(biomesGrainLabel)

        biomesGrainSlider = Slider:new(146, 15, 1, 4, 2.5)
        procGenContainer:addChild(biomesGrainSlider.container)
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
        local islandCurveLabel = Widget.makeText('Island Curve', table.unpack(font))
        islandCurveLabel:setTextColor(0x000000FF)
        procGenContainer:addChild(islandCurveLabel)

        islandCurveSlider = Slider:new(146, 15, 0, 10, 1)
        procGenContainer:addChild(islandCurveSlider.container)
    end

    procGenContainer:addChild(makeSeparator())

    do
        local islandHeightLabel = Widget.makeText('Island Height', table.unpack(font))
        islandHeightLabel:setTextColor(0x000000FF)
        procGenContainer:addChild(islandHeightLabel)

        islandHeightSlider = Slider:new(146, 15, 0, 100, 1)
        procGenContainer:addChild(islandHeightSlider.container)
    end

    procGenContainer:addChild(makeSeparator())

    do
        local function clamp(value, min, max)
            if value < min then
                return min
            elseif value > max then
                return max
            else
                return value
            end
        end

        local function getTileTemplate(altitude, moisture)
            local oceanTile                    = 'mods/crispy-guacamole/tiles/water'
            local beachTile                    = 'mods/crispy-guacamole/tiles/sand'
            local scorchedTile                 = 'mods/crispy-guacamole/tiles/dirt_light'
            local bareTile                     = 'mods/crispy-guacamole/tiles/frozen_dirt'
            local tundraTile                   = 'mods/crispy-guacamole/tiles/snow'
            local snowTile                     = 'mods/crispy-guacamole/tiles/snow'
            local temperateDesertTile          = 'mods/crispy-guacamole/tiles/grass_light'
            local shrubLandTile                = 'mods/crispy-guacamole/tiles/high_grass'
            local taigaTile                    = 'mods/crispy-guacamole/tiles/snow'
            local grassLandTile                = 'mods/crispy-guacamole/tiles/grass'
            local temperateDeciduousForestTile = 'mods/crispy-guacamole/tiles/grass'
            local temperateRainForestTile      = 'mods/crispy-guacamole/tiles/high_grass'
            local subtropicalDesertTile        = 'mods/crispy-guacamole/tiles/sand'
            local tropicalSeasonalForestTile   = 'mods/crispy-guacamole/tiles/high_grass'
            local tropicalRainForestTile       = 'mods/crispy-guacamole/tiles/high_grass'

            assert(0 <= altitude and altitude <= 1)
            assert(0 <= moisture and moisture <= 1)

            if altitude <= 0.001 then
                return oceanTile
            elseif altitude < 0.1 then
                return beachTile
            end
            
            if altitude > 0.8 then
                if moisture < 0.1 then
                    return scorchedTile
                elseif moisture < 0.2 then
                    return bareTile
                elseif moisture < 0.5 then
                    return tundraTile
                else
                    return snowTile
                end
            end
          
            if altitude > 0.6 then
                if moisture < 0.33 then
                    return temperateDesertTile
                elseif moisture < 0.66 then
                    return shrubLandTile
                else
                    return taigaTile;
                end
            end
          
            if altitude > 0.3 then
                if moisture < 0.16 then
                    return temperateDesertTile;
                elseif moisture < 0.50 then
                    return grassLandTile;
                elseif moisture < 0.83 then
                    return temperateDeciduousForestTile;
                else
                    return temperateRainForestTile;
                end
            end
          
            if moisture < 0.16 then
                return subtropicalDesertTile;
            elseif moisture < 0.33 then
                return grassLandTile;
            elseif moisture < 0.66 then
                return tropicalSeasonalForestTile;
            else
                return tropicalRainForestTile;
            end
        end

        local function perlinNoise(x, y, noiseFrequency, grain)
            local z = 0
            local maxZ = 0
            local noiseLevel = 1
            local levelGrain = grain
            while levelGrain > 1 do
                local p = 2 ^ (noiseLevel - 1)
                z = z + (Perlin:noise(x * noiseFrequency * p, y * noiseFrequency * p) * 0.5 + 0.5) / p
                maxZ = maxZ + 1 / p
                levelGrain = levelGrain - 1
                noiseLevel = noiseLevel + 1
            end
            if levelGrain > 0 then
                noiseLevel = noiseLevel + levelGrain - 1
                local p = 2 ^ (noiseLevel - 1)
                z = z + (Perlin:noise(x * noiseFrequency * p, y * noiseFrequency * p) * 0.5 + 0.5) / p
                maxZ = maxZ + 1 / p
            end
            assert(0 <= z and z <= maxZ)
            return z, maxZ
        end

        local function generate()
            print 'Generating...'
            local width = widthInput:getValue()
            local height = heightInput:getValue()
            local noiseFrequency = noiseFrequencySlider:getValue()
            local altitudeFactor = altitudeFactorSlider:getValue()
            local redistributeAltitude = redistributeAltitudeSlider:getValue()
            local altitudeGrain = altitudeGrainSlider:getValue()
            local biomesGrain = biomesGrainSlider:getValue()
            local stepHeight = stepHeightSlider:getValue()
            local waterLevel = waterLevelSlider:getValue()
            if stepHeight ~= 0 then
                waterLevel = math.floor(waterLevel / stepHeight + 0.5) * stepHeight
            end
            local islandCurve = islandCurveSlider:getValue()
            local islandHeight = islandHeightSlider:getValue()

            local minZ, maxZ

            Map.reset(width, height, 'mods/crispy-guacamole/tiles/grass')
            Map.eachTile(function(tile)
                local x, y = Map.getTilePosition(tile)
                local z = perlinNoise(x, y, noiseFrequency, altitudeGrain)
                z = z ^ redistributeAltitude
                z = z * altitudeFactor

                -- island curve
                local distanceToCenter = flat.Vector2(x, y):length()
                local distanceToCenterNormalized = distanceToCenter / (math.max(width, height) / 2)
                local islandAltitudeFactor = clamp(1 - distanceToCenterNormalized, 0, 1)
                z = z + (islandAltitudeFactor ^ islandCurve) * islandHeight

                if z < waterLevel then
                    z = waterLevel
                end

                Map.setTileZ(tile, z)

                if not minZ or z < minZ then
                    minZ = z
                end
                if not maxZ or z > maxZ then
                    maxZ = z
                end
            end)
            Map.eachTile(function(tile)
                local x, y = Map.getTilePosition(tile)
                local moisture, maxMoisture = perlinNoise(x + 1000, y + 1000, noiseFrequency, biomesGrain)
                local z = Map.getTileZ(tile)
                local normalizedZ = clamp((z - minZ) / (maxZ - minZ), 0, 1)
                local tileTemplate = getTileTemplate(normalizedZ, moisture / maxMoisture)
                Map.setTileTemplate(tile, tileTemplate)
                if stepHeight ~= 0 then
                    z = math.floor(z / stepHeight + 0.5) * stepHeight
                    Map.setTileZ(tile, z)
                end
            end)

            local cameraOrigin = game.convertToCameraPosition(flat.Vector3(0, 0, 0))
            game.setCameraCenter(cameraOrigin)
            print 'Generated!'
        end

        local generateButton = Button:new(Widget.makeText('Generate', table.unpack(font)))
        generateButton.container:click(generate)
        procGenContainer:addChild(generateButton.container)

        local sliders = {
            noiseFrequency       = noiseFrequencySlider,
            altitudeFactor       = altitudeFactorSlider,
            redistributeAltitude = redistributeAltitudeSlider,
            altitudeGrain        = altitudeGrainSlider,
            biomesGrain          = biomesGrainSlider,
            stepHeight           = stepHeightSlider,
            waterLevel           = waterLevelSlider,
            islandCurve          = islandCurveSlider,
            islandHeight         = islandHeightSlider
        }

        local randomizeButton = Button:new(Widget.makeText('Randomize', table.unpack(font)))
        randomizeButton.container:click(function()
            local sliders = {
                noiseFrequencySlider,
                altitudeFactorSlider,
                redistributeAltitudeSlider,
                altitudeGrainSlider,
                biomesGrainSlider,
                stepHeightSlider,
                waterLevelSlider,
                islandCurveSlider,
                islandHeightSlider
            }
            for sliderName, slider in pairs(sliders) do
                local value = slider.min + math.random() * (slider.max - slider.min)
                slider:setValue(value)
            end
            generate()
        end)
        procGenContainer:addChild(randomizeButton.container)

        local randomizeButton = Button:new(Widget.makeText('Copy/Paste', table.unpack(font)))
        randomizeButton.container:setFocusable(true)

        local function getState()
            local state = {
                width  = widthInput:getValue(),
                height = heightInput:getValue()
            }
            for sliderName, slider in pairs(sliders) do
                state[sliderName] = slider:getValue()
            end
            return state
        end

        local function setState(state)
            widthInput:setValue(state.width)
            heightInput:setValue(state.height)
            for sliderName, slider in pairs(sliders) do
                slider:setValue(state[sliderName])
            end
        end

        randomizeButton.container:copy(function(widget)
            local clipboardContent = getState()
            return flat.dumpToString(clipboardContent)
        end)
        randomizeButton.container:paste(function(widget, text)
            local clipboardContent
            local ok, err = pcall(function()
                clipboardContent = assert(load('return ' .. text, 'clipboard', 't', {}))()
            end)
            if not ok then
                print('Error:', err)
                return
            end
            setState(clipboardContent)
            generate()
        end)
        procGenContainer:addChild(randomizeButton.container)
    end
end
