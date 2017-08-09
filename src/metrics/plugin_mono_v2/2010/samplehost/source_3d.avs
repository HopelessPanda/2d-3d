left = ImageSource("/images/avengers_50838_l.png", start=0, end=1, fps=24, use_DevIL=False, info=False, pixel_type="RGB24")
right = ImageSource("/images/avengers_50838_r.png", start=0, end=1, fps=24, use_DevIL=False, info=False, pixel_type="RGB24")
return StackHorizontal(left,right).lanczosresize(960*2,544)