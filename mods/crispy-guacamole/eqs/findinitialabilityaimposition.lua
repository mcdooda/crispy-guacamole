local cg = require(Mod.getFilePath 'scripts/cg')

return flat.eqs.template {
    generator = cg.getHostiles,
    tests = {
        flat.eqs.test.distance(
            1,
            flat.eqs.scoring.inverseLinear,
            {
                from = flat.eqs.context.querier,
                to   = flat.eqs.context.item
            }
        ),
        flat.eqs.test.dot(
            1,
            flat.eqs.scoring.linear,
            {
                line1 = flat.eqs.test.dot.forward(flat.eqs.context.querier),
                line2 = flat.eqs.test.dot.lineBetween(flat.eqs.context.querier, flat.eqs.context.item)
            }
        )
    }
}