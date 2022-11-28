#include "Alfabet.hpp"

namespace konsola
{
	std::vector<std::vector<bool>> Alfabet::at(char c) const
	{
		return alfabet.at(c);
	}
}
