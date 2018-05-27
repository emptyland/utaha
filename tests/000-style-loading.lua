local style = utaha.styleCollection

style
    :addFont("font.normal", "res/fonts/wqy-zenhei.ttf", 20)
    :addFont("font.small", "res/fonts/wqy-zenhei.ttf", 12)
    :addFont("font.big", "res/fonts/wqy-zenhei.ttf", 20)

style
    :setFont("test.font", "font.normal")
    :setColor("test.3", 0xff010200)
    :setSize("test.2", 100, 200)
    :setInt("test.1", 1)
    :setFloat("test.4", 1.1001)

