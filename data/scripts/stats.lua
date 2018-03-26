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
		local timer = Timer.new()
		timer:onUpdate(function()
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
		end)
		timer:start(0, true)
	end
	
	-- entity count
	do
		local label = Widget.makeText('X entities', table.unpack(font))
		label:setTextColor(0x000000FF)
		statsContainer:addChild(label)

		local timer = Timer.new()
		timer:onUpdate(function()
			label:setText(format('%d entities', Map.getNumEntities()))
		end)
		timer:start(0, true)
	end	

	-- timer count
	do
		local label = Widget.makeText('X timers', table.unpack(font))
		label:setTextColor(0x000000FF)
		statsContainer:addChild(label)
		
		local timer = Timer.new()
		timer:onUpdate(function()
			label:setText(format('%d timers', Game.debug_getNumTimers()))
		end)
		timer:start(0, true)
	end
end