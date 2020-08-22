local Button = require 'data/scripts/ui/button'

return function(addContainer, makeSeparator, font)
    local profilerContainer = addContainer 'Profiler'

    local snapshot

    local startButton = Button:new(Widget.makeText('Start', table.unpack(font)))
    startButton.container:click(function()
        flat.ui.info 'Profiler started'
        flat.profiler.start()
    end)
    profilerContainer:addChild(startButton.container)

    local stopButton = Button:new(Widget.makeText('Stop', table.unpack(font)))
    stopButton.container:click(function()
        flat.profiler.stop()
        flat.ui.info 'Profiler stopped'
    end)
    profilerContainer:addChild(stopButton.container)
end