local probabilities = {
    10,
    10,
    100,
    100,
    100,
    100,
    100,
    100,
    100,
    100,
    100,
    100,
    100,
    100,
    100,
    100,
    100,
    100,
    100,
    100,
    100,
    100,
    100,
    100,
    100,
    100,
    100,
    100,
    100,
    100,
    100,
    100,
    100,
    100,
    100,
    100,
    100,
    100,
    100,
    100,
    100,
    100,
    100,
    100,
    100,
    100,
    100,
    100,
    100,
    100,
}



local idToTile = {}
idToTile[0] = {48}
idToTile[2] =   {2}
idToTile[8] =   {3}
idToTile[10] =  {4}
idToTile[11] =  {5}
idToTile[16] =  {6}
idToTile[18] =  {7}
idToTile[22] =  {8}
idToTile[24] =  {9}
idToTile[26] =  {10}
idToTile[27] =  {11}
idToTile[30] =  {12}
idToTile[31] =  {13}
idToTile[64] =  {14}
idToTile[66] =  {15}
idToTile[72] =  {16}
idToTile[74] =  {17}
idToTile[75] =  {18}
idToTile[80] =  {19}
idToTile[82] =  {20}
idToTile[86] =  {21}
idToTile[88] =  {22}
idToTile[90] =  {23}
idToTile[91] =  {24}
idToTile[94] =  {25}
idToTile[95] =  {26}
idToTile[104] = {27}
idToTile[106] = {28}
idToTile[107] = {29}
idToTile[120] = {30}
idToTile[122] = {31}
idToTile[123] = {32}
idToTile[126] = {33}
idToTile[127] = {34}
idToTile[208] = {35}
idToTile[210] = {36}
idToTile[214] = {37}
idToTile[216] = {38}
idToTile[218] = {39}
idToTile[219] = {40}
idToTile[222] = {41}
idToTile[223] = {42}
idToTile[248] = {43}
idToTile[250] = {44}
idToTile[251] = {45}
idToTile[254] = {46}
idToTile[255] = {47}

--          1
--       8     2
--    32          4
--      64     16
--         128



local function selectTile(tl, tr, bl, br, t, r, b, l)
    local caseValue = 0
    if b == "creep" and br == "creep" and bl == "creep" then
        caseValue = caseValue + 128
    end
    if l == "creep" and bl == "creep" and tl == "creep" then
        caseValue = caseValue + 32
    end
    if r == "creep" and br == "creep" and tr == "creep" then
        caseValue = caseValue + 4
    end
    if t == "creep" and tr == "creep" and tl == "creep" then
        caseValue = caseValue + 1
    end
    if tr == "creep" then
        caseValue = caseValue + 2
    end
    if tl == "creep" then
        caseValue = caseValue + 8
    end
    if br == "creep" then
        caseValue = caseValue + 16
    end
    if bl == "creep" then
        caseValue = caseValue + 64
    end
    if caseValue == 255 then
        if math.random(0,1000) > 800 then
            return {48 + math.random(1, 2)}
        end
    end
    return idToTile[caseValue]
end


return {
    probabilities = probabilities ,
    frameDuration = 0.4,
    numFrames = 1,
    selectTile = selectTile
}