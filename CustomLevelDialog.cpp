#include "CustomLevelDialog.hpp"
#include <wx/valnum.h>


myDialog::myDialog(wxWindow* parent,
                   wxWindowID id,
                   const wxPoint& pos,
                   const wxSize& size,
                   long style,
                   const wxString& name): wxDialog(parent, id, "Podaj rozmiar", pos, size, style, name)
{
	wxIntegerValidator<int16_t> colsVal(nullptr, 0, 30);
	wxIntegerValidator<int16_t> rowsVal(nullptr, 0, 16);
	wxIntegerValidator<int16_t> minesVal(nullptr, 1, INT16_MAX);

	enum type
	{
		COLS,
		ROWS,
		MINES
	};

	std::vector<std::tuple<type, std::wstring, wxIntegerValidator<int16_t>*>> items = {
		{COLS, L"Kolumny (min: 8 max: 30)", &colsVal},
		{ROWS, L"Wiersze (min: 8 max: 16)", &rowsVal},
		{MINES, L"Miny (min: 10% planszy)", &minesVal},
	};

	const auto grid = new wxBoxSizer(wxVERTICAL);

	for (auto& [type, textLabel, validator] : items)
	{
		const auto field = new wxBoxSizer(wxVERTICAL);
		const auto text = new wxStaticText(this, wxID_ANY, textLabel, wxDefaultPosition, wxDefaultSize);
		const auto textArea = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, {200, 20}, 0, *validator);
		inputs_.push_back(textArea);

		switch (type)
		{
		case COLS:
			textArea->SetValue(std::to_string(cols_));
			textArea->Bind(wxEVT_TEXT, [this, textArea](wxCommandEvent& e)
			{
				textArea->GetValue().ToInt(&cols_);
				e.Skip();
			});
			break;
		case ROWS:
			textArea->SetValue(std::to_string(rows_));
			textArea->Bind(wxEVT_TEXT, [this, textArea](wxCommandEvent& e)
			{
				textArea->GetValue().ToInt(&rows_);
				e.Skip();
			});
			break;
		case MINES:
			textArea->SetValue(std::to_string(mines_));
			textArea->Bind(wxEVT_TEXT, [this, textArea](wxCommandEvent& e)
			{
				int value;
				textArea->GetValue().ToInt(&value);
				if (value > 0 && value < cols_ * rows_)
					mines_ = value;
				else if (value > 0)
					textArea->SetValue(std::to_string(mines_));

				if (textArea->GetValue().length() > std::to_string(mines_).length())
					textArea->SetValue(std::to_string(mines_));

				textArea->SetInsertionPointEnd();

				e.Skip();
			});
			break;
		}


		field->Add(text, 0, wxEXPAND | wxALL, 5);
		field->Add(textArea, 0, wxEXPAND | wxALL, 5);

		grid->Add(field);
	}

	const auto btn = new wxButton(this, wxID_OK, L"Zatwierdź", wxDefaultPosition, wxDefaultSize);
	grid->Add(btn, 0, wxEXPAND | wxALL, 5);

	SetSizerAndFit(grid);
}

std::tuple<int16_t, int16_t, int16_t> myDialog::GetLevel()
{
	if (cols_ < 8)
		cols_ = 8;
	if (rows_ < 8)
		rows_ = 8;
	if (mines_ < 0.1 * cols_ * rows_)
		mines_ = 0.1 * cols_ * rows_;

	return {cols_, rows_, mines_};
}
