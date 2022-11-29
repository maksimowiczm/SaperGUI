#pragma once
#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include "Game.hpp"


class MyFrame : public wxFrame
{
private:
	const wxSize DEFAULTSIZE = {1280, 180};
	const wxColour LOGOCOLOR = {186, 24, 27, 255};
	const wxColour BACKGROUNDCOLOR = {20, 33, 61, 255};
	const wxColour BUTTONCOLOR = {255, 255, 255, 255};
	const wxColour BUTTONHOVERCOLOR = {252, 163, 17, 255};

	wxPanel* panel_;
	Game* game_ = nullptr;
	wxSizer* grid_;

	std::atomic<bool> end_ = false;

	using MenuItem = std::tuple<bool, std::wstring, std::function<void(wxMouseEvent& e)>>;
	std::vector<MenuItem> activeMenu_;
	std::vector<MenuItem> menu_;
	std::vector<MenuItem> difficultyMenu_;

	using level_t = std::tuple<int16_t, int16_t, int16_t>;
	std::vector<level_t> levels_{
		{8, 8, 10},
		{16, 16, 40},
		{30, 16, 99}
	};

	void startGame(const level_t& level);
	void endGame(bool win, int time);

	void displayLogo() const;
	std::vector<wxButton*> UpdateMenu(bool clear, bool logo);

	static void ShowConsole();
	static void HideConsole();

public:
	MyFrame();
	~MyFrame() override;

	void Menu();
};
