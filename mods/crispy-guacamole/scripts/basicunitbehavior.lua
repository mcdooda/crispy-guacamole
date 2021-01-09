local function basicUnit()
    local followDistance = 2

    local states = {}

    function states:init(entity)
        local extraData = entity:getExtraData()
        extraData.currentLoopingState = nil
    end

    function states:followPlayer(entity)
        local extraData = entity:getExtraData()
        extraData.currentLoopingState = 'followPlayer'

        entity:clearPath()
        entity:setAttackTarget(nil)

        local playerToFollow = assert(extraData.playerToFollow)
        entity:setSpeed(playerToFollow:getSpeed())
        while true do
            if playerToFollow:isValid() then
                local entityPosition = entity:getPosition():toVector2()

                local playerToFollowPosition = playerToFollow:getPosition():toVector2()
                if playerToFollow:isFollowingPath() then
                    local playerMovementDirection = playerToFollow:getCurrentMovementDirection()
                    local hit, endPosition = Map.navigationRaycast(playerToFollowPosition, playerMovementDirection, followDistance + entity:getRadius(), 0.5, Map.Navigability.GROUND)
                    if hit then
                        playerToFollowPosition = endPosition - playerMovementDirection * entity:getRadius()
                    else
                        playerToFollowPosition = endPosition
                    end
                end

                if (entityPosition - playerToFollowPosition):length2() > followDistance * followDistance then
                    entity:moveTo(playerToFollowPosition)
                else
                    coroutine.yield()
                end
            else
                return
            end
        end
    end

    function states:combat(entity)
        local extraData = entity:getExtraData()
        extraData.currentLoopingState = 'combat'

        local lastTargetTime = game.getTime()
        local stayInCombatDuration = 3

        while true do
            local attackTarget = self:findAttackTarget(entity)
            local currentTime = game.getTime()
            if not attackTarget or not attackTarget:isValid() then
                if lastTargetTime + stayInCombatDuration < currentTime then
                    break
                end
            else
                lastTargetTime = currentTime
                entity:setAttackTarget(attackTarget)

                if not entity:isInAttackRange(attackTarget) then
                    local moveToPosition = self:findCombatMoveToPosition(entity)
                    entity:moveTo(moveToPosition)
                end
            end
            coroutine.yield()
        end
        
        return 'followPlayer'
    end

    function states:findAttackTarget(entity)
        local findAttackTargetEQSTemplate = require(Mod.getFilePath 'eqs/findattacktarget')
        local targetItem = flat.eqs.run(findAttackTargetEQSTemplate, entity, flat.eqs.RunMode.SingleBestItem)
        if targetItem then
            return targetItem.item
        end
    end

    function states:findCombatMoveToPosition(entity)
        local findCombatMoveToPositionEQSTemplate = require(Mod.getFilePath 'eqs/findcombatmovetoposition')
        local positionItem = flat.eqs.run(findCombatMoveToPositionEQSTemplate, entity, flat.eqs.RunMode.SingleBestItem)
        assert(positionItem)
        return positionItem.item
    end

    function states:onEntityEnteredVisionRange(entity, otherEntity)
        if entity:getExtraData().currentLoopingState == 'combat' then
            local currentAttackTarget = entity:getAttackTarget()
            if not currentAttackTarget or not currentAttackTarget:isValid() then
                local newAttackTarget = self:findAttackTarget(entity)
                if newAttackTarget and newAttackTarget:isValid() then
                    entity:setAttackTarget(newAttackTarget) 
                end
            end
        end
    end

    function states:useAbility(entity)

    end

	return states
end

return {
    basicUnit = basicUnit
}