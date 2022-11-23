#pragma once
#include <wx/wxprec.h>

#include "Game.hpp"
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif


class MyFrame : public wxFrame
{
private:
	enum BUTTONS
	{
		START
	};

	wxPanel* panel_;
	wxStaticText* logo_;
	Game* game_;
	wxSizer* grid_;

	using MenuItem = std::tuple<std::wstring, std::function<void(wxMouseEvent& e)>>;
	std::vector<MenuItem> activeMenu_;
	std::vector<MenuItem> menu_;
	std::vector<MenuItem> difficultyMenu_;
	std::vector<MenuItem> gameOverMenu_;

	std::vector<std::tuple<int16_t, int16_t, int16_t>> levels_{
		{8, 8, 10},
		{16, 16, 40},
		{30, 16, 99}
	};

	void startGame(std::tuple<int16_t, int16_t, int16_t> level);

public:
	MyFrame();
	~MyFrame() override;

	void Menu();
	void UpdateMenu();
};
