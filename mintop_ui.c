#include "mintop_ui.h"
#include "gfx/gfx.h"
#include "gfx/gfx_2d.h"
#include "memory/arena.h"
#include "os/os.h"

static MT_StateMachine state_machine = {0};


MT_Window 
MT_CreateWindow(const char* title, V2F position, V2F size, GFX_Font font)
{
    MT_Window window = {0};
    window.window_base = GFX_CreateRect(size, position);
    window.thickness = 4;
    window.window_base_outline = GFX_CreateRect(v2(window.window_base.size.x + window.thickness,window.window_base.size.y + window.thickness),  
    v2(
      (window.window_base.pos.x) - (window.thickness * 0.5), 
      (window.window_base.pos.y)- (window.thickness * 0.5)
    ));


    window.window_header = GFX_CreateRect(v2(window.window_base.size.x, 30), position);
    
    
    GFX_SetRectColor(&window.window_header,v4(50,54,53,255));
    GFX_SetRectColor(&window.window_base_outline,v4(120,120,120,255));
    GFX_SetRectColor(&window.window_base,v4(27,27,27,255));
   
    window.header_text_size = font.size;
    window.header_text_size = window.header_text_size;
    window.window_title = GFX_CreateText(
        title,
        v2((window.window_header.pos.x + 10), (window.window_header.pos.y +window.header_text_size*0.5) + (window.window_header.size.y/4)),
   font);
   return window;
}

void 
MT_UpdateWindow(MT_Window* window, OS_App* app, GFX_Renderer* renderer)
{

    if (UI_PointInRect(OS_GetMousePosition(app), window->window_header) && !window->window_nav_mode)
    {
      window->window_gradient_outline = 30;
      window->window_nav_clicked_position = OS_GetMousePosition(app);
      window->window_nav_mode = OS_GetLeftMouseState().clicked;
      window->window_drag_offset =  v2( OS_GetMousePosition(app).x - window->window_header.pos.x, OS_GetMousePosition(app).y - window->window_header.pos.y);
    } else if (!window->window_nav_mode) window->window_gradient_outline = 0;
          GFX_SetRectColor(&window->window_base_outline,v4(25 +  window->window_gradient_outline,25+ window->window_gradient_outline,25+ window->window_gradient_outline,150));
    if (OS_GetLeftMouseState().released && window->window_nav_mode) window->window_nav_mode = 0;

    if (window->window_nav_mode)
    {
      V2F win_mouse_pos = OS_GetMousePosition(app); 
      V2F window_nav_new_pos = v2(
        win_mouse_pos.x - window->window_drag_offset.x,
        win_mouse_pos.y - window->window_drag_offset.y
      );
      window->window_header.pos = v3(window_nav_new_pos.x, window_nav_new_pos.y,0);
      window->window_base.pos = window->window_header.pos;
      window->window_base_outline.pos = v3(
      (window->window_base.pos.x) - (window->thickness * 0.5), 
      (window->window_base.pos.y)- (window->thickness * 0.5), 0
      );
      window->window_title.pos =  v2((window->window_header.pos.x + 10), (window->window_header.pos.y +window->header_text_size*0.5) + (window->window_header.size.y/4));
    }
}
void 
MT_DrawWindow(MT_Window* window, GFX_Renderer* renderer)
{

    GFX_DrawRect(&window->window_base_outline, renderer);
    GFX_DrawRect(&window->window_base, renderer);
    GFX_DrawRect(&window->window_header, renderer);
    GFX_DrawText(&window->window_title, renderer);

}


// Scrollbar

