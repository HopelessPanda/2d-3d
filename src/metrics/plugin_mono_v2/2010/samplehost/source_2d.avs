mono = ImageSource("/images/avengers_50838_m.png", start=0, end=1, fps=24, use_DevIL=False, info=False, pixel_type="RGB24")
return mono.AddBorders(0, 0, mono.width, 0).LanczosResize(960*2,544)