#include <tuple>

namespace konsola
{
	class ConsoleApp
	{
	public:
		std::tuple<int16_t, int16_t> configureConsole();
		void run();
	};
}
