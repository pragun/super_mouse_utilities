#include <stdio.h>
#include <cstdint>
#include "reporting_functions.h"

constexpr uint8_t CFG_ENTRY_SERIALIZATION_SIZE_LIMIT = 61;

struct Cfg_Serialized_Entry_TypeDef {
	uint8_t report_type;
	uint8_t cfg_size;
	uint8_t data[CFG_ENTRY_SERIALIZATION_SIZE_LIMIT];
};

struct CFG_EVT_Serialized_TypeDef {
	uint8_t size;
	uint8_t data[CFG_ENTRY_SERIALIZATION_SIZE_LIMIT];
};

struct User_Config_TypeDef {
	uint8_t application_id;
	uint8_t keypad_key;

	CFG_EVT_Serialized_TypeDef key_down;
	CFG_EVT_Serialized_TypeDef mouse_movement;
	CFG_EVT_Serialized_TypeDef short_release;
	CFG_EVT_Serialized_TypeDef long_release;
};

template <typename V, typename... T>
constexpr auto array_of(T&&... t)
->std::array < V, sizeof...(T) >
{
	return { { std::forward<T>(t)... } };
}

template <ReportingFunctionEnum report_type>
CFG_EVT_Serialized_TypeDef
C(Reporting_Func_Params_Typedef<report_type> obj) {
	CFG_EVT_Serialized_TypeDef a;
	uint8_t siz = (uint8_t)sizeof(Reporting_Func_Params_Typedef<report_type>);

	a.data[0] = (uint8_t)report_type;
	a.data[1] = siz;

	uint8_t* ptr = reinterpret_cast<uint8_t*>(&obj);
	for (uint8_t i = 0; i < siz; i++) {
		a.data[i + 2] = ptr[i];
	}

	a.size = siz + 2;

	return a;
}

struct Key_Size_Value_For_Flash {
	uint32_t key;
	uint8_t size;
	uint8_t value[CFG_ENTRY_SERIALIZATION_SIZE_LIMIT];
};

#define CFG(X, ...) C(Reporting_Func_Params_Typedef<ReportingFunctionEnum::X>__VA_ARGS__)


Key_Size_Value_For_Flash convert_cfg_entry_to_key_size_value(User_Config_TypeDef cfg) {
	Key_Size_Value_For_Flash ksvf_obj;
	ksvf_obj.key = (cfg.application_id << 8) | (cfg.keypad_key);

	constexpr uint8_t evt_table_length = 4;
	uint8_t evt_offset_table[evt_table_length];

	evt_offset_table[0] = 0;
	evt_offset_table[1] = evt_offset_table[0] + cfg.key_down.size;
	evt_offset_table[2] = evt_offset_table[1] + cfg.mouse_movement.size;
	evt_offset_table[3] = evt_offset_table[2] + cfg.short_release.size;
	ksvf_obj.size = evt_offset_table[3] + cfg.long_release.size;

	uint8_t* write_ptr = &ksvf_obj.value[0];

	for (uint8_t i = 0; i < evt_table_length; i++) {
		*write_ptr = evt_offset_table[i];
		write_ptr++;
	}

	for (uint8_t i = 0; i < cfg.key_down.size; i++) {
		*write_ptr = cfg.key_down.data[i];
		write_ptr++;
	}

	for (uint8_t i = 0; i < cfg.mouse_movement.size; i++) {
		*write_ptr = cfg.mouse_movement.data[i];
		write_ptr++;
	}

	for (uint8_t i = 0; i < cfg.short_release.size; i++) {
		*write_ptr = cfg.short_release.data[i];
		write_ptr++;
	}

	for (uint8_t i = 0; i < cfg.long_release.size; i++) {
		*write_ptr = cfg.long_release.data[i];
		write_ptr++;
	}

	return ksvf_obj;
}

