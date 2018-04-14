local states = {}

local function isValidAttackTarget(tower, target)
	-- tower hostile towards target?
	return target:hasComponent(Component.life) and target:isHostile(tower)
end

local function isBetterAttackTarget(tower, target)
	local attackTarget = tower:getAttackTarget()
	if not attackTarget then
		return true
	end

	if not tower:canSee(attackTarget) then
		return true
	end

	if not tower:isInAttackRange(attackTarget)
	   and tower:isInAttackRange(target) then
	   return true
	end

	return false
end

function states:init(tower)
	
end

function states:idle(tower)
	
end

function states:onEntityEnteredVisionRange(tower, entity)
	if isValidAttackTarget(tower, entity) and isBetterAttackTarget(tower, entity) then
		tower:setAttackTarget(entity)
	end
end

function states:onEntityLeftVisionRange(tower, entity)
	if entity == tower:getAttackTarget() then
		local towerPosition2d = tower:getPosition():toVector2()
		local closestEntity
		local closestEntityDistance2 = math.huge

		-- find closest visible valid attack target
		for _, visibleEntity in tower:eachVisibleEntity() do
			if isValidAttackTarget(tower, visibleEntity) then
				local visibleEntityPosition2d = visibleEntity:getPosition():toVector2()
				local distance2 = (visibleEntityPosition2d - towerPosition2d):length2()
				if distance2 < closestEntityDistance2 then
					closestEntityDistance2 = distance2
					closestEntity = visibleEntity
				end
			end
		end

		if closestEntity then
			tower:setAttackTarget(closestEntity)
		end
	end
end

return states
