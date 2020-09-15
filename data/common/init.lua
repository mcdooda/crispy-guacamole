-- load script nodes
flat.graph.loadNodeClasses('script', 'data')
flat.graph.loadCompounds('script', Mod.getFilePath 'compounds')
flat.graph.loadCompounds('script', 'data/compounds')

-- load sound nodes
flat.graph.loadNodeClasses('sound', 'data')

flat.graph.loadNodeClasses('statemachine', 'data')

flat.registerUniqueObjectGetter(
    'CG.Entity',
    function(object)
        return object:getExtraData()
    end
)