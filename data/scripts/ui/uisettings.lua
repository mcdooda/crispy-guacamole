return {
    defaultFont = {'data/misc/fonts/Lato-Medium.ttf', 12},
    buttonFont = {'data/misc/fonts/Lato-Medium.ttf', 14},
    menuButtonFont = {'data/misc/fonts/LiberationSans-Regular.ttf', 12},
    titleFont = {'data/misc/fonts/gnuolane rg.ttf', 25},
    glyphFont = 'data/misc/fonts/Glyphter.ttf',
    customFont = 'data/misc/fonts/customFont.ttf',
    fontIcons = {'Glyphter', 'customFont'},
    addFontIcon = function(fontIcon)
        self.fontIcons[#self.fontIcons + 1] = fontIcon
    end
}