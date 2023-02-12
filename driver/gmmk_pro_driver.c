/* Copyright 2022-2023, Martin Petkovski

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#include <Windows.h>
#include <stdio.h>
#include <Psapi.h>
#include <TlHelp32.h>
#include "hidapi.h"

#define VENDOR_ID 0x320F
#define PRODUCT_ID 0x5044
#define BUFFER_SIZE 256

hid_device* device;

void to_lower_case(wchar_t* str, int len)
{
	for (int i = 0; i < len; i++)
		str[i] = tolower(str[i]);
}

void send_message(char message)
{
	char data[3];
	data[0] = 0x00;
	data[1] = 0x00;
	data[2] = message;
	const int result = hid_write(device, data, 3);
	if (result < 0)
	{
		printf("Failed to send message %d\n", result);
	}
}

const wchar_t* get_clipboard_text(void)
{
	HANDLE clip = NULL;

	if (OpenClipboard(NULL))
	{
		clip = GetClipboardData(CF_UNICODETEXT);
		CloseClipboard();
	}

	return clip;
}

HWND get_foreground_window(void)
{
	return GetForegroundWindow();
}

void get_foreground_window_process_name(wchar_t* title, const int size)
{
	DWORD process_id;
	GetWindowThreadProcessId(get_foreground_window(), &process_id);

	const HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnapshot == INVALID_HANDLE_VALUE)
	{
		return;
	}

	PROCESSENTRY32 pe;
	pe.dwSize = sizeof(PROCESSENTRY32);

	if (Process32First(hSnapshot, &pe))
	{
		do
		{
			if (pe.th32ProcessID == process_id)
			{
				wcsncpy_s(title, size, pe.szExeFile, MAX_PATH);
				CloseHandle(hSnapshot);
				return;
			}
		} while (Process32Next(hSnapshot, &pe));
	}

	CloseHandle(hSnapshot);
}

void open_website(const wchar_t* address)
{
	ShellExecute(NULL, L"open", address, NULL, NULL, SW_SHOWNORMAL);
}

void open_website_with_query(const wchar_t* address)
{
	wchar_t str[MAX_PATH];
	wcsncpy_s(str, sizeof(str) / sizeof(str[0]), address, wcslen(address));
	const wchar_t* clipboard_text = get_clipboard_text();
	wcscat_s(str, MAX_PATH, clipboard_text);
	open_website(str);
}

const wchar_t* substring_search(const wchar_t* str, const wchar_t* sub)
{
	const size_t sub_len = wcslen(sub);
	const size_t str_len = wcslen(str);
	for (size_t i = 0; i < str_len; i++)
	{
		if (!wcsncmp(str + i, sub, sub_len))
		{
			return str + i;
		}
	}
	return NULL;
}

DWORD WINAPI read_thread(LPVOID lpParameter)
{
	hid_set_nonblocking(device, 0);
	unsigned char read_buffer[BUFFER_SIZE];

	while (1)
	{
		memset(read_buffer, 0, BUFFER_SIZE);
		const int res = hid_read(device, read_buffer, BUFFER_SIZE);
		if (res >= 0)
		{
			unsigned char message = read_buffer[2];
			if (message == 0x01)
				open_website(L"https://chat.openai.com/chat");
			else if (message == 0x02)
				open_website(L"www.twitter.com");
			else if (message == 0x03)
				open_website_with_query(L"www.youtube.com/results?search_query=");
			else if (message == 0x04)
				open_website(L"www.instagram.com/direct/t/");
			else if (message == 0x05)
				open_website_with_query(L"www.google.com/search?q=");
		}

		Sleep(20);
	}
}

DWORD WINAPI write_thread(LPVOID lpParameter)
{
	char prev_message = 0x00;
	while (1)
	{
		wchar_t title[MAX_PATH];
		get_foreground_window_process_name(title, sizeof(title));
		to_lower_case(title, MAX_PATH);

		char message = 0x00;
		if (substring_search(title, L"firefox") != NULL)
			message = 0x01;
		else if (substring_search(title, L"devenv") != NULL)
			message = 0x02;
		else if (substring_search(title, L"discord") != NULL)
			message = 0x03;
		else if (substring_search(title, L"explorer") != NULL)
			message = 0x04;

		if (message != prev_message)
		{
			send_message(message);
			prev_message = message;
		}

		Sleep(20);
	}
}

int main(int argc, char* argv[])
{
	printf("Initializing HID device...\n");

	device = hid_open(VENDOR_ID, PRODUCT_ID, NULL);

	if (!device) {
		printf("Unable to open device\n");
		return 1;
	}

	wchar_t manufacturer[100];
	wchar_t product[100];
	hid_get_manufacturer_string(device, manufacturer, 100);
	hid_get_product_string(device, product, 100);
	printf("Using %ls %ls\n", manufacturer, product);

	DWORD dw_thread_id;
	HANDLE thread_handles[2];
	thread_handles[0] = CreateThread(NULL, 0, read_thread, NULL, 0, &dw_thread_id);
	if (thread_handles[0] == NULL)
	{
		printf("Failed to create read thread\n");
		return 1;
	}

	thread_handles[1] = CreateThread(NULL, 0, write_thread, NULL, 0, &dw_thread_id);
	if (thread_handles[1] == NULL)
	{
		printf("Failed to create write thread\n");
		return 1;
	}

	WaitForMultipleObjects(2, thread_handles, TRUE, -1);
	CloseHandle(thread_handles[0]);
	CloseHandle(thread_handles[1]);

	hid_close(device);

	hid_exit();

	return 0;
}

