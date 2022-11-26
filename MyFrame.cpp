#include "MyFrame.hpp"

#include <future>
#include <utility>

MyFrame::MyFrame() : wxFrame(nullptr, wxID_ANY, "Saper")
{
	panel_ = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize);
	panel_->SetBackgroundColour(wxColour(0, 0, 0, 255));

	grid_ = new wxBoxSizer(wxVERTICAL);

	menu_ = {
		{
			true,
			L"START", [this](wxMouseEvent& e)
			{
				this->activeMenu_ = this->difficultyMenu_;
				UpdateMenu(true, true);
			}
		},
		{
			true,
			L"WYJŚCIE",
			[this](wxMouseEvent& e)
			{
				Close(true);
			}
		}
	};
	difficultyMenu_ = {
		{
			true,
			L"ŁATWY", [this](wxMouseEvent& e)
			{
				this->startGame(levels_[0]);
			}
		},
		{
			true,
			L"ŚREDNI", [this](wxMouseEvent& e)
			{
				this->startGame(levels_[1]);
			}
		},
		{
			true,
			L"TRUDNY", [this](wxMouseEvent& e)
			{
				this->startGame(levels_[2]);
			}
		},
		{
			true,
			L"NIESTANDARDOWY", [this](wxMouseEvent& e)
			{
			}
		},
	};
}

MyFrame::~MyFrame()
{
	end_ = true;
	if (game_ != nullptr)
		delete game_;
}

void MyFrame::startGame(const level_t& level)
{
	if (game_ != nullptr)
		delete game_;

	const auto& [cols, rows, mines] = level;

	game_ = new Game(panel_, cols, rows, mines);
	Fit();
	game_->Start();

	while (!end_)
	{
		if (const auto [status, win, time] = game_->End(); status)
		{
			std::this_thread::sleep_for(std::chrono::seconds(1));
			endGame(win, time);
			break;
		}

		wxYield();
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
	}
}

void MyFrame::endGame(const bool win, const int time)
{
	grid_->Clear(true);

	std::vector<MenuItem> menu = {
		{
			false,
			(win ? L"ROZBROJONE" : L"GAME OVER"), nullptr
		},
	};
	if (win)
		menu.emplace_back(false, L"TWÓJ CZAS " + std::to_wstring(time), nullptr);

	menu.emplace_back(true, L"POWRÓT DO MENU",
	                  [this](wxMouseEvent& e)
	                  {
		                  this->activeMenu_ = this->menu_;
		                  UpdateMenu(true, true);
	                  });
	menu.emplace_back(false, L"", nullptr);

	activeMenu_ = menu;
	UpdateMenu(true, true);
}

void MyFrame::displayLogo() const
{
	const auto logo = new wxStaticText(panel_, wxID_ANY, "SAPER", wxDefaultPosition, {256, 50}, wxALIGN_CENTRE_HORIZONTAL);
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

void MyFrame::UpdateMenu(const bool clear, const bool logo)
{
	if (clear)
		grid_->Clear(true);

	if (logo)
		displayLogo();

	for (const auto& [active, label, func] : activeMenu_)
	{
		const auto item = new wxStaticText(panel_, wxID_ANY, label, wxDefaultPosition, {256, 50}, wxALIGN_CENTRE_HORIZONTAL);
		if (active)
		{
			item->SetForegroundColour({200, 224, 244, 255});
			item->Bind(wxEVT_LEFT_UP, func);
		}
		else
		{
			item->SetForegroundColour(wxColour(*wxWHITE));
		}

		grid_->Add(item, 0, wxEXPAND);
	}

	panel_->Layout();
	panel_->SetSizerAndFit(grid_);
	Fit();
}
