#include "Game.hpp"


Game::Game(wxPanel* panel, const int16_t cols, const int16_t rows, const int16_t mines) : cols_(cols), rows_(rows), mines_(mines),
                                                                                          board_(cols, rows, mines), panel_(panel)
{
	const auto grid = panel_->GetSizer();
	grid->Clear(true);


	wxFont font;
	font.SetPointSize(20);
	font.SetWeight(wxFONTWEIGHT_BOLD);

	const auto header = new wxGridSizer(5);

	timeLabel_ = new wxStaticText(panel_, wxID_ANY, "0");
	timeLabel_->SetForegroundColour(wxColour(*wxWHITE));
	timeLabel_->SetFont(font);

	minesLeftLabel_ = new wxStaticText(panel_, wxID_ANY, std::to_string(mines), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT);
	minesLeftLabel_->SetForegroundColour(wxColour(*wxWHITE));
	minesLeftLabel_->SetFont(font);

	header->AddSpacer(1);
	header->Add(timeLabel_, 0, wxEXPAND);
	header->AddSpacer(1);
	header->Add(minesLeftLabel_, 0, wxEXPAND);
	header->AddSpacer(1);
	header->Layout();
	grid->Add(header, 0, wxEXPAND);


	font.SetWeight(wxFONTWEIGHT_NORMAL);
	const auto board = new wxGridSizer(rows, cols, {1, 1});
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			auto btn = new wxButton(panel_, getCellIdByCoordinates(j, i, cols), "", wxDefaultPosition, wxSize(50, 50), wxBORDER_NONE);
			btn->SetFont(font);

			btn->SetOwnBackgroundColour({50, 50, 50, 255});

			buttons_.push_back(btn);
			board->Add(btn);
		}
	}
	grid->Add(board);

	panel_->Layout();
	panel_->SetSizerAndFit(grid);
}

Game::~Game()
{
	playing_ = false;
	if (timer_.joinable())
		timer_.join();

	panel_->GetSizer()->Clear(true);
}

void Game::Start()
{
	// RIGHT CLICK
	const std::function rightClick = [this](const wxMouseEvent& e)
	{
		const auto [x, y] = getCellCoordinatesById(e.GetId(), cols_);

		if (!playing_)
			return;

		const auto btn = dynamic_cast<wxButton*>(e.GetEventObject());

		const auto& cell = board_.board_[y][x];
		if (cell.isRevealed)
			return;
		if (!cell.isChecked && checkedCells_ + 1 > mines_)
			return;

		if (board_.CheckSwitch(x, y))
		{
			btn->SetBackgroundColour(GetCellColour(CELLSTATUS::CHECKEDHOVER));
			minesLeftLabel_->SetLabel(std::to_string(mines_ - ++checkedCells_));
		}
		else
		{
			btn->SetBackgroundColour(GetCellColour(CELLSTATUS::HIDDEN));
			minesLeftLabel_->SetLabel(std::to_string(mines_ - --checkedCells_));
		}
		panel_->Layout();
	};

	// LEFT CLICK
	const std::function leftClick = [this](const wxMouseEvent& e)
	{
		const auto [x, y] = getCellCoordinatesById(e.GetId(), cols_);

		if (revealedCells_ == 0)
		{
			playing_ = true;
			startTimer();
		}

		if (!playing_)
			return;

		const auto cells = board_.RevealCell(x, y);
		for (const auto& cell : cells)
		{
			if (cell.isMine)
			{
				end(false);
				return;
			}

			const auto index = getCellIndexByCoordinates(cell.x, cell.y, cols_);
			buttons_[index]->SetBackgroundColour(GetCellColour(CELLSTATUS::REVEALED));

			if (cell.minesAround > 0)
			{
				buttons_[index]->SetLabel(std::to_string(cell.minesAround));
				buttons_[index]->SetForegroundColour(GetCellTextColour(cell.minesAround));
			}
		}

		revealedCells_ += cells.size();
		wxYield();
		if (revealedCells_ == cols_ * rows_ - mines_)
			end(true);
	};

	// HOVER
	const std::function hover = [this](const wxMouseEvent& e)
	{
		const auto [x, y] = getCellCoordinatesById(e.GetId(), cols_);
		const auto& cell = board_.board_[y][x];

		const auto btn = dynamic_cast<wxButton*>(e.GetEventObject());

		if (cell.isChecked)
		{
			btn->SetBackgroundColour(GetCellColour(CELLSTATUS::CHECKEDHOVER));
			return;
		}
		if (!cell.isRevealed)
			btn->SetBackgroundColour(GetCellColour(CELLSTATUS::HOVER));
	};

	const std::function hoverLeft = [this](const wxMouseEvent& e)
	{
		const auto [x, y] = getCellCoordinatesById(e.GetId(), cols_);
		const auto& cell = board_.board_[y][x];
		const auto btn = dynamic_cast<wxButton*>(e.GetEventObject());

		if (cell.isChecked)
		{
			btn->SetBackgroundColour(GetCellColour(CELLSTATUS::CHECKED));
			return;
		}
		if (!cell.isRevealed)
			btn->SetBackgroundColour(GetCellColour(CELLSTATUS::HIDDEN));
	};

	for (const auto& btn : buttons_)
	{
		btn->Bind(wxEVT_LEFT_DOWN, leftClick);
		btn->Bind(wxEVT_RIGHT_DOWN, rightClick);
		btn->Bind(wxEVT_ENTER_WINDOW, hover);
		btn->Bind(wxEVT_LEAVE_WINDOW, hoverLeft);
	}
}

std::tuple<int16_t, int16_t> Game::getCellCoordinatesById(const int id, const int16_t cols)
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

inline int Game::getCellIndexByCoordinates(const int16_t x, const int16_t y, const int16_t cols)
{
	return y * cols + x;
}

inline int Game::getCellIdByCoordinates(const int16_t x, const int16_t y, const int16_t cols)
{
	return getCellIndexByCoordinates(x, y, cols) + firstButtonId;
}

wxColour Game::GetCellTextColour(const int mines)
{
	switch (mines)
	{
	case 1:
		return {0, 0, 192, 255};
	case 2:
		return {192, 0, 0, 255};
	case 3:
		return {0, 192, 0, 255};
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
	case CELLSTATUS::HOVER:
		return {35, 35, 35, 255};
	case CELLSTATUS::CHECKEDHOVER:
		return {200, 0, 0, 255};
	}

	throw std::runtime_error("GetCellColour");
}

void Game::explode(const wxColour& colour) const
{
	const auto& mines = board_.allMines_;

	int i = 0;
	for (const auto& mine : mines)
	{
		buttons_[getCellIndexByCoordinates(mine.x, mine.y, cols_)]->SetBackgroundColour(colour);

		auto time = static_cast<int>(std::log(i++));
		std::this_thread::sleep_for(std::chrono::milliseconds(time));
		wxYield();
	}
}

void Game::end(const bool win)
{
	playing_ = false;
	over_ = true;
	win_ = win;

	if (timer_.joinable())
		timer_.join();

	if (win)
		explode(GetCellColour(CELLSTATUS::WIN));
	else
		explode(GetCellColour(CELLSTATUS::BLOWN));
}

std::tuple<bool, bool, int> Game::Status() const
{
	return {over_, win_, time_};
}


inline void Game::startTimer()
{
	timer_ = std::thread([this]
	{
		while (playing_)
		{
			timeLabel_->CallAfter([this]
			{
				timeLabel_->SetLabel(std::to_string(time_++));
			});

			std::this_thread::sleep_for(std::chrono::seconds(1));
		}
	});

	timer_.detach();
}
