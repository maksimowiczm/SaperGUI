#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include "Menu.hpp"
#include "MyFrame.hpp"

class MyApp final : public wxApp
{
public:
	bool OnInit() override
	{
		const auto frame = new MyFrame();
		frame->Show(true);

		//frame->StartGame();
		frame->Menu();

		return true;
	}
};

wxIMPLEMENT_APP(MyApp);
