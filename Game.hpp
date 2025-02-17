#pragma once
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include <thread>

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
		HOVER,
		CHECKEDHOVER
	};

	static wxColour GetCellTextColour(int mines);

	static wxColour GetCellColour(CELLSTATUS colour);

	int16_t cols_;
	int16_t rows_;
	int16_t mines_;
	Board board_;

	std::atomic<bool> playing_ = false;
	bool over_ = false;
	bool win_ = false;

	wxPanel* panel_;
	std::vector<wxButton*> buttons_{};
	wxStaticText* minesLeftLabel_;
	wxStaticText* timeLabel_;

	std::thread timer_;
	std::atomic<int> time_;

	int revealedCells_{};
	int checkedCells_{};

	static std::tuple<int16_t, int16_t> getCellCoordinatesById(int id, int16_t cols);
	static int getCellIndexByCoordinates(int16_t x, int16_t y, int16_t cols);
	static int getCellIdByCoordinates(int16_t x, int16_t y, int16_t cols);

	void explode(const wxColour& colour) const;
	void startTimer();
	void end(bool win);
public:
	Game(wxPanel* panel, int16_t cols, int16_t rows, int16_t mines);
	~Game();

	void Start();
	std::tuple<bool, bool, int> Status() const;
};
