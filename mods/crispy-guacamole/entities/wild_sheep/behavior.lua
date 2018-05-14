local BehaviorHelper = require 'data/scripts/componenthelpers/behavior'

local sqrt = math.sqrt
local random = math.random

local movement = require 'mods/crispy-guacamole/entities/wild_sheep/movement'

local states = {}

function states:init(sheep)
	sheep:enterState 'wander'
end

function states:idle(sheep)
	sheep:jump()
end

function states:wander(sheep)
	sheep:setDefaultMoveAnimation()
	sheep:setSpeed(movement.speed)

	local initialPosition = sheep:getPosition()
	local initialPosition2d = initialPosition:toVector2()
	while true do

		do
			local pathPoint = initialPosition2d + flat.Vector2((random() * 2 - 1) * 2, (random() * 2 - 1) * 2)
			sheep:moveTo(pathPoint)
		end

		do
			while random() < 0.3 do
				sheep:playAnimation 'graze'
			end
		end

		do
			if random() < 0.1 then
				sheep:playAnimation 'crouch'
				local numLoops = random(3, 6)
				sheep:playAnimation('sleep', numLoops)
				sheep:playAnimation 'standup'
			end
		end
	end
end

function states:flee(sheep)
	sheep:setMoveAnimation 'run'
	sheep:setSpeed(movement.runSpeed)
	
	local data = sheep:getExtraData()
	local fleeTarget = data.fleeTarget
	assert(fleeTarget)
	local endFleeTime = data.endFleeTime
	local distance = 0.5
	local lastKnownPosition2d = fleeTarget:getPosition():toVector2()
	while game.getTime() < endFleeTime or fleeTarget:isValid() and sheep:canSee(fleeTarget) do
		local position2d = sheep:getPosition():toVector2()
		local fleeTargetPosition2d
		if fleeTarget:isValid() then
			fleeTargetPosition2d = fleeTarget:getPosition():toVector2()
			lastKnownPosition2d = fleeTargetPosition2d
		else
			fleeTargetPosition2d = lastKnownPosition2d
		end
		local pathPoint = position2d + (position2d - fleeTargetPosition2d):getNormalized() * distance
		sheep:moveTo(pathPoint)
	end
	data.fleeTarget = nil
	sheep:enterState 'wander'
end

function states:onEntityEnteredVisionRange(sheep, entity)
	local isHostile = sheep:isHostile(entity)
	local data = sheep:getExtraData()
	if isHostile then
		if not data.fleeTarget or not data.fleeTarget:isValid() then
			data.fleeTarget = entity
		elseif data.fleeTarget ~= entity then
			local sheepPosition2d = sheep:getPosition():toVector2()
			local entityPositon2d = entity:getPosition():toVector2()
			local fleeTargetPosition2d = data.fleeTarget:getPosition():toVector2()
			local entityDistance2 = (entityPositon2d - sheepPosition2d):length2()
			local targetDistance2 = (entityPositon2d - fleeTargetPosition2d):length2()
			if entityDistance2 < targetDistance2 then
				data.fleeTarget = entity
			end
		end
		data.endFleeTime = game.getTime() + 5
		sheep:clearPath()
		return 'flee'
	else
		local entityData = entity:getExtraData()
		if entityData.fleeTarget and entityData.fleeTarget:isValid() then
			data.fleeTarget = entityData.fleeTarget
			data.endFleeTime = entityData.endFleeTime
			sheep:clearPath()
			return 'flee'
		end
	end
end

return states