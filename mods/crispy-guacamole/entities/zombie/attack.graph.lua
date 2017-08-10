script {
    nodes = {
        {'scriptInput', PinTypes.NUMBER, PinTypes.STRING},  -- 1
        'init',                                             -- 2
        'test',                                             -- 3
        {'string', 'Hello world from saved script'},        -- 4
        'multiply',                                         -- 5
        {'number', 2},                                      -- 6
        'test'                                              -- 7
    },
    links = {
        {1, 1, 3, 1}, -- scriptInput number -> test1 number
        {4, 1, 3, 2}, --      string string -> test1 string
        {2, 1, 3, 3}, --       init impulse -> test1 impulse
        {3, 1, 5, 1}, --       test1 number -> multiply operand 1
        {6, 1, 5, 2}, --      number number -> multiply operand 2
        {5, 1, 7, 1}, --    multiply result -> test2 number
        {1, 2, 7, 2}, -- scriptInput string -> test2 string
        {3, 3, 7, 3}, --      test1 impulse -> test2 impulse
    }
}