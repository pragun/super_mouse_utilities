#include "config.h"
#include <functional>
#include <algorithm>


auto array_of_config_key_values = array_of<User_Config_TypeDef>(
	User_Config_TypeDef{
			.application_id = 0,
			.keypad_key = 0,
			.key_down = CFG(NO_REPORT,{}),
			.mouse_movement = CFG(ALTERED_MOUSE_MOVEMENT,
				{.x_factor = 1,
				.y_factor = 1,
				.z_factor = 1, }),
			.short_release = CFG(NO_REPORT,{}),
			.long_release = CFG(NO_REPORT,{}),
	},
	User_Config_TypeDef{
			.application_id = 0,
			.keypad_key = 1,
			.key_down = CFG(KEYBOARD_PRESS_RELEASE,
				{.modifier_keys = 0x00,
				 .keys = {65,0,0}
				}),
			.mouse_movement = CFG(ALTERED_MOUSE_MOVEMENT,
				{.x_factor = 2,
				.y_factor = 2,
				.z_factor = 2, }),
			.short_release = CFG(KEYBOARD_PRESS_RELEASE,
				{.modifier_keys = 0x02,
				 .keys = {65,0,0}
				}),
			.long_release = CFG(NO_REPORT,{}),
	});


std::array<Key_Size_Value_For_Flash, array_of_config_key_values.size()> config_ksv_objs;

/*
User_Config_TypeDef list_of_config_key_values[] =
{
		{
				.application_id = 0,
				.keypad_key = 0,
				.key_down = CFG(NO_REPORT,{}),
				.mouse_movement = CFG(ALTERED_MOUSE_MOVEMENT,
					{.x_factor = 1,
					.y_factor = 1,
					.z_factor = 1, }),
				.short_release = CFG(NO_REPORT,{}),
				.long_release = CFG(NO_REPORT,{}),
		},
};
*/