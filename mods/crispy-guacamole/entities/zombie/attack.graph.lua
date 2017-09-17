script {
    nodes = {
        {name = 'script/scriptinput', initArguments = {{PinTypes.NUMBER, 'Number'}, {PinTypes.STRING, 'String'}}},  -- 1
        {name = 'init'},                                                                                     -- 2
        {name = 'test'},                                                                                     -- 3
        {name = 'string', initArguments = {'Hello world from saved script'}},                                -- 4
        {name = 'math/multiply'},                                                                                 -- 5
        {name = 'number', initArguments = {2}},                                                              -- 6
        {name = 'test'}                                                                                      -- 7
    },
    links = {
        {1, 1, 3, 2}, -- scriptInput number -> test1 number
        {4, 1, 3, 3}, --      string string -> test1 string
        {2, 1, 3, 4}, --       init impulse -> test1 impulse
        {3, 2, 5, 1}, --       test1 number -> multiply operand 1
        {6, 1, 5, 2}, --      number number -> multiply operand 2
        {5, 1, 7, 2}, --    multiply result -> test2 number
        {1, 2, 7, 3}, -- scriptInput string -> test2 string
        {3, 4, 7, 4}, --      test1 impulse -> test2 impulse
    }
}