local states = {}

function states:init(archer)
	
end

function states:idle(archer)
	
end

function states:onEntityEnteredVisionRange(archer, entity)
	local isHostile = archer:isHostile(entity) or entity:isHostile(archer)
	if isHostile then
		local currentAttackTarget = archer:getAttackTarget()
		if not currentAttackTarget then
			archer:setAttackTarget(entity)
		end
	end
end

return states
