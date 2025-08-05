#ifndef MINTOP_UI
#define MINTOP_UI
#include "torn.h"

typedef struct MT_Window MT_Window;
struct MT_Window
{
    GFX_Rect window_base;
    GFX_Rect window_base_outline;
    i32 thickness;
    GFX_Rect window_header;
    GFX_Text window_title;


    int gradient;
    i32 window_gradient_outline;
    i32 header_text_size;
    b32 window_nav_mode;
    V2F window_drag_offset;
    V2F window_nav_clicked_position;


    int total_widgets;
    

};
typedef struct MT_Text MT_Text;
struct MT_Text{
    GFX_Text text;
    MT_Window* parent;
};

MT_Window MT_CreateWindow(const char* title, V2F position, V2F size, GFX_Font font);
void MT_UpdateWindow(MT_Window* window, OS_App* app, GFX_Renderer* renderer);
void MT_DrawWindow(MT_Window* window, GFX_Renderer* renderer);


typedef struct MT_HSlider MT_HSlider;
struct MT_HSlider
{
    GFX_Rect scroller_base;
    GFX_Rect scroller_base_outline;
    int scroller_thickness;
    GFX_Rect scroller_top;
    GFX_Text scroller_title;
    MT_Window* parent_window;
    int scroller_font_size;
    int active;
    r32 percent;
    i32 id;
    V2F original_pos;
};

MT_HSlider MT_CreateHSlider(V2F position, V2F size, char* text, GFX_Font font);

void MT_UpdateHSlider(MT_HSlider* scrollbar, OS_App* app, GFX_Renderer* renderer);
void MT_DrawHSlider(MT_HSlider* scrollbar, GFX_Renderer* renderer);
void MT_MoveHSlider(MT_HSlider* slider, V2F new_pos);

typedef struct MT_StateMachine MT_StateMachine;
struct MT_StateMachine
{
    MT_Window* windows;
    MT_HSlider* scrollbars;
    int window_used, scrollbar_used, text_used;
    GFX_Font font;
    MT_Text* text;
    MEM_Arena arena;
    MT_Window* active_window;
    V2F window_orgin_pos;
    r32 widgets_count;
    OS_App* app;
    GFX_Renderer* renderer;
};

void MT_Init(OS_App* app, GFX_Renderer* renderer);
void MT_PushWindow(char* title, V2F size, V2F pos);
void MT_SetFont(GFX_Font font);
float*  MT_PushSlider(char* slider_title, r32 min, r32 max);
void MT_PopWindow(void);
GFX_Text* MT_GetTextSliderPtr(void);
void MT_Draw(OS_App* app, GFX_Renderer* renderer);

MT_Text* MT_PushText(char* text);


#endif