#include "Game.hpp"

#include <future>

Game::Game(wxPanel* panel, const int16_t cols, const int16_t rows, const int16_t mines,
           std::function<void(bool win, int time)>* end) : end_(end), panel_(panel)
{
	//const auto grid = new wxBoxSizer(wxVERTICAL);
	auto grid = panel->GetSizer();
	board_ = new Board(cols, rows, mines);
	cols_ = new int16_t(cols);
	rows_ = new int16_t(rows);
	mines_ = new int16_t(mines);
	headerMutex_ = new std::mutex();

	{
		const auto row = new wxGridSizer(5);
		timeLabel_ = new wxStaticText(panel_, wxID_ANY, "0");
		minesLeftLabel_ = new wxStaticText(panel_, wxID_ANY, std::to_string(mines), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT);
		timeLabel_->SetForegroundColour(wxColour(*wxWHITE));
		minesLeftLabel_->SetForegroundColour(wxColour(*wxWHITE));

		row->AddSpacer(1);
		row->Add(timeLabel_, 0, wxEXPAND);
		row->AddSpacer(1);
		row->Add(minesLeftLabel_, 0, wxEXPAND);
		row->AddSpacer(1);
		grid->Add(row, 0, wxEXPAND);
	}

	for (int i = 0; i < rows; i++)
	{
		const auto row = new wxBoxSizer(wxHORIZONTAL);

		for (int j = 0; j < cols; j++)
		{
			auto btn = new wxButton(panel_, firstButtonId + buttons_.size(), "", wxDefaultPosition, wxSize(32, 32), wxBORDER_NONE);

			btn->SetOwnBackgroundColour(wxColour(50, 50, 50, 255));

			buttons_.push_back(btn);
			row->Add(btn, 0, wxALL, 1);
		}

		grid->Add(row, 0);
	}

	panel_->SetSizerAndFit(grid);
	grid->Layout();
}

Game::~Game()
{
	delete board_;
	delete checkedCells_;

	playing_ = false;
	if (timer_->joinable())
		timer_->join();
}

void Game::Start()
{
	// RIGHT CLICK
	const std::function<void(wxMouseEvent& e)> rightClick = [this](const wxMouseEvent& e)
	{
		if (!playing_)
			return;

		const auto btn = dynamic_cast<wxButton*>(e.GetEventObject());

		const auto [x, y] = getCellCoordiatesById(e.GetId(), *cols_);
		const auto& cell = board_->board_[y][x];
		if (cell.isRevealed)
			return;
		if (!cell.isChecked && *checkedCells_ + 1 > *mines_)
			return;

		if (board_->CheckSwitch(x, y))
		{
			btn->SetBackgroundColour(GetCellColour(CELLSTATUS::CHECKED));
			minesLeftLabel_->SetLabel(std::to_string(*mines_ - ++*checkedCells_));
		}
		else
		{
			btn->SetBackgroundColour(GetCellColour(CELLSTATUS::HIDDEN));
			minesLeftLabel_->SetLabel(std::to_string(*mines_ - --*checkedCells_));
		}

		headerMutex_->lock();
		panel_->Layout();
		headerMutex_->unlock();
	};

	// LEFT CLICK
	const std::function<void(wxMouseEvent& e)> leftClick = [this](const wxMouseEvent& e)
	{
		if (revealedCells_ == 0)
		{
			playing_ = true;

			timer_ = new std::thread([this]
			{
				while (playing_)
				{
					timeLabel_->SetLabel(std::to_string(time_++));
					headerMutex_->lock();
					panel_->Layout();
					headerMutex_->unlock();
					std::this_thread::sleep_for(std::chrono::seconds(1));
				}
			});
		}

		if (!playing_)
			return;

		const auto [x, y] = getCellCoordiatesById(e.GetId(), *cols_);
		const auto cells = board_->RevealCell(x, y);
		revealedCells_ += cells.size();

		for (const auto& cell : cells)
		{
			if (cell.isMine)
			{
				End(false);
				return;
			}

			const auto index = getCellIdByCoordiates(cell.x, cell.y, *cols_);
			buttons_[index]->SetBackgroundColour(GetCellColour(CELLSTATUS::REVEALED));

			if (cell.minesAround > 0)
			{
				buttons_[index]->SetLabel(std::to_string(cell.minesAround));
				buttons_[index]->SetForegroundColour(GetCellTextColour(cell.minesAround));
			}
		}

		if (revealedCells_ == *cols_ * *rows_ - *mines_)
			End(true);
	};

	for (const auto& btn : buttons_)
	{
		btn->Bind(wxEVT_RIGHT_UP, rightClick);
		btn->Bind(wxEVT_LEFT_UP, leftClick);
	}
}

std::tuple<int16_t, int16_t> Game::getCellCoordiatesById(const int id, const int16_t cols)
{
	int col = (id - firstButtonId) % cols;
	int tempId = id - firstButtonId;
	int row = 0;
	while (tempId >= 0)
	{
		tempId -= cols;
		row++;
	}

	return {col, row - 1};
}

int Game::getCellIdByCoordiates(const int16_t x, const int16_t y, const int16_t cols)
{
	return y * cols + x;
}

wxColour Game::GetCellTextColour(const int mines)
{
	switch (mines)
	{
	case 1:
		return {0, 0, 255, 255};
	case 2:
		return {255, 0, 0, 255};
	case 3:
		return {0, 255, 0, 255};
	case 4:
		return {0, 0, 128, 255};
	case 5:
		return {128, 0, 0, 255};
	case 6:
		return {0, 128, 128, 255};
	case 7:
		return {0, 0, 0, 255};
	case 8:
		return {128, 128, 128, 255};
	default:
		throw std::runtime_error("GetCellTextColour");
	}
}

wxColour Game::GetCellColour(const CELLSTATUS colour)
{
	switch (colour)
	{
	case CELLSTATUS::HIDDEN:
		return {50, 50, 50, 255};
	case CELLSTATUS::CHECKED:
		return {255, 0, 0, 255};
	case CELLSTATUS::REVEALED:
		return {100, 100, 100, 255};
	case CELLSTATUS::BLOWN:
		return {200, 200, 200, 255};
	case CELLSTATUS::WIN:
		return {0, 255, 0, 255};
	default:
		throw std::runtime_error("GetCellColour");
	}
}

void Game::explode()
{
	const auto mines = board_->allMines_;

	for (const auto& mines : mines)
	{
		buttons_[getCellIdByCoordiates(mines.x, mines.y, *cols_)]->SetBackgroundColour(GetCellColour(CELLSTATUS::BLOWN));
	}

	//panel_->Layout();
}

void Game::End(const bool win)
{
	playing_ = false;

	if (!win)
	{
		auto future = std::async(std::launch::async, [this]
		{
			explode();
		});
	}


	//std::this_thread::sleep_for(std::chrono::seconds(5));
	timer_->join();
	(*end_)(win, time_);
}
