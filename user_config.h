#include "config.h"
#include "usb_hid_keys.h"
#include <functional>
#include <algorithm>

#define CFG(X, ...) C(Reporting_Func_Params_Typedef<ReportingFunctionEnum::X>__VA_ARGS__)

#define KBRD_PRS_RLS(...) Reporting_Func_Params_Typedef<ReportingFunctionEnum::KEYBOARD_PRESS_RELEASE>__VA_ARGS__

#define BIG_MOD_DIVISOR 40
#define SMALL_MOD_DIVISOR 10

#define FUSION_360_ID 1
#define FUSION_360_NAME "fusion360.exe"

auto array_of_config_key_values = array_of<User_Config_TypeDef>(
	User_Config_TypeDef{
		.application_id = 0, .keypad_key = 0,
		.key_down = CFG(NO_REPORT,{}),
		.mouse_movement = CFG(ALTERED_MOUSE_MOVEMENT,{.x_factor = 1, .y_factor = 1, .z_factor = 1, }),
		.short_release = CFG(NO_REPORT,{}),
		.long_release = CFG(NO_REPORT,{}),
	}

	,User_Config_TypeDef{
   .application_id = 0, .keypad_key = 1,
   .key_down = CFG(MODIFIER_HOLD_PRESS_RELEASE,{.modifier_hold_state = KEY_MOD_RALT, .press_release = {.modifier_keys = 0x00, .keys = {KEY_TAB,0,0}}}),
   .mouse_movement = CFG(MOTION_MOD_KEY_PRESS_RELEASE, {.x_divisor = BIG_MOD_DIVISOR, .y_divisor = BIG_MOD_DIVISOR, .z_divisor = 1,
		   .x_movement_keys = {
		   KBRD_PRS_RLS({.modifier_keys = 0x00, .keys = {KEY_LEFT,0,0} }),
		   KBRD_PRS_RLS({.modifier_keys = 0x00, .keys = {KEY_RIGHT,0,0} })},
		   .y_movement_keys = {
		   KBRD_PRS_RLS({.modifier_keys = 0x00, .keys = {KEY_UP,0,0} }),
		   KBRD_PRS_RLS({.modifier_keys = 0x00, .keys = {KEY_DOWN,0,0} })},
		   .z_movement_keys = {
		   KBRD_PRS_RLS({.modifier_keys = 0x00, .keys = {KEY_ENTER,0,0} }),
		   KBRD_PRS_RLS({.modifier_keys = 0x00, .keys = {KEY_ESC,0,0} })}, }),
   .short_release = CFG(NO_REPORT,{}),
   .long_release = CFG(NO_REPORT,{}),
	}

	,User_Config_TypeDef{
	.application_id = 0, .keypad_key = 2,
	.key_down = CFG(NO_REPORT,{}),
	.mouse_movement = CFG(MOTION_MOD_KEY_PRESS_RELEASE, {.x_divisor = BIG_MOD_DIVISOR, .y_divisor = BIG_MOD_DIVISOR, .z_divisor = 1,
			.x_movement_keys = {
			KBRD_PRS_RLS({.modifier_keys = KEY_MOD_RCTRL, .keys = {KEY_X,0,0} }),
			KBRD_PRS_RLS({.modifier_keys = KEY_MOD_RCTRL, .keys = {KEY_C,0,0} })},
			.y_movement_keys = {
			KBRD_PRS_RLS({.modifier_keys = KEY_MOD_RCTRL, .keys = {KEY_V,0,0} }),
			KBRD_PRS_RLS({.modifier_keys = KEY_MOD_RMETA, .keys = {KEY_V,0,0} })},
			.z_movement_keys = {
			KBRD_PRS_RLS({.modifier_keys = KEY_MOD_RCTRL, .keys = {KEY_Z,0,0} }),
			KBRD_PRS_RLS({.modifier_keys = KEY_MOD_RCTRL, .keys = {KEY_Y,0,0} })}, }),
	.short_release = CFG(NO_REPORT,{}),
	.long_release = CFG(NO_REPORT,{}),
	}

	,User_Config_TypeDef{
   .application_id = 0, .keypad_key = 3,
   .key_down = CFG(MODIFIER_HOLD_PRESS_RELEASE,{.modifier_hold_state = KEY_MOD_RCTRL, .press_release = {.modifier_keys = 0x00, .keys = {0,0,0}}}),
   .mouse_movement = CFG(ALTERED_MOUSE_MOVEMENT,{.x_factor = 2, .y_factor = 2, .z_factor = 2, }),
		.short_release = CFG(NO_REPORT,{}),
		.long_release = CFG(NO_REPORT,{}),
	}

	, User_Config_TypeDef{
	.application_id = 0, .keypad_key = 4,
	.key_down = CFG(NO_REPORT,{}),
	.mouse_movement = CFG(MOTION_MOD_KEY_PRESS_RELEASE, {.x_divisor = BIG_MOD_DIVISOR, .y_divisor = BIG_MOD_DIVISOR, .z_divisor = 1,
			.x_movement_keys = {
			KBRD_PRS_RLS({.modifier_keys = KEY_MOD_RMETA, .keys = {KEY_LEFT,0,0} }),
			KBRD_PRS_RLS({.modifier_keys = KEY_MOD_RMETA, .keys = {KEY_RIGHT,0,0} })},
			.y_movement_keys = {
			KBRD_PRS_RLS({.modifier_keys = KEY_MOD_RMETA, .keys = {KEY_UP,0,0} }),
			KBRD_PRS_RLS({.modifier_keys = KEY_MOD_RMETA, .keys = {KEY_DOWN,0,0} })},
			.z_movement_keys = {
			KBRD_PRS_RLS({.modifier_keys = KEY_MOD_RMETA, .keys = {KEY_TAB,0,0} }),
			KBRD_PRS_RLS({.modifier_keys = KEY_MOD_RMETA, .keys = {KEY_TAB,0,0} })}, }),
	.short_release = CFG(NO_REPORT,{}),
	.long_release = CFG(NO_REPORT,{}),
	}

	, User_Config_TypeDef{
		.application_id = 0, .keypad_key = 5,
		.key_down = CFG(NO_REPORT,{}),
		.mouse_movement = CFG(ALTERED_MOUSE_MOVEMENT,{.x_factor = 2, .y_factor = 2, .z_factor = 2, }),
		.short_release = CFG(NO_REPORT,{}),
		.long_release = CFG(NO_REPORT,{}),
	}

	, User_Config_TypeDef{
	.application_id = 0, .keypad_key = 6,
	.key_down = CFG(NO_REPORT,{}),
	.mouse_movement = CFG(MOTION_MOD_KEY_PRESS_RELEASE, {.x_divisor = BIG_MOD_DIVISOR, .y_divisor = BIG_MOD_DIVISOR, .z_divisor = 1,
			.x_movement_keys = {
			KBRD_PRS_RLS({.modifier_keys = KEY_MOD_RCTRL, .keys = {KEY_PAGEUP,0,0} }),
			KBRD_PRS_RLS({.modifier_keys = KEY_MOD_RCTRL, .keys = {KEY_PAGEDOWN,0,0} })},
			.y_movement_keys = {
			KBRD_PRS_RLS({.modifier_keys = KEY_MOD_RCTRL | KEY_MOD_RSHIFT, .keys = {KEY_PAGEUP,0,0} }),
			KBRD_PRS_RLS({.modifier_keys = KEY_MOD_RCTRL | KEY_MOD_RSHIFT, .keys = {KEY_PAGEDOWN,0,0} })},
			.z_movement_keys = {
			KBRD_PRS_RLS({.modifier_keys = KEY_MOD_RCTRL, .keys = {KEY_HOME,0,0} }),
			KBRD_PRS_RLS({.modifier_keys = KEY_MOD_RCTRL, .keys = {KEY_END,0,0} })}, }),
	.short_release = CFG(NO_REPORT,{}),
	.long_release = CFG(NO_REPORT,{}),
	}

	, User_Config_TypeDef{
		.application_id = 0, .keypad_key = 7,
		.key_down = CFG(NO_REPORT,{}),
		.mouse_movement = CFG(ALTERED_MOUSE_MOVEMENT,{.x_factor = 1, .y_factor = 1, .z_factor = 1, }),
		.short_release = CFG(NO_REPORT,{}),
		.long_release = CFG(NO_REPORT,{}),
	}

	, User_Config_TypeDef{
		.application_id = 0, .keypad_key = 8,
		.key_down = CFG(NO_REPORT,{}),
		.mouse_movement = CFG(ALTERED_MOUSE_MOVEMENT,{.x_factor = 1, .y_factor = 1, .z_factor = 1, }),
		.short_release = CFG(NO_REPORT,{}),
		.long_release = CFG(NO_REPORT,{}),
	}

	, User_Config_TypeDef{
		.application_id = 0, .keypad_key = 9,
		.key_down = CFG(NO_REPORT,{}),
		.mouse_movement = CFG(ALTERED_MOUSE_MOVEMENT,{.x_factor = 1, .y_factor = 1, .z_factor = 1, }),
		.short_release = CFG(NO_REPORT,{}),
		.long_release = CFG(NO_REPORT,{}),
	}

	, User_Config_TypeDef{
		.application_id = 0, .keypad_key = 10,
		.key_down = CFG(NO_REPORT,{}),
		.mouse_movement = CFG(ALTERED_MOUSE_MOVEMENT,{.x_factor = 1, .y_factor = 1, .z_factor = 1, }),
		.short_release = CFG(NO_REPORT,{}),
		.long_release = CFG(NO_REPORT,{}),
	}

	, User_Config_TypeDef{
		.application_id = 0, .keypad_key = 11,
		.key_down = CFG(NO_REPORT,{}),
		.mouse_movement = CFG(ALTERED_MOUSE_MOVEMENT,{.x_factor = 1, .y_factor = 1, .z_factor = 1, }),
		.short_release = CFG(NO_REPORT,{}),
		.long_release = CFG(NO_REPORT,{}),
	}

	, User_Config_TypeDef{
		.application_id = 0, .keypad_key = 12,
		.key_down = CFG(NO_REPORT,{}),
		.mouse_movement = CFG(ALTERED_MOUSE_MOVEMENT,{.x_factor = 1, .y_factor = 1, .z_factor = 1, }),
		.short_release = CFG(NO_REPORT,{}),
		.long_release = CFG(NO_REPORT,{}),
	}

	,User_Config_TypeDef{
		.application_id = 0, .keypad_key = RING_BUTTON_KEYPAD_IDX,
		.key_down = CFG(NO_REPORT,{}),
		.mouse_movement = CFG(MOTION_MOD_KEY_PRESS_RELEASE, {.x_divisor = SMALL_MOD_DIVISOR, .y_divisor = SMALL_MOD_DIVISOR, .z_divisor = 1,
			.x_movement_keys = {
			KBRD_PRS_RLS({.modifier_keys = 0x00, .keys = {KEY_LEFT,0,0} }),
			KBRD_PRS_RLS({.modifier_keys = 0x00, .keys = {KEY_RIGHT,0,0} })},
			.y_movement_keys = {
			KBRD_PRS_RLS({.modifier_keys = 0x00, .keys = {KEY_UP,0,0} }),
			KBRD_PRS_RLS({.modifier_keys = 0x00, .keys = {KEY_DOWN,0,0} })},
			.z_movement_keys = {
			KBRD_PRS_RLS({.modifier_keys = 0x00, .keys = {KEY_ENTER,0,0} }),
			KBRD_PRS_RLS({.modifier_keys = 0x00, .keys = {KEY_ESC,0,0} })}, }),
		.short_release = CFG(NO_REPORT,{}),
		.long_release = CFG(NO_REPORT,{}),
	}

	,User_Config_TypeDef{
		.application_id = 0, .keypad_key = SCROLL_BUTTON_KEYPAD_IDX,
		.key_down = CFG(NO_REPORT,{}),
		.mouse_movement = CFG(MOTION_AS_SCROLL, {.x_divisor = 4, .y_divisor = 4}),
		.short_release = CFG(NO_REPORT,{}),
		.long_release = CFG(NO_REPORT,{}),
	}

);


std::array<Key_Size_Value_For_Flash, array_of_config_key_values.size()> config_ksv_objs;

