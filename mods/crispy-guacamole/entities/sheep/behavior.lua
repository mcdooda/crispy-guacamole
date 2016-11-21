local math = math
local random = math.random

local movement = require 'mods/crispy-guacamole/entities/sheep/movement'

local function sleep(duration)
	local getTime = Time.getTime
	local endTime = getTime() + duration
	local yield = coroutine.yield
	while getTime() < endTime do
		yield()
	end
end

local states = {}
local inputs = {}

function states:init(sheep)
	sheep:enterState 'wander'
	--sheep:playAnimation 'graze'
end

function states:idle(sheep)
	sheep:jump()
end

function states:wander(sheep)
	sheep:setDefaultMoveAnimation()
	sheep:setSpeed(movement.speed)
	local x, y = sheep:getPosition()
	while true do

		do
			local rx = x + (random() * 2 - 1) * 2
			local ry = y + (random() * 2 - 1) * 2
			sheep:moveTo(rx, ry)
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
	local endFleeTime = data.endFleeTime
	local distance = 0.5
	local lastKnownX, lastKnownY = fleeTarget:getPosition()
	while Time.getTime() < endFleeTime or fleeTarget:isValid() and sheep:canSee(fleeTarget) do
		local x, y = sheep:getPosition()
		local fx, fy
		if fleeTarget:isValid() then
			fx, fy = fleeTarget:getPosition()
			lastKnownX, lastKnownY = fx, fy
		else
			fx, fy = lastKnownX, lastKnownY
		end
		local dx, dy = x - fx, y - fy
		local d = math.sqrt(dx * dx + dy * dy)
		local rx, ry = x + (dx / d) * distance, y + (dy / d) * distance
		sheep:moveTo(rx, ry)
	end
	data.fleeTarget = nil
	sheep:enterState 'wander'
end

function states:onEntityEnteredVisionRange(sheep, entity)
	local isHostile = entity:getTemplateName() ~= 'sheep'
	local data = sheep:getExtraData()
	if isHostile then
		if not data.fleeTarget or not data.fleeTarget:isValid() then
			data.fleeTarget = entity
		elseif data.fleeTarget ~= entity then
			local x, y = sheep:getPosition()
			local ex, ey = entity:getPosition()
			local tx, ty = data.fleeTarget:getPosition()
			local entityDistance2 = (ex - x) * (ex - x) + (ey - y) * (ey - y)
			local targetDistance2 = (tx - x) * (tx - x) + (ty - y) * (ty - y)
			if entityDistance2 < targetDistance2 then
				data.fleeTarget = entity
			end
		end
		data.endFleeTime = Time.getTime() + 5
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