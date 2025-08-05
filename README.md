# mintop 

'mintop' is an immediate mode GUI library written in C99 which is currently in development. So far, it can support sliders, and text objects.

This is based off IMGUI and Casey Muratori blog on immediate-mode GUIs: https://caseymuratori.com/blog_0001

It's current defacto renderer is 'torn,' a custom framework that abstracts away all the rendering and os-specific code. 


<img width="518" height="270" alt="mintop windowing" src="https://github.com/user-attachments/assets/ccace4ec-4b50-4ec3-bad5-763e49e1d16f" />


The code for this is:
```c
 MT_Init(app, renderer);
 MT_SetFont(GFX_CreateFont("font_path.ttf", 20));
 MT_PushWindow("Color Selection", v2(500,250), v2(200,100));
 {
   float* red = MT_PushSlider("Red Component", 0, 255);
   GFX_Text* tred = MT_GetTextSliderPtr();
   float* green = MT_PushSlider("Green Component", 0, 255);
   GFX_Text* tgreen = MT_GetTextSliderPtr();
   float* blue = MT_PushSlider("Blue Component", 0, 255);
   GFX_Text* tblue = MT_GetTextSliderPtr();
   GFX_Text* text = MT_PushText("Color");
 }
 MT_PopWindow();

// in your windowing loop
MT_Draw(app, renderer);
```


Here's an example of multiple windows with varying widget attributes!

<img width="1232" height="699" alt="image" src="https://github.com/user-attachments/assets/66155997-7687-4d43-b64b-e125afc705d2" />

