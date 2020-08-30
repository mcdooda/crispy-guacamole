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
    local slopeProbabilitySlider
    local slopeProbabilityThresholdSlider
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
        local slopeProbabilityLabel = Widget.makeText('Slope Probability', table.unpack(font))
        slopeProbabilityLabel:setTextColor(0x000000FF)
        procGenContainer:addChild(slopeProbabilityLabel)

        slopeProbabilitySlider = Slider:new(146, 15, 0, 1, 0)
        procGenContainer:addChild(slopeProbabilitySlider.container)
    end

    procGenContainer:addChild(makeSeparator())

    do
        local slopeProbabilityThresholdLabel = Widget.makeText('Slope Probability Threshold', table.unpack(font))
        slopeProbabilityThresholdLabel:setTextColor(0x000000FF)
        procGenContainer:addChild(slopeProbabilityThresholdLabel)

        slopeProbabilityThresholdSlider = Slider:new(146, 15, 0, 0.05, 0)
        procGenContainer:addChild(slopeProbabilityThresholdSlider.container)
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

        local biomes = {
            ocean = {
                tileTemplate = 'mods/crispy-guacamole/tiles/water'
            },
            beach = {
                tileTemplate = 'mods/crispy-guacamole/tiles/sand',
            },
            scorched = {
                tileTemplate = 'mods/crispy-guacamole/tiles/dirt_light',
            },
            bare = {
                tileTemplate = 'mods/crispy-guacamole/tiles/frozen_dirt',
            },
            tundra = {
                tileTemplate = 'mods/crispy-guacamole/tiles/snow',
                props = {
                    {'mods/crispy-guacamole/props/snow_pine', 0.8},
                }
            },
            snow = {
                tileTemplate = 'mods/crispy-guacamole/tiles/snow',
                props = {
                    {'mods/crispy-guacamole/props/snow_pine', 0.5},
                }
            },
            temperateDesert = {
                tileTemplate = 'mods/crispy-guacamole/tiles/grass_light',
            },
            shrubLand = {
                tileTemplate = 'mods/crispy-guacamole/tiles/high_grass',
            },
            taiga = {
                tileTemplate = 'mods/crispy-guacamole/tiles/snow',
                props = {
                    {'mods/crispy-guacamole/props/snow_pine', 0.4},
                }
            },
            grassLand = {
                tileTemplate = 'mods/crispy-guacamole/tiles/grass',
                props = {
                    {'mods/crispy-guacamole/props/tree', 0.27},
                }
            },
            temperateDeciduousForest = {
                tileTemplate = 'mods/crispy-guacamole/tiles/grass',
                props = {
                    {'mods/crispy-guacamole/props/pine', 0.4},
                }
            },
            temperateRainForest = {
                tileTemplate = 'mods/crispy-guacamole/tiles/high_grass',
                props = {
                    {'mods/crispy-guacamole/props/pine', 0.4},
                }
            },
            subtropicalDesert = {
                tileTemplate = 'mods/crispy-guacamole/tiles/sand',
                props = {
                    {'mods/crispy-guacamole/props/cactus', 1},
                }
            },
            tropicalSeasonalForest = {
                tileTemplate = 'mods/crispy-guacamole/tiles/high_grass',
                props = {
                    {'mods/crispy-guacamole/props/pine', 0.4},
                }
            },
            tropicalRainForest = {
                tileTemplate = 'mods/crispy-guacamole/tiles/high_grass',
                props = {
                    {'mods/crispy-guacamole/props/pine', 0.4},
                }
            },
        }

        local function getBiome(altitude, moisture)
            assert(0 <= altitude and altitude <= 1)
            assert(0 <= moisture and moisture <= 1)

            if altitude <= 0.001 then
                return biomes.ocean
            elseif altitude < 0.1 then
                return biomes.beach
            end
            
            if altitude > 0.8 then
                if moisture < 0.1 then
                    return biomes.scorched
                elseif moisture < 0.2 then
                    return biomes.bare
                elseif moisture < 0.5 then
                    return biomes.tundra
                else
                    return biomes.snow
                end
            end
          
            if altitude > 0.6 then
                if moisture < 0.33 then
                    return biomes.temperateDesert
                elseif moisture < 0.66 then
                    return biomes.shrubLand
                else
                    return biomes.taiga;
                end
            end
          
            if altitude > 0.3 then
                if moisture < 0.16 then
                    return biomes.temperateDesert;
                elseif moisture < 0.50 then
                    return biomes.grassLand;
                elseif moisture < 0.83 then
                    return biomes.temperateDeciduousForest;
                else
                    return biomes.temperateRainForest;
                end
            end
          
            if moisture < 0.16 then
                return biomes.subtropicalDesert;
            elseif moisture < 0.33 then
                return biomes.grassLand;
            elseif moisture < 0.66 then
                return biomes.tropicalSeasonalForest;
            else
                return biomes.tropicalRainForest;
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
            local slopeProbability = slopeProbabilitySlider:getValue()
            local slopeProbabilityThreshold = slopeProbabilityThresholdSlider:getValue()
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
                local biome = getBiome(normalizedZ, moisture / maxMoisture)
                Map.setTileTemplate(tile, biome.tileTemplate)
                if stepHeight ~= 0 and slopeProbability - slopeProbabilityThreshold <= moisture / maxMoisture then
                    local originalZ = z
                    z = math.floor(z / stepHeight + 0.5) * stepHeight
                    if slopeProbabilityThreshold > 0 and moisture / maxMoisture <= slopeProbability then
                        local alpha = (moisture / maxMoisture - (slopeProbability - slopeProbabilityThreshold)) / slopeProbabilityThreshold
                        assert(0 <= alpha and alpha <= 1)
                        z = z * alpha + originalZ * (1 - alpha)
                        --z = z * (1 - alpha) + originalZ * alpha
                    end
                    Map.setTileZ(tile, z)
                end

                if biome.props then
                    local v, max = perlinNoise(x - 1000, y - 1000, noiseFrequency * 10, 1)
                    local alpha = v / max
                    for i = 1, #biome.props do
                        local prop, probability = table.unpack(biome.props[i])
                        if alpha < probability then
                            Map.setPropTemplate(tile, prop)
                            break
                        end
                    end
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
            noiseFrequency            = noiseFrequencySlider,
            altitudeFactor            = altitudeFactorSlider,
            redistributeAltitude      = redistributeAltitudeSlider,
            altitudeGrain             = altitudeGrainSlider,
            biomesGrain               = biomesGrainSlider,
            stepHeight                = stepHeightSlider,
            slopeProbability          = slopeProbabilitySlider,
            slopeProbabilityThreshold = slopeProbabilityThresholdSlider,
            waterLevel                = waterLevelSlider,
            islandCurve               = islandCurveSlider,
            islandHeight              = islandHeightSlider
        }

        local randomizeButton = Button:new(Widget.makeText('Randomize', table.unpack(font)))
        randomizeButton.container:click(function()
            for sliderName, slider in pairs(sliders) do
                local value = slider.min + math.random() * (slider.max - slider.min)
                slider:setValue(value)
            end
            generate()
        end)
        procGenContainer:addChild(randomizeButton.container)

        local copyPasteButton = Button:new(Widget.makeText('Copy/Paste', table.unpack(font)))
        copyPasteButton.container:setFocusable(true)

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
                if state[sliderName] then
                    slider:setValue(clamp(state[sliderName], slider.min, slider.max))
                end
            end
        end

        copyPasteButton.container:copy(function(widget)
            local clipboardContent = getState()
            return flat.dumpToString(clipboardContent)
        end)
        copyPasteButton.container:paste(function(widget, text)
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
        procGenContainer:addChild(copyPasteButton.container)
    end
end
