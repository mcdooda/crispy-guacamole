local UiSettings = require 'data/scripts/ui/uisettings'

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

    do
        local speedDownButton = Widget.makeText(' << ', table.unpack(font))
        speedDownButton:setTextColor(0x000000FF)
        speedDownButton:click(function()
            updateGameSpeed(timeSpeed * 0.5)
            Game.debug_setTimeSpeed(timeSpeed)
        end)
        buttonsContainer:addChild(speedDownButton)
    end

    do
        local pauseButton = Widget.makeText(' || ', table.unpack(font))
        pauseButton:setTextColor(0x000000FF)
        pauseButton:click(function()
            updateGameSpeed('<pause>')
            timePaused = true
            Game.debug_pauseNextFrame()
        end)
        buttonsContainer:addChild(pauseButton)
    end

    do
        local resumeButton = Widget.makeText(' > ', table.unpack(font))
        resumeButton:setTextColor(0x000000FF)
        resumeButton:click(function()
            timePaused = false
            updateGameSpeed(timeSpeed)
            Game.debug_resume()
        end)
        buttonsContainer:addChild(resumeButton)
    end

    do
        local speedUpButton = Widget.makeText(' >> ', table.unpack(font))
        speedUpButton:setTextColor(0x000000FF)
        speedUpButton:click(function()
            updateGameSpeed(timeSpeed * 2)
            Game.debug_setTimeSpeed(timeSpeed)
        end)
        buttonsContainer:addChild(speedUpButton)
    end

    gameSpeedContainer:addChild(buttonsContainer)

    timeSpeedLabel = Widget.makeText('Game speed: 1', table.unpack(font))
    timeSpeedLabel:setTextColor(0x000000FF)
    gameSpeedContainer:addChild(timeSpeedLabel)
end