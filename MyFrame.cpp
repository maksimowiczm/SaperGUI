#include "MyFrame.hpp"

#include <future>
#include <utility>

#include <Windows.h>
#include "CustomLevelDialog.hpp"
#include "Konsola/ConsoleApp.hpp"

MyFrame::MyFrame() : wxFrame(nullptr, wxID_ANY, "Saper", wxDefaultPosition, wxDefaultSize, wxCAPTION | wxCLOSE_BOX | wxMINIMIZE_BOX)
{
	HideConsole();

	panel_ = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize);
	panel_->SetBackgroundColour(wxColour(0, 0, 0, 255));
	font_ = new wxFont();

	grid_ = new wxBoxSizer(wxVERTICAL);

	menu_ = {
		{
			true,
			L"START", [this](wxMouseEvent& e)
			{
				this->activeMenu_ = this->difficultyMenu_;
				this->UpdateMenu(true, true);
			}
		},
		{
			true,
			L"WERSJA KONSOLOWA", [this](wxMouseEvent& e)
			{
				ShowConsole();
				Hide();

				auto consoleApp = konsola::ConsoleApp();
				consoleApp.run();

				HideConsole();
				Show();
			}
		},
		{
			true,
			L"WYJŚCIE",
			[this](wxMouseEvent& e)
			{
				Close(true);
			}
		},
		{false, L"", nullptr},
	};
	difficultyMenu_ = {
		{
			true,
			L"ŁATWY", [this](wxMouseEvent& e)
			{
				startGame(levels_[0]);
			}
		},
		{
			true,
			L"ŚREDNI", [this](wxMouseEvent& e)
			{
				startGame(levels_[1]);
			}
		},
		{
			true,
			L"TRUDNY", [this](wxMouseEvent& e)
			{
				startGame(levels_[2]);
			}
		},
		{
			true,
			L"NIESTANDARDOWY", [this](wxMouseEvent& e)
			{
				const auto dialog = new myDialog(this, wxID_ANY, wxDefaultPosition, wxDefaultSize,wxCAPTION);

				level_t level;
				if (const auto status = dialog->ShowModal(); status == wxID_OK)
					level = dialog->GetLevel();

				dialog->Destroy();

				startGame(level);
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
		if (const auto [status, win, time] = game_->Status(); status)
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
		menu.emplace_back(false, L"TWÓJ CZAS: " + std::to_wstring(time), nullptr);

	menu.emplace_back(true, L"POWRÓT DO MENU",
	                  [this](wxMouseEvent& e)
	                  {
		                  this->activeMenu_ = this->menu_;
		                  UpdateMenu(true, true);
	                  });
	menu.emplace_back(false, L"", nullptr);
	if (!win)
		menu.emplace_back(false, L"", nullptr);

	activeMenu_ = menu;
	UpdateMenu(true, true);
}

void MyFrame::displayLogo() const
{
	const auto logo = new wxStaticText(panel_, wxID_ANY, "SAPER", wxDefaultPosition, DEFAULTSIZE, wxALIGN_CENTRE_HORIZONTAL);
	logo->SetForegroundColour(wxColour(*wxWHITE));
	wxFont font = logo->GetFont();
	font.SetPointSize(100);
	font.SetWeight(wxFONTWEIGHT_BOLD);
	logo->SetFont(font);
	grid_->Add(logo);
}

void MyFrame::Menu()
{
	activeMenu_ = menu_;
	UpdateMenu(false, true);
}

std::vector<wxButton*> MyFrame::UpdateMenu(const bool clear = true, const bool logo = true)
{
	if (clear)
		grid_->Clear(true);

	if (logo)
		displayLogo();

	std::vector<wxButton*> items{};
	for (const auto& [active, label, func] : activeMenu_)
	{
		const auto item = new wxButton(panel_, wxID_ANY, label, wxDefaultPosition, DEFAULTSIZE, wxBORDER_NONE);
		items.push_back(item);

		wxFont font = item->GetFont();
		font.SetPointSize(75);
		font.SetWeight(wxFONTWEIGHT_BOLD);
		item->SetFont(font);

		item->SetBackgroundColour({0, 0, 0, 255});

		if (active)
		{
			item->SetForegroundColour({200, 224, 244, 255});

			item->Bind(wxEVT_LEFT_UP, func);
			item->Bind(wxEVT_ENTER_WINDOW, [item](wxMouseEvent& e)
			{
				item->SetForegroundColour({222, 0, 0, 255});
			});
			item->Bind(wxEVT_LEAVE_WINDOW, [item](wxMouseEvent& e)
			{
				item->SetForegroundColour({200, 224, 244, 255});
			});
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

	return items;
}

void MyFrame::HideConsole()
{
	ShowWindow(GetConsoleWindow(), SW_HIDE);
}

void MyFrame::ShowConsole()
{
	ShowWindow(GetConsoleWindow(), SW_SHOW);
}
