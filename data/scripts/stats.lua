local Button = require 'data/scripts/ui/button'

return function(addContainer, makeSeparator, font)
	local format = string.format

	local statsContainer = addContainer 'Stats'

	-- fps
	do
		local averageFpsLabel = Widget.makeText('Average: X fps', table.unpack(font))
		averageFpsLabel:setTextColor(0x000000FF)
		statsContainer:addChild(averageFpsLabel)

		local highestFpsLabel = Widget.makeText('Highest: X fps', table.unpack(font))
		highestFpsLabel:setTextColor(0x000000FF)
		statsContainer:addChild(highestFpsLabel)

		local lowestFpsLabel = Widget.makeText('Lowest: X fps', table.unpack(font))
		lowestFpsLabel:setTextColor(0x000000FF)
		statsContainer:addChild(lowestFpsLabel)

		local highestFps = -math.huge
		local lowestFps = math.huge

		local resetButton = Button:new(Widget.makeText('Reset', table.unpack(font)))
		resetButton.container:click(function()
			highestFps = -math.huge
			lowestFps = math.huge
		end)
		statsContainer:addChild(resetButton.container)
		
		local getFrameRate = Time.getFrameRate

		local medianNumFrames = 50
		local medianFps = {}
		for j = 1, medianNumFrames do
			medianFps[j] = getFrameRate()
		end

		local i = 1
		local timer = flat.Timer()
		timer:onUpdate(function()
			local fps = getFrameRate()

			medianFps[i] = nil
			medianFps[i + medianNumFrames] = fps
			local displayedFps = 0
			for j = i + 1, i + medianNumFrames do
				displayedFps = displayedFps + medianFps[j]
			end
			displayedFps = displayedFps / medianNumFrames
			averageFpsLabel:setText(format('Average: %.1f fps', displayedFps))
			i = i + 1

			if fps > highestFps then
				highestFps = fps
				highestFpsLabel:setText(format('Highest: %.1f fps', highestFps))
			end

			if fps < lowestFps then
				lowestFps = fps
				lowestFpsLabel:setText(format('Lowest: %.1f fps', lowestFps))
			end
		end)
		timer:start(0, true)
	end
	
	-- entity count
	do
		local label = Widget.makeText('X entities', table.unpack(font))
		label:setTextColor(0x000000FF)
		statsContainer:addChild(label)

		local timer = flat.Timer()
		timer:onUpdate(function()
			label:setText(format('%d entities', Map.getNumEntities()))
		end)
		timer:start(0, true)
	end	

	-- timer count
	do
		local timersLabel = Widget.makeText('X timers', table.unpack(font))
		timersLabel:setTextColor(0x000000FF)
		statsContainer:addChild(timersLabel)
		local frameTimersLabel = Widget.makeText('X frame timers', table.unpack(font))
		frameTimersLabel:setTextColor(0x000000FF)
		statsContainer:addChild(frameTimersLabel)
		
		local timer = flat.Timer()
		timer:onUpdate(function()
			timersLabel:setText(format('%d timers', game.debug_getNumTimers()))
			frameTimersLabel:setText(format('%d frame timers', game.debug_getNumFrameTimers()))
		end)
		timer:start(0, true)
	end
end