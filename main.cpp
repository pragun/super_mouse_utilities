#include "sciter-x.h"
#include "sciter-x-window.hpp"
#include "hidapi.h"
#include <stdio.h>
#include "resources.cpp" // resources packaged into binary blob.
#include "user_config.h"
#include "rpc_impl.hpp"
#include <psapi.h>
#include <pybind11/embed.h>

namespace py = pybind11;

PYBIND11_EMBEDDED_MODULE(fast_calc, m) {
	// `m` is a `py::module_` which is used to bind functions and classes
	m.def("add", [](int i, int j) {
		return i + j;
		});
}

#define printf debug_printf
#define STM32_PID 0x572b
#define STM32_VID 0x0483

#define IDT_TIMER1 1
#define IDT_TIMER2 2

int __cdecl debug_printf(const char* format, ...)
{
	char str[2048];

	va_list argptr;
	va_start(argptr, format);
	int ret = vsnprintf(str, sizeof(str), format, argptr);
	va_end(argptr);

	OutputDebugStringA(str);

	return ret;
}

class frame : public sciter::window {
public:
	frame() : window(SW_TITLEBAR | SW_RESIZEABLE | SW_CONTROLS | SW_MAIN | SW_ENABLE_DEBUG) {
	}

	// passport - lists native functions and properties exposed to script:
	SOM_PASSPORT_BEGIN(frame)
		SOM_FUNCS(
			SOM_FUNC(nativeMessage),
			SOM_FUNC(list_hid_devices),
			SOM_FUNC(test_config_read_write),
			SOM_FUNC(write_user_config),
			SOM_FUNC(erase_flash),
			SOM_FUNC(reset_system),
		)
		SOM_PASSPORT_END

		// function expsed to script:
		sciter::string  nativeMessage() { return WSTR("Hello C++ World"); }

	int log_printf(const char* format, ...) {
		char str[2048];
		va_list argptr;
		va_start(argptr, format);
		int ret = vsnprintf(str, sizeof(str), format, argptr);
		va_end(argptr);
		call_function("new_log_entry", std::string(str));
		return ret;
	}