MT_HSlider MT_CreateHSlider(V2F position, V2F size, char* text, GFX_Font font)
{
    MT_HSlider sbar = {0};
    sbar.scroller_thickness = 1;
    sbar.scroller_base = GFX_CreateRect(size, position);

   

    sbar.scroller_base_outline = GFX_CreateRect(v2(sbar.scroller_base.size.x + sbar.scroller_thickness,sbar.scroller_base.size.y + sbar.scroller_thickness),  
    v2(
      (sbar.scroller_base.pos.x) - (sbar.scroller_thickness * 0.5), 
      (sbar.scroller_base.pos.y) - (sbar.scroller_thickness * 0.5)
    ));
     

  i32 scroller_top_size = sbar.scroller_base.size.h ;
  sbar.scroller_top = GFX_CreateRect(v2(20,scroller_top_size), v2(sbar.scroller_base.pos.x,sbar.scroller_base.pos.y ));


  GFX_SetRectColor(&sbar.scroller_base_outline, v4(103,103,103,255));
  GFX_SetRectColor(&sbar.scroller_base,v4(40,40,40,255));
 GFX_SetRectColor(&sbar.scroller_top,v4(150,50,50,255));


  sbar.scroller_font_size = font.size;
  sbar.scroller_title = GFX_CreateText(text, v2(
        sbar.scroller_base.pos.x + 5,
      ( sbar.scroller_top.pos.y + (sbar.scroller_top.size.h /2)  + ( sbar.scroller_font_size  * 0.35))
    ), font);
    return sbar;
}

void MT_UpdateHSlider(MT_HSlider* scrollbar, OS_App* app, GFX_Renderer* renderer)
{
    if (UI_PointInRect(OS_GetMousePosition(app), scrollbar->scroller_base))  scrollbar->active = OS_GetLeftMouseState().clicked;
    if (OS_GetLeftMouseState().released && scrollbar->active) scrollbar->active = 0;

    if (scrollbar->active)
    {
        scrollbar->scroller_top.size.w = OS_GetMousePosition(app).x - scrollbar->scroller_base.pos.x ;
            scrollbar->scroller_top.size.w = U_Clamp(scrollbar->scroller_top.size.w, scrollbar->scroller_base.size.w,0);
            // Get numerical value:
            scrollbar->percent = (scrollbar->scroller_top.size.w / scrollbar->scroller_base.size.w);    
    }

}
void MT_DrawHSlider(MT_HSlider* scrollbar, GFX_Renderer* renderer)
{
    GFX_DrawRect(&scrollbar->scroller_base_outline, renderer);
    GFX_DrawRect(&scrollbar->scroller_base,renderer);
    GFX_DrawRect(&scrollbar->scroller_top, renderer);
    GFX_DrawText(&scrollbar->scroller_title, renderer);
}

void MT_MoveHSlider(MT_HSlider* slider, V2F new_pos)
{
    slider->scroller_base.pos = v3(new_pos.x, new_pos.y, 0);
    slider->scroller_base_outline.pos = v3(
      (slider->scroller_base.pos.x) - (slider->scroller_thickness * 0.5), 
      (slider->scroller_base.pos.y) - (slider->scroller_thickness * 0.5), 0
    );
    slider->scroller_base.pos = v3(slider->scroller_base.pos.x,slider->scroller_base.pos.y, 0 );
    slider->scroller_top.pos = v3(slider->scroller_base.pos.x,slider->scroller_base.pos.y , 0);
    slider->scroller_title.pos = v2(
        slider->scroller_base.pos.x + 5,
      ( slider->scroller_top.pos.y + (slider->scroller_top.size.h /2)  + ( slider->scroller_font_size  * 0.35))
    );


   
}
// State-machine

void MT_Init(OS_App* app, GFX_Renderer* renderer)
{
    
    state_machine.scrollbars = malloc(sizeof(MT_HSlider) * 1024);
    state_machine.windows =  malloc(sizeof(MT_Window) * 1024);;
    state_machine.text = malloc(sizeof(GFX_Text) * 1024);
    state_machine.app = app;
    state_machine.renderer = renderer;
}

void MT_PushWindow(char* title, V2F size, V2F pos)
{
  state_machine.widgets_count = 0;
   state_machine.windows[state_machine.window_used] = 
   MT_CreateWindow(title, pos, size, state_machine.font);
   state_machine.active_window = &state_machine.windows[state_machine.window_used++];
   state_machine.window_orgin_pos = v2(state_machine.active_window->window_header.pos.x, state_machine.active_window->window_header.pos.y + 50);
   
}


