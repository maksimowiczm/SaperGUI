#include "Game.hpp"

Game::Game(wxPanel* panel, const int16_t cols, const int16_t rows, const int16_t mines) : panel_(panel)
{
	const auto grid = new wxBoxSizer(wxVERTICAL);
	board_ = new Board(cols, rows, mines);
	cols_ = new int16_t(cols);
	rows_ = rows;
	playing_ = true;

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
}

int Game::Start()
{
	// RIGHT CLICK
	const std::function<void(wxMouseEvent& e)> rightClick = [this](const wxMouseEvent& e)
	{
		if (!playing_)
			return;

		const auto btn = dynamic_cast<wxButton*>(e.GetEventObject());

		const auto [x, y] = getCellCoordiatesById(e.GetId(), *cols_);
		if (board_->board_[y][x].isRevealed)
			return;

		if (board_->CheckSwitch(x, y))
			btn->SetBackgroundColour(GetCellColour(CELLSTATUS::CHECKED));
		else
			btn->SetBackgroundColour(GetCellColour(CELLSTATUS::HIDDEN));
	};

	// LEFT CLICK
	const std::function<void(wxMouseEvent& e)> leftClick = [this](const wxMouseEvent& e)
	{
		if (!playing_)
			return;

		const auto [x, y] = getCellCoordiatesById(e.GetId(), *cols_);
		const auto cells = board_->RevealCell(x, y);
		revealed_cells_ += cells.size();

		for (const auto& cell : cells)
		{
			if (cell.isMine)
			{
				explode();
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
	};

	for (const auto& btn : buttons_)
	{
		btn->Bind(wxEVT_RIGHT_UP, rightClick);
		btn->Bind(wxEVT_LEFT_UP, leftClick);
	}

	return -1;
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
	playing_ = false;
	const auto mines = board_->allMines_;

	for (const auto& mines : mines)
	{
		buttons_[getCellIdByCoordiates(mines.x, mines.y, *cols_)]->SetBackgroundColour(GetCellColour(CELLSTATUS::BLOWN));
	}
}
