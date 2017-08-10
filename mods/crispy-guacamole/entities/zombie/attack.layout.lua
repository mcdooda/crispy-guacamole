-- [scriptInput] number ------> number [test1] number ----------------------> operand 1 [multiply] result -----------> number [test2]
--                         /--> string         string     [number] number --> operand 2                           /--> string
--      [string] string --/ /-> impulse       impulse -\                                   [scriptInput] string -/ /-> impulse
--                         /                            \                                                         /
--       [init] impulse --/                              \-------------------------------------------------------/


return {
    {100, -100}, -- scriptInput
    {100, -300}, -- init
    {250, -100}, -- test1
    {100, -200}, -- string
    {550, -100}, -- multiply
    {400, -200}, -- number
    {700, -100}, -- test2
}