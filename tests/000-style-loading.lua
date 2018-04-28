local style = utaha.styleCollection

style
    :addFont("font.normal", "assets/fonts/default.ttf", 20)
    :addFont("font.small", "assets/fonts/default.ttf", 12)
    :addFont("font.big", "assets/fonts/default.ttf", 20)

style
    :setFont("test.font", "font.normal")
    :setColor("test.3", 0xff010200)
    :setSize("test.2", 100, 200)
    :setInt("test.1", 1)
    :setFloat("test.4", 1.1001)

