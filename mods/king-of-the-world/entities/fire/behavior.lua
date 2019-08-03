local states = {}

function states:init(fire)
    fire:playAnimation('Fire', -1, false)
    fire:incComponentDisableLevel(Component.movement)
end


function states:updatePosition(fire)
    local extraData = fire:getExtraData()
    local endTime = game.getTime() + 5
    local entity = extraData.attachedEntity or nil
    while game.getTime() < endTime do
        if entity and entity:isValid() then
            if entity:isLiving() then
                entity:dealDamage(1)
            end
            local betterPosition = entity:getPosition()
            betterPosition:x(betterPosition:x() + 0.6)
            betterPosition:y(betterPosition:y() + 0.6)
            betterPosition:z(betterPosition:z() + 0.6)
            fire:setPosition(entity:getPosition())
        else
            return 'burnOut'
        end
        coroutine.yield()
    end
    fire:despawn()
end

function states:burnOut(fire)
    local endTime = game.getTime() + 0.8
    while game.getTime() < endTime do
        fire:playAnimation('Fire', -1, false)
        coroutine.yield()
    end
    fire:despawn()
end

return states