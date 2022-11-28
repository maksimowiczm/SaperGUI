#pragma once
#include "ConsoleApp.hpp"

#include <exception>
#include <Windows.h>
#include "ConsoleMenu.hpp"

std::tuple<int16_t, int16_t> konsola::ConsoleApp::configureConsole()
{
	HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_CURSOR_INFO cursorInfo;
	GetConsoleCursorInfo(out, &cursorInfo);
	cursorInfo.bVisible = false; // set the cursor visibility
	SetConsoleCursorInfo(out, &cursorInfo);

	CONSOLE_FONT_INFOEX cfi;
	cfi.cbSize = sizeof(cfi);
	cfi.nFont = 0;
	cfi.dwFontSize.X = 0;                   // Width of each character in the font
	cfi.dwFontSize.Y = 24;                  // Height
	cfi.FontFamily = FF_DONTCARE;
	cfi.FontWeight = FW_BOLD;
	wcscpy_s(cfi.FaceName, L"Consolas"); // Choose your font
	SetCurrentConsoleFontEx(out, FALSE, &cfi);

	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);

	return { csbi.srWindow.Right - csbi.srWindow.Left + 1, csbi.srWindow.Bottom - csbi.srWindow.Top + 1 };
}

void konsola::ConsoleApp::run()
{
	const auto [columns, rows] = configureConsole();

	try
	{
		auto menu = ConsoleMenu(columns, rows);
		menu.Start();
	}
	catch (std::exception& ex)
	{
		throw ex;
	}
}
