#pragma once
#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

class myDialog : wxDialog
{
	int cols_ = 8;
	int rows_ = 8;
	int mines_ = 10;
	std::vector<wxTextCtrl*> inputs_{};

public:
	myDialog(wxWindow* parent,
	         wxWindowID id,
	         const wxPoint& pos = wxDefaultPosition,
	         const wxSize& size = wxDefaultSize,
	         long style = wxDEFAULT_DIALOG_STYLE,
	         const wxString& name = wxDialogNameStr);

	std::tuple<int16_t, int16_t, int16_t> GetLevel();
	int ShowModal() override { return wxDialog::ShowModal(); }
	bool Destroy() override { return wxDialog::Destroy(); }
};
