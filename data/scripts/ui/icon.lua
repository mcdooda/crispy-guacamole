local UiSettings = require 'data/scripts/ui/uisettings'

local Icon = {}
Icon.__index = Icon

local icons = {
    reset = "a",
    trash = "b",
    leaf = "c",
    ["warning-sign"] = "d",
    silhouette = "e",
    hide = "f",
    see = "g",
    house = "h",
    mouse = "i",
    remove = "j",
    brush = "k",
    cone = "l",
    music = "m",
    alert = "n",
    palette = "o",
    change = "p",
    select = "q",
    aperture = "r",
    cog = "s",
    maximize = "t",
    minimize = "u",
    hashtag = "v",
    stack = "w",
    X = "x",
    snow = "y",
    refresh = "z",
    menu = "A",
    left = "B",
    right = "C",
    down = "D",
    checkbox = "E",
    copy = "F",
    circle = "G",
    code = "H",
    cogs = "I",
    copyFile = "J",
    ["alert-circle"] = "K",
    new = "L",
    ["open-folder"] = "M",
    ["ok-circle"] = "N",
    ok = "O",
    picture = "P",
    play = "Q",
    pause = "R",
    remove = "S",
    ["remove-circle"] = "T",
    search = "U",
    shield = "V",
    save = "W",
    next = "X",
    previous = "Y",
    sound = "Z",
    ["sound-plus"] = "1",
    ["star-empty"] = "2",
    star = "3",
    ["sound-less"] = "0"
}


function Icon:new(icon, size)
    size = size or 14

    local container = Widget.makeText(icons[icon], UiSettings.glyphFont, size)
    local o = setmetatable({
        container             = container,
    }, self)

    return o
end



return Icon