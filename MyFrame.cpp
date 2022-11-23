#include "MyFrame.hpp"

#include "Game.hpp"

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
				UpdateMenu();
			}
		},
		{
			L"WYJŚCIE",
			[](wxMouseEvent& e)
			{
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
}

MyFrame::~MyFrame()
{
	if (game_ == nullptr)
		delete game_;
}

void MyFrame::startGame(std::tuple<int16_t, int16_t, int16_t> level)
{
	grid_->Clear(true);

	const auto [cols, rows, mines] = level;
	game_ = new Game(panel_, cols, rows, mines);
	Fit();
	auto result = game_->Start();

	std::cerr << result;
}

void MyFrame::Menu()
{
	activeMenu_ = menu_;
	UpdateMenu();
}

void MyFrame::UpdateMenu()
{
	grid_->Clear(true);

	const auto logo = new wxStaticText(panel_, wxID_ANY, "SAPER", wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE_HORIZONTAL);
	logo->SetForegroundColour(wxColour(*wxWHITE));
	wxFont font = logo->GetFont();
	font.SetPointSize(20);
	font.SetWeight(wxFONTWEIGHT_BOLD);
	logo->SetFont(font);
	grid_->Add(logo);

	for (const auto& [label, func] : activeMenu_)
	{
		const auto item = new wxStaticText(panel_, wxID_ANY, label, wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE_HORIZONTAL);
		item->SetForegroundColour(wxColour(*wxWHITE));
		item->Bind(wxEVT_LEFT_UP, func);
		grid_->Add(item);
	}

	panel_->SetSizerAndFit(grid_, true);
}
