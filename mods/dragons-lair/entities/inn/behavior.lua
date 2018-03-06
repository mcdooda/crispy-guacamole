local states = {}

local quests = {
	
}

function states:init(inn)
	inn:click(function(inn) 			
		inn:setUiVisible(true)
	end)
	inn:deselected(function(inn) 			
		inn:setUiVisible(false)
	end)
end

function states:idle(inn)	
end

return states
