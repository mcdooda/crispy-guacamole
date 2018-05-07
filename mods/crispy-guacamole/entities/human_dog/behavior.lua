local BehaviorHelper = require 'data/scripts/componenthelpers/behavior'

local sqrt = math.sqrt
local random = math.random

local movement = require 'mods/crispy-guacamole/entities/human_dog/movement'

local states = {}

function states:init(dog)
	dog:enterState 'wander'
end

function states:idle(dog)
	dog:jump()
end

function states:wander(dog)
	dog:setDefaultMoveAnimation()
	dog:setSpeed(movement.speed)

	local initialPosition = dog:getPosition()
	local initialPosition2d = initialPosition:toVector2()
	while true do

		do
			local pathPoint = initialPosition2d + flat.Vector2((random() * 2 - 1) * 2, (random() * 2 - 1) * 2)
			dog:moveTo(pathPoint)
		end

		do
			while random() < 0.1 do
				dog:playAnimation 'idle'
			end
		end

		do
			if random() < 0.3 then
				while random() < 0.9 do
					dog:playAnimation 'sit'
				end
			end
		end

	end
end

function states:followTarget(dog)
	dog:setMoveAnimation 'run'
	dog:setSpeed(movement.speed * 3)

	local target = dog:getExtraData().target
	while target:isValid() do
		local position = dog:getPosition()
		local direction = (target:getPosition() - position):getNormalized()
		dog:moveTo((position + direction):toVector2())
		if not target:isValid() or (dog:getPosition() - target:getPosition()):length2() < 1 then
			break
		end
	end
	dog:getExtraData().target = nil

	while random() < 0.9 do
		dog:playAnimation 'sit'
	end

	dog:enterState 'wander'
end

function states:onEntityEnteredVisionRange(dog, entity)
	if entity:getTemplateName() == 'wild_sheep' and (not dog:getExtraData().target or not dog:getExtraData().target:isValid()) then
		dog:getExtraData().target = entity
		return 'followTarget'
	end
end

return states