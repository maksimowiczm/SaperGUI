#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include "MyFrame.hpp"

class MyApp final : public wxApp
{
public:
	bool OnInit() override
	{
		const auto frame = new MyFrame();
		frame->Show(true);
		frame->Menu();
		//frame->startGame({ 30,16,99 });

		return true;
	}
};

//wxIMPLEMENT_APP(MyApp);
wxIMPLEMENT_APP_CONSOLE(MyApp);
