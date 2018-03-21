return function(addContainer, makeSeparator, font)
	local format = string.format

	local statsContainer = addContainer 'Stats'

	-- fps
	do
		local label = Widget.makeText('X fps', table.unpack(font))
		label:setTextColor(0x000000FF)
		statsContainer:addChild(label)
		
		local getFrameRate = Time.getFrameRate
		local medianNumFrames = 50
		local medianFps = {}
		for j = 1, medianNumFrames do
			medianFps[j] = getFrameRate()
		end
		local i = 1
		Timer.start(0, function()
			local fps = getFrameRate()
			medianFps[i] = nil
			medianFps[i + medianNumFrames] = fps
			local displayedFps = 0
			for j = i + 1, i + medianNumFrames do
				displayedFps = displayedFps + medianFps[j]
			end
			displayedFps = displayedFps / medianNumFrames
			label:setText(format('%.1f fps', displayedFps))
			i = i + 1
		end, nil,  true)
	end
	
	-- entity count
	do
		local label = Widget.makeText('X entities', table.unpack(font))
		label:setTextColor(0x000000FF)
		statsContainer:addChild(label)
		
		Timer.start(0, function()
			label:setText(format('%d entities', Map.getNumEntities()))
		end, nil, true)
	end	

	-- timer count
	do
		local label = Widget.makeText('X timers', table.unpack(font))
		label:setTextColor(0x000000FF)
		statsContainer:addChild(label)
		
		Timer.start(0, function()
			label:setText(format('%d timers', Game.debug_getNumTimers()))
		end, nil, true)
	end
end