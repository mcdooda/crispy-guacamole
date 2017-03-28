local states = {}

local function isValidAttackTarget(archer, target)
	return target:hasComponent(Component.life)
		and (archer:isHostile(target) or target:isHostile(archer))
end

local function isBetterAttackTarget(archer, target)
	local attackTarget = archer:getAttackTarget()
	if not attackTarget then
		return true
	end

	if not archer:canSee(attackTarget) then
		return true
	end

	if not archer:isInAttackRange(attackTarget)
	   and archer:isInAttackRange(target) then
	   return true
	end

	return false
end

function states:init(archer)
	
end

function states:idle(archer)
	
end

function states:onEntityEnteredVisionRange(archer, entity)
	if isValidAttackTarget(archer, entity) and isBetterAttackTarget(archer, entity) then
		archer:setAttackTarget(entity)
	end
end

function states:onEntityLeftVisionRange(archer, entity)
	if entity == archer:getAttackTarget() then
		local archerX, archerY = archer:getPosition()
		local closestEntity
		local closestEntityDistance2 = math.huge

		-- find closest visible valid attack target
		for _, visibleEntity in archer:eachVisibleEntity() do
			if isValidAttackTarget(archer, visibleEntity) then
				local visibleEntityX, visibleEntityY = visibleEntity:getPosition()
				local distance2 = (visibleEntityX - archerX) * (visibleEntityX - archerX) + (visibleEntityY - archerY) * (visibleEntityY - archerY)
				if distance2 < closestEntityDistance2 then
					closestEntityDistance2 = distance2
					closestEntity = visibleEntity
				end
			end
		end

		if closestEntity then
			archer:setAttackTarget(closestEntity)
		end
	end
end

return states
