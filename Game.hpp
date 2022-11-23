#pragma once
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <atomic>
#include "Logika/Board.hpp"

class Game
{
private:
	constexpr static int firstButtonId = 1000;

	enum class CELLSTATUS
	{
		HIDDEN,
		REVEALED,
		CHECKED,
		BLOWN,
		WIN,
	};

	static wxColour GetCellTextColour(int mines);

	static wxColour GetCellColour(CELLSTATUS colour);

	Board* board_;
	wxPanel* panel_;
	std::vector<wxButton*> buttons_{};
	int16_t* cols_;
	int16_t rows_;

	std::atomic<bool> playing_;
	int revealed_cells_ = 0;

	static std::tuple<int16_t, int16_t> getCellCoordiatesById(int id, int16_t cols);
	static int getCellIdByCoordiates(int16_t x, int16_t y, int16_t cols);
	void explode();
public:
	Game(wxPanel* panel, int16_t cols, int16_t rows, int16_t mines);
	int Start();
	~Game();
};
