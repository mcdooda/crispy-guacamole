local states = {}

function states:init(archer)
	
end

function states:idle(archer)
	
end

function states:onEntityEnteredVisionRange(archer, entity)
	local canDie = entity:hasComponent(Component.life)
	if canDie then
		local isHostile = archer:isHostile(entity) or entity:isHostile(archer)
		if isHostile then
			local currentAttackTarget = archer:getAttackTarget()
			if not currentAttackTarget
			or not archer:canSee(currentAttackTarget)
			or not archer:isInAttackRange(currentAttackTarget) then
				archer:setAttackTarget(entity)
			end
		end
	end
end

function states:onEntityLeftVisionRange(archer, entity)
	if entity == archer:getAttackTarget() then
		-- TODO: find new target + remove archer:canSee(currentAttackTarget)
	end
end

return states
