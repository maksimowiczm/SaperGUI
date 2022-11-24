#include "MyFrame.hpp"

MyFrame::MyFrame() : wxFrame(nullptr, wxID_ANY, "Saper")
{
	panel_ = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize);
	panel_->SetBackgroundColour(wxColour(0, 0, 0, 255));
	grid_ = new wxBoxSizer(wxVERTICAL);

	menu_.emplace_back();
	menu_ = {
		{
			L"START", [this](wxMouseEvent& e)
			{
				this->activeMenu_ = this->difficultyMenu_;
				UpdateMenu(true, true);
			}
		},
		{
			L"WYJŚCIE",
			[this](wxMouseEvent& e)
			{
				Close(true);
			}
		}
	};
	difficultyMenu_ = {
		{
			L"ŁATWY", [this](wxMouseEvent& e)
			{
				this->startGame(levels_[0]);
			}
		},
		{
			L"ŚREDNI", [this](wxMouseEvent& e)
			{
				this->startGame(levels_[1]);
			}
		},
		{
			L"TRUDNY", [this](wxMouseEvent& e)
			{
				this->startGame(levels_[2]);
			}
		},
		{
			L"NIESTANDARDOWY", [this](wxMouseEvent& e)
			{
			}
		},
	};
	gameOverMenu_ = {
		{
			L"POWRÓT DO MENU",
			[this](wxMouseEvent& e)
			{
				this->activeMenu_ = this->menu_;
				UpdateMenu(true, true);
			}
		}
	};
}

MyFrame::~MyFrame()
{
	if (game_ != nullptr)
		delete game_;
}

void MyFrame::startGame(level_t level)
{
	grid_->Clear(true);

	if (game_ != nullptr)
		delete game_;

	const auto [cols, rows, mines] = level;
	const auto func = new std::function([this](const bool win, const int time) { endGame(win, time); });
	game_ = new Game(panel_, cols, rows, mines, func);
	Fit();
	game_->Start();
}

void MyFrame::endGame(const bool win, const int time)
{
	grid_->Clear(true);

	displayLogo();

	const auto label = new wxStaticText(panel_, wxID_ANY, win ? "ROZBROJONE" : "GAME OVER", wxDefaultPosition, wxDefaultSize,
	                                    wxALIGN_CENTRE_HORIZONTAL);
	label->SetForegroundColour(wxColour(*wxWHITE));
	grid_->Add(label);
	if (win)
	{
		const auto timeLabel = new wxStaticText(panel_, wxID_ANY, L"TWÓJ CZAS " + std::to_string(time), wxDefaultPosition, wxDefaultSize,
		                                        wxALIGN_CENTRE_HORIZONTAL);
		timeLabel->SetForegroundColour(wxColour(*wxWHITE));
		grid_->Add(timeLabel);
	}

	activeMenu_ = gameOverMenu_;
	UpdateMenu(false, false);
}

void MyFrame::displayLogo() const
{
	const auto logo = new wxStaticText(panel_, wxID_ANY, "SAPER", wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE_HORIZONTAL);
	logo->SetForegroundColour(wxColour(*wxWHITE));
	wxFont font = logo->GetFont();
	font.SetPointSize(20);
	font.SetWeight(wxFONTWEIGHT_BOLD);
	logo->SetFont(font);
	grid_->Add(logo);
}

void MyFrame::Menu()
{
	activeMenu_ = menu_;
	UpdateMenu(false, true);
}

void MyFrame::UpdateMenu(const bool clear, const bool logo) const
{
	if (grid_ == nullptr)
		std::cerr << "nullptr";

	if (clear)
		grid_->Clear(true);

	if (logo)
		displayLogo();

	for (const auto& [label, func] : activeMenu_)
	{
		const auto item = new wxStaticText(panel_, wxID_ANY, label, wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE_HORIZONTAL);
		item->SetForegroundColour(wxColour(*wxWHITE));
		item->Bind(wxEVT_LEFT_UP, func);
		grid_->Add(item);
	}

	panel_->SetSizerAndFit(grid_, true);
}
