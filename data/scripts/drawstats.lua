return function(addContainer, makeSeparator, font)
	local format = string.format

	local statsContainer = addContainer 'Draw Stats'

	do
		do
			local label = Widget.makeText('Opaque:', table.unpack(font))
			label:setTextColor(0x000000FF)
			statsContainer:addChild(label)
		end

		local numOpaqueObjectsLabel = Widget.makeText('X objects', table.unpack(font))
		numOpaqueObjectsLabel:setTextColor(0x000000FF)
		statsContainer:addChild(numOpaqueObjectsLabel)

		local numOpaqueDrawCallsLabel = Widget.makeText('X draw calls', table.unpack(font))
		numOpaqueDrawCallsLabel:setTextColor(0x000000FF)
		statsContainer:addChild(numOpaqueDrawCallsLabel)

		do
			local label = Widget.makeText('Transparent:', table.unpack(font))
			label:setTextColor(0x000000FF)
			statsContainer:addChild(label)
		end

		local numTransparentObjectsLabel = Widget.makeText('X objects', table.unpack(font))
		numTransparentObjectsLabel:setTextColor(0x000000FF)
		statsContainer:addChild(numTransparentObjectsLabel)

		local numTransparentDrawCallsLabel = Widget.makeText('X draw calls', table.unpack(font))
		numTransparentDrawCallsLabel:setTextColor(0x000000FF)
		statsContainer:addChild(numTransparentDrawCallsLabel)

		local timer = flat.Timer()
		timer:onUpdate(function()
			local numOpaqueObjects, numOpaqueDrawCalls, numTransparentObjects, numTransparentDrawCalls = Map.debug_getDrawStats()
			numOpaqueObjectsLabel:setText(format('%d objects', numOpaqueObjects))
			numOpaqueDrawCallsLabel:setText(format('%d draw calls', numOpaqueDrawCalls))
			numTransparentObjectsLabel:setText(format('%d objects', numTransparentObjects))
			numTransparentDrawCallsLabel:setText(format('%d draw calls', numTransparentDrawCalls))
		end)
		timer:start(0, true)
	end	
end