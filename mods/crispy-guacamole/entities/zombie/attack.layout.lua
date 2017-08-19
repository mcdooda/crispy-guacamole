-- [scriptInput] number ------> number [test1] number ----------------------> operand 1 [multiply] result -----------> number [test2]
--                         /--> string         string     [number] number --> operand 2                           /--> string
--      [string] string --/ /-> impulse       impulse -\                                   [scriptInput] string -/ /-> impulse
--                         /                            \                                                         /
--       [init] impulse --/                              \-------------------------------------------------------/


return {
    {100, -100}, -- scriptInput
    {100, -500}, -- init
    {300, -100}, -- test1
    {100, -300}, -- string
    {700, -100}, -- multiply
    {500, -300}, -- number
    {900, -100}, -- test2
}