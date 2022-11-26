#pragma once
#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include "Game.hpp"

#include <future>

class MyFrame : public wxFrame
{
private:
	wxPanel* panel_;
	Game* game_ = nullptr;
	wxSizer* grid_;

	std::atomic<bool> end_ = false;

	using MenuItem = std::tuple<bool, std::wstring, std::function<void(wxMouseEvent& e)>>;
	std::vector<MenuItem> activeMenu_;
	std::vector<MenuItem> menu_;
	std::vector<MenuItem> difficultyMenu_;
	std::vector<MenuItem> gameOverMenu_;

	using level_t = std::tuple<int16_t, int16_t, int16_t>;
	std::vector<level_t> levels_{
		{8, 8, 10},
		{16, 16, 40},
		{30, 16, 99}
	};

	void endGame(bool win, int time);
	void displayLogo() const;

public:
	void startGame(const level_t& level);
	MyFrame();
	~MyFrame() override;

	void Menu();
	void UpdateMenu(bool clear, bool logo);
};
