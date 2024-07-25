///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 4.2.1-0-g80c4cb6)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#pragma once
#include <wx/wx.h>
#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/intl.h>
#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/choice.h>
#include <wx/textctrl.h>
#include <wx/checkbox.h>
#include <wx/sizer.h>
#include <wx/spinctrl.h>
#include <wx/statbox.h>
#include <wx/button.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/frame.h>

///////////////////////////////////////////////////////////////////////////
class MyApp : public wxApp {
public:
    virtual bool OnInit();
};

enum {
    ID_Button = 1
};
///////////////////////////////////////////////////////////////////////////////
/// Class MyFrame
///////////////////////////////////////////////////////////////////////////////
class MyFrame : public wxFrame
{
	private:

	protected:
		wxStaticText* m_staticLabel_Species;
		wxChoice* m_Choice_Species;

		wxStaticText* m_staticLabel_Nickname;
		wxTextCtrl* m_textCtrl_Nickname;

		wxCheckBox* m_checkBox_Shiny;

		wxStaticText* m_staticLabel_Level;
		wxSpinCtrl* m_spinCtrl_Level;

		wxStaticText* m_staticLabel_Gender;
		wxChoice* m_choice_Gender;

		wxStaticText* m_staticLabel_Nature;
		wxChoice* m_choice_Nature;
		
		wxStaticText* m_staticLabel_Item;
		wxChoice* m_choice_Item;

		wxStaticText* m_staticLabel_Ability;
		wxChoice* m_choice_Ability;

		wxStaticText* m_staticLabel_Move1;
		wxChoice* m_choice_Move1;

		wxStaticText* m_staticLabel_Move2;
		wxChoice* m_choice_Move2;

		wxStaticText* m_staticLabel_Move3;
		wxChoice* m_choice_Move3;

		wxStaticText* m_staticLabel_Move4;
		wxChoice* m_choice_Move4;

		wxStaticText* m_staticLabel_HP_EV;
		wxSpinCtrl* m_spinCtrl_HP_EV;
		wxStaticText* m_staticLabel_HP_IV;
		wxSpinCtrl* m_spinCtrl_HP_IV;
		wxStaticText* m_staticLabel_Attack_EV;
		wxSpinCtrl* m_spinCtrl_Attack_EV;
		wxStaticText* m_staticLabel_Attack_IV;
		wxSpinCtrl* m_spinCtrl_Attack_IV;
		wxStaticText* m_staticLabel_Defence_EV;
		wxSpinCtrl* m_spinCtrl_Defence_EV;
		wxStaticText* m_staticLabel_Defence_IV;
		wxSpinCtrl* m_spinCtrl_Defence_IV;
		wxStaticText* m_staticLabel_SpezAtk_EV;
		wxSpinCtrl* m_spinCtrl_SpezAtk_EV;
		wxStaticText* m_staticLabel_SpezAtk_IV;
		wxSpinCtrl* m_spinCtrl_SpezAtk_IV;
		wxStaticText* m_staticLabel_SpezDef_EV;
		wxSpinCtrl* m_spinCtrl_SpezDef_EV;
		wxStaticText* m_staticLabel_SpezDef_IV;
		wxSpinCtrl* m_spinCtrl_SpezDef_IV;
		wxStaticText* m_staticLabel_Speed_EV;
		wxSpinCtrl* m_spinCtrl_Speed_EV;
		wxStaticText* m_staticLabel_Speed_IV;
		wxSpinCtrl* m_spinCtrl_Speed_IV;
		//wxButton* m_buttonStart;
		wxStaticText* m_staticLabel_SLOT;
		wxSpinCtrl* m_spinCtrl_Slot;
		wxButton* m_buttonGenerate;
		wxStaticText* m_staticLabel_Start;
		wxButton* m_buttonStart;

		wxStaticBitmap* m_PokemonSprite;

		wxString ImageFolderPath;

		void LoadSprite(const wxString& ImageName);


		wxDECLARE_EVENT_TABLE();

		// Virtual event handlers, override them in your derived class
		void m_buttonStartOnButtonClick( wxCommandEvent& event ) ;
		void m_buttonGenerateOnButtonClick( wxCommandEvent& event ) ;

		void EVsOnSpin(wxSpinEvent& event);
		void SpeciesOnChoice(wxCommandEvent& event);

		static const int MaxEvValue = 510;

	public:

		//MyFrame( const wxString& title = wxEmptyString );
		MyFrame( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 975,557 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );

		~MyFrame();

};


