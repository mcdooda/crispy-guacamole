return function(addContainer, makeSeparator, font)
    local profilerContainer = addContainer 'Profiler'

    local snapshot

    local startButton = Widget.makeText('Start', table.unpack(font))
    startButton:setTextColor(0x000000FF)
    startButton:click(function()
        flat.ui.info 'Profiler started'
        flat.profiler.start()
    end)
    profilerContainer:addChild(startButton)

    local stopButton = Widget.makeText('Stop', table.unpack(font))
    stopButton:setTextColor(0x000000FF)
    stopButton:click(function()
        flat.profiler.stop()
        flat.ui.info 'Profiler stopped'
    end)
    profilerContainer:addChild(stopButton)
end