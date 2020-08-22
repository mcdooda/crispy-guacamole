local UiSettings = require 'data/scripts/ui/uisettings'
local Button = require 'data/scripts/ui/button'

return function(addContainer, makeSeparator, font)
	local gameSpeedContainer = addContainer 'Game Speed'

    local buttonsContainer = Widget.makeLineFlow()

    local timeSpeed = 1
    local timeSpeedLabel
    local timePaused = false
    local function updateGameSpeed(newGameSpeed)
        if type(newGameSpeed) == 'number' then
            if newGameSpeed > 0.9 then
                newGameSpeed = math.floor(newGameSpeed)
            end
            timeSpeed = newGameSpeed
        end
        if not timePaused then
            timeSpeedLabel:setText('Game speed: ' .. newGameSpeed)
        end
    end

    local buttonsOptions = { padding = {4, 8, 4, 8} }

    do
        local speedDownButton = Button:new(Widget.makeText('<<', table.unpack(font)), buttonsOptions)
        speedDownButton.container:click(function()
            updateGameSpeed(timeSpeed * 0.5)
            game.debug_setTimeSpeed(timeSpeed)
        end)
        buttonsContainer:addChild(speedDownButton.container)
    end

    do
        local pauseButton = Button:new(Widget.makeText('||', table.unpack(font)), buttonsOptions)
        pauseButton.container:click(function()
            updateGameSpeed('<pause>')
            timePaused = true
            game.debug_pauseNextFrame()
        end)
        buttonsContainer:addChild(pauseButton.container)
    end

    do
        local resumeButton = Button:new(Widget.makeText('>', table.unpack(font)), buttonsOptions)
        resumeButton.container:click(function()
            timePaused = false
            updateGameSpeed(timeSpeed)
            game.debug_resume()
        end)
        buttonsContainer:addChild(resumeButton.container)
    end

    do
        local speedUpButton = Button:new(Widget.makeText('>>', table.unpack(font)), buttonsOptions)
        speedUpButton.container:click(function()
            updateGameSpeed(timeSpeed * 2)
            game.debug_setTimeSpeed(timeSpeed)
        end)
        buttonsContainer:addChild(speedUpButton.container)
    end

    gameSpeedContainer:addChild(buttonsContainer)

    timeSpeedLabel = Widget.makeText('Game speed: 1', table.unpack(font))
    timeSpeedLabel:setTextColor(0x000000FF)
    gameSpeedContainer:addChild(timeSpeedLabel)
end