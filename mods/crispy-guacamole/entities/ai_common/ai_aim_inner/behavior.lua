local states = {}

function states:init(entity)
    coroutine.yield()
    local instigator = entity:getInstigator()
    assert(instigator)
    while true do
        if not instigator:isValid() or not instigator:isAlive() then
            entity:despawn()
            break
        end
        coroutine.yield()
    end
end

return states