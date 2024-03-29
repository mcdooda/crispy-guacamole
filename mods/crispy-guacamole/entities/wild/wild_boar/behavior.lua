local random = math.random

local states = {}

function states:init(boar)
	return 'wander'
end

function states:idle(boar)
	boar:jump()
end

function states:wander(boar)
	local initialPosition = boar:getPosition()
	local initialPosition2d = initialPosition:toVector2()
	while true do
		local pathPoint = initialPosition2d + flat.Vector2((random() * 2 - 1) * 2, (random() * 2 - 1) * 2)
		boar:moveTo(pathPoint)
	end
end

function states:onDamageTaken(boar, amount, instigator)
	--print(amount, instigator)
end

return states
