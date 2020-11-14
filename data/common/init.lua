-- script
flat.graph.loadNodeClasses('script', 'data')
flat.graph.loadNodeClasses('script', Mod.getPath())
flat.graph.loadCompounds('script', Mod.getFilePath 'compounds')
flat.graph.loadCompounds('script', 'data/compounds')

-- sound
flat.graph.loadNodeClasses('sound', 'data')
flat.graph.loadCompounds('sound', Mod.getFilePath 'sounds')

-- state machine
flat.graph.loadNodeClasses('statemachine', 'data')

flat.registerUniqueObjectGetter(
    'CG.Entity',
    function(object)
        return object:getExtraData()
    end
)