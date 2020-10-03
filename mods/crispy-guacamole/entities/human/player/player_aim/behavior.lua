local states = {}

function states:init(aim)
    if not aim:getInstigator() then
        return
    end
    while true do
        --aim:setPosition(aim:getInstigator():getPosition())
        coroutine.yield()
    end
end

return states