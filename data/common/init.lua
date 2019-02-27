flat.graph.loadNodeClasses('script', 'data')

flat.registerUniqueObjectGetter(
    'CG.Entity',
    function(object)
        return object:getExtraData()
    end
)