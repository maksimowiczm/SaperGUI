#pragma once

#include <vector>
#include <algorithm>
#include <functional>
#include <cinttypes>
#include <string>

class Menu
{
private:
	bool stop_ = false;
	std::vector<std::tuple<int16_t, int16_t, int16_t>> levels_{
		{8, 8, 10},
		{16, 16, 40},
		{30, 16, 99}
	};

	using MenuItem = std::tuple<std::string*, std::function<void()>>;
	std::vector<MenuItem> activeMenu_;
	std::vector<MenuItem> menu_;
	std::vector<MenuItem> difficultyMenu_;
	std::vector<MenuItem> gameOverMenu_;
	// Aktualna pozycja w menu
	uint32_t cursor_ = 0;

	void Display() const;

	void Use() const;

	void StartGame(std::tuple<int16_t, int16_t, int16_t> level);

	void PrintGameOver(bool clear, bool win, int time);

	std::tuple<int16_t, int16_t, int16_t> CustomLevel();

public:
	// Oznaczenia możliwych ruchów użytkownika
	enum controls
	{
		up,
		down,
		left,
		right,
		use,
		check,
		none
	};

	// Sprawdza czy naciśnięty przycisk jest zbindowany do jednej z kontrolek
	static bool
	checkIfControlUsed(controls control, int ch, std::vector<std::tuple<controls, std::vector<int>>> controls);

	Menu();

	~Menu();

	void Start();
};
