#include "Menu.hpp"

#include <iostream>
#include <string>

Menu::Menu()
{
	menu_ = {
		{
			new std::string{"start"}, [this]
			{
				this->Display();
				this->activeMenu_ = this->difficultyMenu_;
			}
		},
		{
			new std::string{"wyjscie"}, [this]
			{
				stop_ = true;
			}
		},
	};
	difficultyMenu_ = {
		{
			new std::string{"latwy"}, [this]
			{
				Display();
				this->StartGame(levels_[0]);
			}
		},
		{
			new std::string{"sredni"}, [this]
			{
				Display();
				this->StartGame(levels_[1]);
			}
		},
		{
			new std::string{"trudny"}, [this]
			{
				Display();
				this->StartGame(levels_[2]);
			}
		},
		{
			new std::string{"niestandardowy"}, [this]
			{
				Display();
				this->StartGame(CustomLevel());
			}
		},
	};
	gameOverMenu_ = {
		{
			new std::string{"wyjscie"}, [this]
			{
				this->Display();
				this->PrintGameOver(true, true, INT32_MAX);
				this->activeMenu_ = this->menu_;
			}
		},
	};
	activeMenu_ = menu_;
}

void Menu::Display() const
{
	for (uint32_t i = 0; i < activeMenu_.size(); i++)
	{
		const auto& [str, f] = activeMenu_[i];
	}
}

bool Menu::checkIfControlUsed(controls control, int ch, std::vector<std::tuple<controls, std::vector<int>>> controls)
{
	const auto& vec = std::get<1>(controls.at(control));
	return std::ranges::find(vec, ch) != vec.end();
}

void Menu::Start()
{
}

void Menu::Use() const
{
	std::get<1>(activeMenu_[cursor_])();
}

void Menu::StartGame(std::tuple<int16_t, int16_t, int16_t> level)
{
	auto& [x, y, mines] = level;
	//auto game = Game(x, y, mines, COORD{int16_t(cols_ / 2), 0}, controls_);
	// game.DisplayBoard(false);
	// auto [win, time] = game.Start();
	// game.DisplayBoard(true);

	// PrintGameOver(false, win, time);
}

void Menu::PrintGameOver(const bool clear, const bool win, const int time)
{
	const std::string result = win ? "rozbrojone" : "game over";
	// if (win)
	// 	Print("twoj czas " + std::to_string(time), {int16_t(cols_ / 2), 12}, NULL, clear);

	activeMenu_ = gameOverMenu_;
	cursor_ = 0;
}

std::tuple<int16_t, int16_t, int16_t> Menu::CustomLevel()
{
	int32_t col = 8, row = 8, mine = 10;
	const std::vector variables{&col, &row, &mine};
	const auto limit = [this, &col, &row, &mine]
	{
		if (cursor_ != 3)
		{
			if (col + 1 > 30)
				col = 30;
			if (col < 8)
				col = 8;
			if (row + 1 > 24)
				row = 24;
			if (row < 8)
				row = 8;
			if (mine + 1 > col * row - 1)
				mine = col * row - 1;
			if (mine < col * row * 0.1)
				mine = col * row * 0.1;
			Use();
		}
	};
	std::string cols{"kolumny " + std::to_string(col)},
	            rows{"wiersze " + std::to_string(row)},
	            mines{"miny " + std::to_string(mine)},
	            start{"start"};
	auto ready = false, cursorMoved = true;
	cursor_ = 0;
	// std::vector<std::tuple<std::string *, COORD, std::function<void()>>> customLevelMenu = {
	// 	{&cols, COORD(cols_ / 2, 6), [this, &cols, &col, &customLevelMenu]
	// 	 {
	// 		 Print(cols, std::get<1>(customLevelMenu[0]), NULL, true);
	// 		 cols = "kolumny " + std::to_string(col);
	// 	 }},
	// 	{&rows, COORD(cols_ / 2, 12), [this, &rows, &row, &customLevelMenu]
	// 	 {
	// 		 Print(rows, std::get<1>(customLevelMenu[1]), NULL, true);
	// 		 rows = "wiersze " + std::to_string(row);
	// 	 }},
	// 	{&mines, COORD(cols_ / 2, 18), [this, &mines, &mine, &col, &row, &customLevelMenu]
	// 	 {
	// 		 Print(mines, std::get<1>(customLevelMenu[2]), NULL, true);
	// 		 mines = "miny " + std::to_string(mine);
	// 	 }},
	// 	{&start, COORD(cols_ / 2, 24), [&ready]
	// 	 { ready = true; }}};
	// activeMenu_ = customLevelMenu;

	while (!ready)
	{
		Display();

		controls key = none;

		switch (key)
		{
		case up:
			limit();
			cursor_ = (cursor_ - 1) % activeMenu_.size();
			cursorMoved = true;
			break;
		case down:
			limit();
			cursor_ = (cursor_ + 1) % activeMenu_.size();
			cursorMoved = true;
			break;
		case use:
			Use();
			break;
		default:
			break;
		}
	}

	Display();
	return {col, row, mine};
}

Menu::~Menu()
{
	for (auto& [ptr, func] : menu_)
		delete ptr;
	for (auto& [ptr, func] : difficultyMenu_)
		delete ptr;
	for (auto& [ptr, func] : gameOverMenu_)
		delete ptr;
}