	void timer_called()
	{
		TCHAR buffer[MAX_PATH] = { 0 };
		DWORD dwProcId = 0;

		log_printf("Timer Called..."); 
		HWND foreground_window = GetForegroundWindow();
		if (foreground_window != NULL) {
			GetWindowThreadProcessId(foreground_window, &dwProcId);
			HANDLE hProc = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, dwProcId);
			//GetModuleFileName((HMODULE)hProc, buffer, MAX_PATH);
			DWORD dbytes = GetModuleFileNameExA((HMODULE)hProc, NULL, buffer, MAX_PATH);
			CloseHandle(hProc);
			log_printf("Foreground Window is %s", buffer);
		}
		else {
			log_printf("Foreground Window HWND is NULL");
		}
	}

	
	int list_hid_devices()
	{
		int i;
		struct hid_device_info* devs, * cur_dev;

		if (hid_init())
			return -1;

		devs = hid_enumerate(STM32_VID, STM32_PID);
		cur_dev = devs;
		while (cur_dev) {
			log_printf("Device Found:  type: %04hx %04hx", cur_dev->vendor_id, cur_dev->product_id);
			log_printf("Path: %s  Serial_number: %ls", cur_dev->path, cur_dev->serial_number);
			log_printf("Manufacturer: %ls", cur_dev->manufacturer_string);
			log_printf("Product:      %ls", cur_dev->product_string);
			log_printf("Release:      %hx", cur_dev->release_number);
			log_printf("Interface:    %d", cur_dev->interface_number);
			log_printf("Usage (page): 0x%hx (0x%hx)", cur_dev->usage, cur_dev->usage_page);
			cur_dev = cur_dev->next;
		}
		hid_free_enumeration(devs);

		log_printf("Listed all matching devices...");
		log_printf("------------------------------");
	}

	int test_config_read_write() {
		// Set up the command buffer.
		#define MAX_STR 255
		wchar_t wstr[MAX_STR];
		unsigned char buf[256];
		hid_device* handle;
		int res;

		memset(buf, 0x00, sizeof(buf));
		buf[0] = 0x01;
		buf[1] = 0x81;

		// Open the device using the VID, PID,
		// and optionally the Serial number.
		handle = hid_open_w_usage_page(STM32_VID, STM32_PID, 0x00, 0xFF00);
		if (!handle) {
			log_printf("unable to open device\n");
			return 1;
		}
		log_printf("Opened device successfully...\n");

		// Read the Manufacturer String
		wstr[0] = 0x0000;
		res = hid_get_manufacturer_string(handle, wstr, MAX_STR);
		if (res < 0)
			log_printf("Unable to read manufacturer string\n");
		log_printf("Manufacturer String: %ls\n", wstr);

		// Read the Product String
		wstr[0] = 0x0000;
		res = hid_get_product_string(handle, wstr, MAX_STR);
		if (res < 0)
			log_printf("Unable to read product string\n");
		log_printf("Product String: %ls\n", wstr);

		// Read the Serial Number String
		wstr[0] = 0x0000;
		res = hid_get_serial_number_string(handle, wstr, MAX_STR);
		if (res < 0)
			log_printf("Unable to read serial number string\n");
		log_printf("Serial Number String: (%d) %ls", wstr[0], wstr);
		log_printf("\n");

		// Read Indexed String 1
		wstr[0] = 0x0000;
		res = hid_get_indexed_string(handle, 1, wstr, MAX_STR);
		if (res < 0)
			log_printf("Unable to read indexed string 1\n");
		log_printf("Indexed String 1: %ls\n", wstr);

		/*
		// Set the hid_read() function to be non-blocking.
		hid_set_nonblocking(handle, 1);

		// Try to read from the device. There should be no
		// data here, but execution should not block.
		res = hid_read(handle, buf, 17);
		*/

		// Send a Feature Report to the device

		buf[0] = 0x05;
		buf[1] = 0xa0;
		buf[2] = 0x0a;
		buf[3] = 0x00;
		buf[4] = 0x00;

		res = hid_write(handle, buf, 17);
		if (res < 0) {
			log_printf("Unable to send a feature report.\n");
			log_printf("%ls", hid_error(handle));
		}

		memset(buf, 0, sizeof(buf));
		return 0;
	}

	int write_user_config() {
		std::transform(array_of_config_key_values.begin(), array_of_config_key_values.end(), config_ksv_objs.begin(), convert_cfg_entry_to_key_size_value);
		for (auto s : config_ksv_objs) {
			write_ksv_obj(s);
		}
		return 0;
	}

	int write_ksv_obj(Key_Size_Value_For_Flash& obj) {
		hid_device* handle;

		handle = hid_open_w_usage_page(STM32_VID, STM32_PID, 0x00, 0xFF00);
		if (!handle) {
			log_printf("unable to open device\n");
			return 1;
		}
		log_printf("Opened device successfully...\n");

		unsigned char buf[64];
		memset(buf, 0x00, sizeof(buf));
		buf[0] = 0x05; //report id for HID output Reports
		buf[1] = (unsigned char) RPC_Function_Enum::WRITE_KEY_SIZE_VALUE_TO_FLASH; //cmd id for KSV_write
		memcpy(&buf[2], &obj, obj.size + 5);
		int res = hid_write(handle, buf, obj.size + 7);
		return res;
		}

	int erase_flash() {
		hid_device* handle;

		handle = hid_open_w_usage_page(STM32_VID, STM32_PID, 0x00, 0xFF00);
		if (!handle) {
			log_printf("unable to open device\n");
			return 1;
		}
		log_printf("Opened device successfully...\n");

		unsigned char buf[64];
		memset(buf, 0x00, sizeof(buf));
		buf[0] = 0x05; //report id for HID output Reports
		buf[1] = (unsigned char)RPC_Function_Enum::ERASE_FLASH_SECTOR; //cmd id for KSV_write
		int res = hid_write(handle, buf, 5);
		return res;
	}

	int reset_system() {
		hid_device* handle;

		handle = hid_open_w_usage_page(STM32_VID, STM32_PID, 0x00, 0xFF00);
		if (!handle) {
			log_printf("unable to open device\n");
			return 1;
		}
		log_printf("Opened device successfully...\n");

		unsigned char buf[64];
		memset(buf, 0x00, sizeof(buf));
		buf[0] = 0x05; //report id for HID output Reports
		buf[1] = (unsigned char)RPC_Function_Enum::RESET_SYSTEM; //cmd id for KSV_write
		int res = hid_write(handle, buf, 5);
		return res;
	}

};

HWINDOW* hwnd_ptr;

sciter::om::hasset<frame> frame_ptr;

void CALLBACK Timerproc(
	HWND Arg1,
	UINT Arg2,
	UINT_PTR Arg3,
	DWORD Arg4
) {
	frame_ptr->timer_called();
}

int uimain(std::function<int()> run ) {

	py::scoped_interpreter guard{};

	auto fast_calc = py::module_::import("fast_calc");
	auto result = fast_calc.attr("add")(1, 2).cast<int>();
	assert(result == 3);

	/*Py_Initialize();
	auto a = Py_GetPath();
	PyRun_SimpleString("print('Today is', ctime(time()))\n");
	if (Py_FinalizeEx() < 0) {
		exit(120);
	}
	*/
	//PyMem_RawFree();
	

  sciter::archive::instance().open(aux::elements_of(resources)); // bind resources[] (defined in "resources.cpp") with the archive

  sciter::om::hasset<frame> pwin = new frame();

  // note: this:://app URL is dedicated to the sciter::archive content associated with the application
  pwin->load( WSTR("this://app/main.htm") );
  
  //test_hid();
  HWINDOW hwnd = pwin->get_hwnd();
  hwnd_ptr = &hwnd;
  frame_ptr = pwin;

  SetTimer(hwnd,             // handle to main window 
			IDT_TIMER1,            // timer identifier 
			1000,                 // 10-second interval 
			(TIMERPROC) &Timerproc);     // no timer callback 

  pwin->expand();

  return run();
}

