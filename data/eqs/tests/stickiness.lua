local Stickiness = flat.eqs.test.Test:inherit 'Stickiness'
Stickiness.cost = flat.eqs.Cost.CONSTANT

function Stickiness:computeItemScore(queryInstance, item)
    local querier = queryInstance:getQuerier()
    local itemIsCurrentAttackTarget = item == querier:getAttackTarget()
    return itemIsCurrentAttackTarget and 1 or 0
end

game.eqs.test.stickiness = Stickiness