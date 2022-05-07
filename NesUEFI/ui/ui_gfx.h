#ifndef UI_GFX_H
#define UI_GFX_H

#include <stdbool.h>

typedef enum {
	UI_STATE_SPLASH,
	UI_STATE_MENU,
	UI_STATE_LOAD_ROM,
	UI_STATE_PLAY
} ui_state_t;

void render_menu(UINT16 key);
void render_splash_screen();
int populate_menu_items();
ui_state_t ui_manage_states();
void ui_key_handler(uint32_t key);

#endif