local cg = require(Mod.getFilePath 'scripts/cg')

return flat.eqs.template {
    generator = cg.generatePositionsAroundAttackTarget,
    tests = {
        flat.eqs.test.distance(
            1,
            flat.eqs.scoring.inverseLinear,
            {
                from = flat.eqs.context.querier,
                to   = flat.eqs.context.item,
            }
        )
    }
}