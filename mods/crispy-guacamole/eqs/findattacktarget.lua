local cg = require(Mod.getFilePath 'scripts/cg')

return flat.eqs.template {
    generator = cg.getHostiles,
    tests = {
        flat.eqs.test.distance(
            2,
            flat.eqs.scoring.inverseLinear,
            {
                from = flat.eqs.context.querier,
                to   = flat.eqs.context.item,
            }
        ),
        game.eqs.test.stickiness(
            1,
            flat.eqs.scoring.inverseLinear
        )
    }
}