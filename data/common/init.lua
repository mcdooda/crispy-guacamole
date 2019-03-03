flat.graph.loadNodeClasses('script', 'data')
flat.graph.loadCompounds('script', Mod.getPath() .. '/compounds')

flat.registerUniqueObjectGetter(
    'CG.Entity',
    function(object)
        return object:getExtraData()
    end
)