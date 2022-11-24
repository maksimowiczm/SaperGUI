#pragma once
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <atomic>
#include <mutex>
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
	};

	static wxColour GetCellTextColour(int mines);

	static wxColour GetCellColour(CELLSTATUS colour);


	std::function<void(bool win, int time)>* end_;
	Board* board_;
	wxPanel* panel_;
	std::vector<wxButton*> buttons_{};
	wxStaticText* minesLeftLabel_;
	int16_t* cols_;
	int16_t* rows_;
	int16_t* mines_;

	std::mutex* headerMutex_;
	std::atomic<bool> playing_ = false;
	std::thread* timer_;
	std::atomic<int> time_;
	wxStaticText* timeLabel_;
	int revealedCells_{};
	int* checkedCells_ = new int(0);

	static std::tuple<int16_t, int16_t> getCellCoordiatesById(int id, int16_t cols);
	static int getCellIdByCoordiates(int16_t x, int16_t y, int16_t cols);
	void explode();
public:
	Game(wxPanel* panel, int16_t cols, int16_t rows, int16_t mines, std::function<void(bool win, int time)>* end);
	void Start();
	void End(bool win);
	~Game();
};