void MT_SetFont(GFX_Font font)
{
    state_machine.font = font;
}
void MT_PopWindow(void)
{
    state_machine.active_window->total_widgets = state_machine.widgets_count;
    state_machine.widgets_count = 0;
}
float* MT_PushSlider(char* slider_title, r32 min, r32 max)
{

    V2F position = v2(
        state_machine.active_window->window_base.pos.x + 10,
        state_machine.active_window->window_base.pos.y + state_machine.active_window->window_header.size.y + 10 + (state_machine.widgets_count * 33) 

    ); 
    state_machine.scrollbars[state_machine.scrollbar_used]
    = MT_CreateHSlider(position, v2(state_machine.windows->window_base.size.x - 30, 25), slider_title, state_machine.font);
    state_machine.window_orgin_pos.y += 30 ;
    state_machine.scrollbars[state_machine.scrollbar_used].parent_window = state_machine.active_window;
        state_machine.scrollbars[state_machine.scrollbar_used].original_pos = v2(state_machine.scrollbars[state_machine.scrollbar_used].scroller_base.pos.x, state_machine.scrollbars[state_machine.scrollbar_used].scroller_base.pos.y);
    state_machine.scrollbars[state_machine.scrollbar_used].id = state_machine.scrollbar_used;
        state_machine.scrollbar_used += 1;
    state_machine.widgets_count += 1;

    
    return &state_machine.scrollbars[state_machine.scrollbar_used-1].percent;
}

GFX_Text* MT_GetTextSliderPtr(void)
{
    return &state_machine.scrollbars[state_machine.scrollbar_used-1].scroller_title;
}

void MT_Draw(OS_App* app, GFX_Renderer* renderer)
{
     for (int i = 0; i < state_machine.window_used; i++) {
        MT_Window* parent = &state_machine.windows[i];

        MT_UpdateWindow(parent, app, renderer);
        MT_DrawWindow(parent, renderer);

         if (parent->window_nav_mode) {
            int local_slider_index = 0;
            int local_text_index = 0;

             for (int j = 0; j < state_machine.scrollbar_used; j++) {
                if (state_machine.scrollbars[j].parent_window == parent) {
                    V2F slider_pos = v2(
                        parent->window_base.pos.x + 10,
                        parent->window_base.pos.y + parent->window_header.size.y + 10 + (local_slider_index * 33)
                    );
                    MT_MoveHSlider(&state_machine.scrollbars[j], slider_pos);
                    local_slider_index++;
                }
            }

             for (int j = 0; j < state_machine.text_used; j++) {
                if (state_machine.text[j].parent == parent) {
                    V2F text_pos = v2(
                        parent->window_base.pos.x + 10,
                        parent->window_base.pos.y + parent->window_header.size.y + state_machine.font.size + 10 + (local_slider_index + local_text_index) * 33
                    );
                    state_machine.text[j].text.pos = text_pos;
                    local_text_index++;
                }
            }
        }
    }

     for (int i = 0; i < state_machine.scrollbar_used; i++) {
        MT_UpdateHSlider(&state_machine.scrollbars[i], app, renderer);
        MT_DrawHSlider(&state_machine.scrollbars[i], renderer);
    }

     for (int i = 0; i < state_machine.text_used; i++) {
        GFX_DrawText(&state_machine.text[i].text, renderer);
    }
}



MT_Text* MT_PushText(char* text)
{

     V2F position = v2(
        state_machine.active_window->window_base.pos.x + 10,
        state_machine.active_window->window_base.pos.y + state_machine.active_window->window_header.size.y +10+ state_machine.font.size + (state_machine.widgets_count * 33)

    ); 
     state_machine.widgets_count += 1;
    state_machine.text[state_machine.text_used].text = GFX_CreateText(text, position, state_machine.font);
 state_machine.text[state_machine.text_used].parent = state_machine.active_window;
        state_machine.text_used+=1;
    return &state_machine.text[state_machine.text_used - 1];
}