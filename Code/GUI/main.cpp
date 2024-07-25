#include "main.h"
#include "GUI_Species.h"
#include "GUI_Items.h"
#include "GUI_Moves.h"
#include "GUI_Natures.h"

extern "C" {
    #include "C_Files/main_c.h"
}

wxBEGIN_EVENT_TABLE(MyFrame, wxFrame)
    EVT_BUTTON(ID_Button, MyFrame::m_buttonStartOnButtonClick)
wxEND_EVENT_TABLE()

wxIMPLEMENT_APP(MyApp);


bool MyApp::OnInit() {
    MyFrame* frame = new MyFrame(NULL,wxID_ANY,"Generate Pokemon Structure",wxDefaultPosition,wxSize( 975,557 ),wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL);
    frame->Show(true);
	InitTransmitBuffer();
    return true;
};

MyFrame::MyFrame( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( NULL, wxID_ANY, title, pos, size, style )
//MyFrame::MyFrame(const wxString& title ) : wxFrame( NULL, wxID_ANY, title)
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	//Define the folder where the sprites are stored
	ImageFolderPath = wxT("PokemonSprites/");
	wxImage::AddHandler(new wxPNGHandler);


	wxBoxSizer* TopLevelSizer;
	TopLevelSizer = new wxBoxSizer( wxVERTICAL );

	wxBoxSizer* Layer1Sizer1;
	Layer1Sizer1 = new wxBoxSizer( wxHORIZONTAL );

	m_staticLabel_Species = new wxStaticText( this, wxID_ANY, _("Species"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticLabel_Species->Wrap( -1 );
	Layer1Sizer1->Add( m_staticLabel_Species, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	//wxString m_Choice_SpeciesChoices[] = { _("Bisasam"), _("Bisaknsop"), _("Bisaflor"), _("Glumanda"), _("Glutexo"), _("Glurak"), _("Schiggy"), _("Schillok"), _("Turtok") };
	int m_Choice_SpeciesNChoices = sizeof( m_Choice_SpeciesChoices ) / sizeof( wxString );
	//int m_Choice_SpeciesNChoices = sizeof( species_GUI ) / sizeof( wxString );
	m_Choice_Species = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_Choice_SpeciesNChoices, m_Choice_SpeciesChoices, 0 );
	//m_Choice_Species = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_Choice_SpeciesNChoices, species_GUI, 0 );
	
	m_Choice_Species->SetSelection( 0 );
	Layer1Sizer1->Add( m_Choice_Species, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5 );


	Layer1Sizer1->Add( 10, 0, 0, wxEXPAND, 5 );

	m_staticLabel_Nickname = new wxStaticText( this, wxID_ANY, _("Nickname"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticLabel_Nickname->Wrap( -1 );
	Layer1Sizer1->Add( m_staticLabel_Nickname, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_textCtrl_Nickname = new wxTextCtrl( this, wxID_ANY, _("Nickname"), wxDefaultPosition, wxDefaultSize, 0 );
	#ifdef __WXGTK__
	if ( !m_textCtrl_Nickname->HasFlag( wxTE_MULTILINE ) )
	{
	m_textCtrl_Nickname->SetMaxLength( 20 );
	}
	#else
	m_textCtrl_Nickname->SetMaxLength( 20 );
	#endif
	Layer1Sizer1->Add( m_textCtrl_Nickname, 1, wxALL|wxALIGN_CENTER_VERTICAL, 5 );


	Layer1Sizer1->Add( 10, 0, 0, wxEXPAND, 5 );

	m_checkBox_Shiny = new wxCheckBox( this, wxID_ANY, _("Shiny"), wxDefaultPosition, wxDefaultSize, 0 );
	m_checkBox_Shiny->SetValue(true);
	Layer1Sizer1->Add( m_checkBox_Shiny, 1, wxALL|wxALIGN_CENTER_VERTICAL, 5 );


	TopLevelSizer->Add( Layer1Sizer1, 0, wxEXPAND, 5 );

	wxBoxSizer* Layer1Sizer2;
	Layer1Sizer2 = new wxBoxSizer( wxHORIZONTAL );

	m_staticLabel_Level = new wxStaticText( this, wxID_ANY, _("Level"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticLabel_Level->Wrap( -1 );
	Layer1Sizer2->Add( m_staticLabel_Level, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_spinCtrl_Level = new wxSpinCtrl( this, wxID_ANY, wxT("50"), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 100, 52 );
	Layer1Sizer2->Add( m_spinCtrl_Level, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );


	Layer1Sizer2->Add( 10, 0, 0, wxEXPAND, 5 );

	m_staticLabel_Gender = new wxStaticText( this, wxID_ANY, _("Gender"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticLabel_Gender->Wrap( -1 );
	Layer1Sizer2->Add( m_staticLabel_Gender, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	wxString m_choice_GenderChoices[] = { _("Male"), _("Female") };
	int m_choice_GenderNChoices = sizeof( m_choice_GenderChoices ) / sizeof( wxString );
	m_choice_Gender = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choice_GenderNChoices, m_choice_GenderChoices, 0 );
	m_choice_Gender->SetSelection( 1 );
	Layer1Sizer2->Add( m_choice_Gender, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_staticLabel_Nature = new wxStaticText( this, wxID_ANY, _("Nature"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticLabel_Nature->Wrap( -1 );
	Layer1Sizer2->Add( m_staticLabel_Nature, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	//wxString m_choice_NatureChoices[] = { _("Adamant") };
	int m_choice_NatureNChoices = sizeof( m_choice_NatureChoices ) / sizeof( wxString );
	m_choice_Nature = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choice_NatureNChoices, m_choice_NatureChoices, 0 );
	m_choice_Nature->SetSelection( 0 );
	Layer1Sizer2->Add( m_choice_Nature, 0, wxALL, 5 );

	TopLevelSizer->Add( Layer1Sizer2, 0, wxEXPAND, 5 );

	wxBoxSizer* Layer1Sizer3;
	Layer1Sizer3 = new wxBoxSizer( wxHORIZONTAL );

	m_staticLabel_Item = new wxStaticText( this, wxID_ANY, _("Item"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticLabel_Item->Wrap( -1 );
	Layer1Sizer3->Add( m_staticLabel_Item, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	//wxString m_choice_ItemChoices[] = { _("Rare Candy"), _("Masterball"), _("Lum Berry") };
	int m_choice_ItemNChoices = sizeof( m_choice_ItemChoices ) / sizeof( wxString );
	m_choice_Item = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choice_ItemNChoices, m_choice_ItemChoices, 0 );
	m_choice_Item->SetSelection( 1 );
	Layer1Sizer3->Add( m_choice_Item, 0, wxALL, 5 );


	Layer1Sizer3->Add( 10, 0, 0, wxEXPAND, 5 );

	m_staticLabel_Ability = new wxStaticText( this, wxID_ANY, _("Ability"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticLabel_Ability->Wrap( -1 );
	Layer1Sizer3->Add( m_staticLabel_Ability, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	wxString m_choice_AbilityChoices[] = { _("Primary"), _("Secundary") };
	int m_choice_AbilityNChoices = sizeof( m_choice_AbilityChoices ) / sizeof( wxString );
	m_choice_Ability = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choice_AbilityNChoices, m_choice_AbilityChoices, 0 );
	m_choice_Ability->SetSelection( 0 );
	Layer1Sizer3->Add( m_choice_Ability, 0, wxALL, 5 );


	TopLevelSizer->Add( Layer1Sizer3, 0, wxEXPAND, 5 );

	wxBoxSizer* Layer1Sizer4;
	Layer1Sizer4 = new wxBoxSizer( wxHORIZONTAL );

	wxStaticBoxSizer* Layer2staticSizer_Moves;
	Layer2staticSizer_Moves = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, _("Moves") ), wxVERTICAL );

	wxBoxSizer* Layer3Sizer_Move1;
	Layer3Sizer_Move1 = new wxBoxSizer( wxHORIZONTAL );

	m_staticLabel_Move1 = new wxStaticText( Layer2staticSizer_Moves->GetStaticBox(), wxID_ANY, _("Move 1"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticLabel_Move1->Wrap( -1 );
	Layer3Sizer_Move1->Add( m_staticLabel_Move1, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	//wxString m_choice_Move1Choices[] = { _("Hidden Power") };
	int m_choice_Move1NChoices = sizeof( m_choice_Move1Choices ) / sizeof( wxString );
	m_choice_Move1 = new wxChoice( Layer2staticSizer_Moves->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choice_Move1NChoices, m_choice_Move1Choices, 0 );
	m_choice_Move1->SetSelection( 0 );
	Layer3Sizer_Move1->Add( m_choice_Move1, 0, wxALL, 5 );


	Layer2staticSizer_Moves->Add( Layer3Sizer_Move1, 1, wxEXPAND, 5 );

	wxBoxSizer* Layer3Sizer_Move2;
	Layer3Sizer_Move2 = new wxBoxSizer( wxHORIZONTAL );

	m_staticLabel_Move2 = new wxStaticText( Layer2staticSizer_Moves->GetStaticBox(), wxID_ANY, _("Move 2"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticLabel_Move2->Wrap( -1 );
	Layer3Sizer_Move2->Add( m_staticLabel_Move2, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	//wxString m_choice_Move2Choices[] = { _("Hidden Power") };
	int m_choice_Move2NChoices = sizeof( m_choice_Move2Choices ) / sizeof( wxString );
	m_choice_Move2 = new wxChoice( Layer2staticSizer_Moves->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choice_Move2NChoices, m_choice_Move2Choices, 0 );
	m_choice_Move2->SetSelection( 0 );
	Layer3Sizer_Move2->Add( m_choice_Move2, 0, wxALL, 5 );


	Layer2staticSizer_Moves->Add( Layer3Sizer_Move2, 1, wxEXPAND, 5 );

	wxBoxSizer* Layer3Sizer_Move3;
	Layer3Sizer_Move3 = new wxBoxSizer( wxHORIZONTAL );

	m_staticLabel_Move3 = new wxStaticText( Layer2staticSizer_Moves->GetStaticBox(), wxID_ANY, _("Move 3"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticLabel_Move3->Wrap( -1 );
	Layer3Sizer_Move3->Add( m_staticLabel_Move3, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	//wxString m_choice_Move3Choices[] = { _("Hidden Power") };
	int m_choice_Move3NChoices = sizeof( m_choice_Move3Choices ) / sizeof( wxString );
	m_choice_Move3 = new wxChoice( Layer2staticSizer_Moves->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choice_Move3NChoices, m_choice_Move3Choices, 0 );
	m_choice_Move3->SetSelection( 0 );
	Layer3Sizer_Move3->Add( m_choice_Move3, 0, wxALL, 5 );


	Layer2staticSizer_Moves->Add( Layer3Sizer_Move3, 1, wxEXPAND, 5 );

	wxBoxSizer* Layer3Sizer_move4;
	Layer3Sizer_move4 = new wxBoxSizer( wxHORIZONTAL );

	m_staticLabel_Move4 = new wxStaticText( Layer2staticSizer_Moves->GetStaticBox(), wxID_ANY, _("Move 4"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticLabel_Move4->Wrap( -1 );
	Layer3Sizer_move4->Add( m_staticLabel_Move4, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	//wxString m_choice_Move4Choices[] = { _("Hidden Power") };
	int m_choice_Move4NChoices = sizeof( m_choice_Move4Choices ) / sizeof( wxString );
	m_choice_Move4 = new wxChoice( Layer2staticSizer_Moves->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choice_Move4NChoices, m_choice_Move4Choices, 0 );
	m_choice_Move4->SetSelection( 0 );
	Layer3Sizer_move4->Add( m_choice_Move4, 0, wxALL, 5 );


	Layer2staticSizer_Moves->Add( Layer3Sizer_move4, 1, wxEXPAND, 5 );


	Layer1Sizer4->Add( Layer2staticSizer_Moves, 0, 0, 5 );


	Layer1Sizer4->Add( 50, 0, 0, wxEXPAND, 5 );

	wxStaticBoxSizer* Layer2staticSizer_Stats;
	Layer2staticSizer_Stats = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, _("Stats") ), wxVERTICAL );

	wxBoxSizer* Layer3Sizer5_HP;
	Layer3Sizer5_HP = new wxBoxSizer( wxHORIZONTAL );

	m_staticLabel_HP_EV = new wxStaticText( Layer2staticSizer_Stats->GetStaticBox(), wxID_ANY, _("HP EV  "), wxDefaultPosition, wxSize( 100,-1 ), 0 );
	m_staticLabel_HP_EV->Wrap( -1 );
	Layer3Sizer5_HP->Add( m_staticLabel_HP_EV, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_spinCtrl_HP_EV = new wxSpinCtrl( Layer2staticSizer_Stats->GetStaticBox(), wxID_ANY, wxT("0"), wxDefaultPosition, wxSize( -1,-1 ), wxSP_ARROW_KEYS, 0, 252, 7 );
	Layer3Sizer5_HP->Add( m_spinCtrl_HP_EV, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_staticLabel_HP_IV = new wxStaticText( Layer2staticSizer_Stats->GetStaticBox(), wxID_ANY, _("HP IV  "), wxDefaultPosition, wxSize( 100,-1 ), 0 );
	m_staticLabel_HP_IV->Wrap( -1 );
	Layer3Sizer5_HP->Add( m_staticLabel_HP_IV, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_spinCtrl_HP_IV = new wxSpinCtrl( Layer2staticSizer_Stats->GetStaticBox(), wxID_ANY, wxT("31"), wxDefaultPosition, wxSize( -1,-1 ), wxSP_ARROW_KEYS, 0, 31, 31 );
	Layer3Sizer5_HP->Add( m_spinCtrl_HP_IV, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );


	Layer2staticSizer_Stats->Add( Layer3Sizer5_HP, 1, wxEXPAND, 5 );

	wxBoxSizer* Layer3Sizer_Atk;
	Layer3Sizer_Atk = new wxBoxSizer( wxHORIZONTAL );

	m_staticLabel_Attack_EV = new wxStaticText( Layer2staticSizer_Stats->GetStaticBox(), wxID_ANY, _("ATK EV"), wxDefaultPosition, wxSize( 100,-1 ), 0 );
	m_staticLabel_Attack_EV->Wrap( -1 );
	Layer3Sizer_Atk->Add( m_staticLabel_Attack_EV, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_spinCtrl_Attack_EV = new wxSpinCtrl( Layer2staticSizer_Stats->GetStaticBox(), wxID_ANY, wxT("0"), wxDefaultPosition, wxSize( -1,-1 ), wxSP_ARROW_KEYS, 0, 252, 7 );
	Layer3Sizer_Atk->Add( m_spinCtrl_Attack_EV, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_staticLabel_Attack_IV = new wxStaticText( Layer2staticSizer_Stats->GetStaticBox(), wxID_ANY, _("ATK IV"), wxDefaultPosition, wxSize( 100,-1 ), 0 );
	m_staticLabel_Attack_IV->Wrap( -1 );
	Layer3Sizer_Atk->Add( m_staticLabel_Attack_IV, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_spinCtrl_Attack_IV = new wxSpinCtrl( Layer2staticSizer_Stats->GetStaticBox(), wxID_ANY, wxT("31"), wxDefaultPosition, wxSize( -1,-1 ), wxSP_ARROW_KEYS, 0, 31, 29 );
	Layer3Sizer_Atk->Add( m_spinCtrl_Attack_IV, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );


	Layer2staticSizer_Stats->Add( Layer3Sizer_Atk, 1, wxEXPAND, 5 );

	wxBoxSizer* Layer3Sizer_Def;
	Layer3Sizer_Def = new wxBoxSizer( wxHORIZONTAL );

	m_staticLabel_Defence_EV = new wxStaticText( Layer2staticSizer_Stats->GetStaticBox(), wxID_ANY, _("DEF EV"), wxDefaultPosition, wxSize( 100,-1 ), 0 );
	m_staticLabel_Defence_EV->Wrap( -1 );
	Layer3Sizer_Def->Add( m_staticLabel_Defence_EV, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_spinCtrl_Defence_EV = new wxSpinCtrl( Layer2staticSizer_Stats->GetStaticBox(), wxID_ANY, wxT("0"), wxDefaultPosition, wxSize( -1,-1 ), wxSP_ARROW_KEYS, 0, 252, 7 );
	Layer3Sizer_Def->Add( m_spinCtrl_Defence_EV, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_staticLabel_Defence_IV = new wxStaticText( Layer2staticSizer_Stats->GetStaticBox(), wxID_ANY, _("DEF IV"), wxDefaultPosition, wxSize( 100,-1 ), 0 );
	m_staticLabel_Defence_IV->Wrap( -1 );
	Layer3Sizer_Def->Add( m_staticLabel_Defence_IV, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_spinCtrl_Defence_IV = new wxSpinCtrl( Layer2staticSizer_Stats->GetStaticBox(), wxID_ANY, wxT("31"), wxDefaultPosition, wxSize( -1,-1 ), wxSP_ARROW_KEYS, 0, 31, 27 );
	Layer3Sizer_Def->Add( m_spinCtrl_Defence_IV, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );


	Layer2staticSizer_Stats->Add( Layer3Sizer_Def, 1, wxEXPAND, 5 );

	wxBoxSizer* Layer3Sizer5_SpezAtk;
	Layer3Sizer5_SpezAtk = new wxBoxSizer( wxHORIZONTAL );

	m_staticLabel_SpezAtk_EV = new wxStaticText( Layer2staticSizer_Stats->GetStaticBox(), wxID_ANY, _("SPEZATK EV  "), wxDefaultPosition, wxSize( 100,-1 ), 0 );
	m_staticLabel_SpezAtk_EV->Wrap( -1 );
	Layer3Sizer5_SpezAtk->Add( m_staticLabel_SpezAtk_EV, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_spinCtrl_SpezAtk_EV = new wxSpinCtrl( Layer2staticSizer_Stats->GetStaticBox(), wxID_ANY, wxT("0"), wxDefaultPosition, wxSize( -1,-1 ), wxSP_ARROW_KEYS, 0, 252, 7 );
	Layer3Sizer5_SpezAtk->Add( m_spinCtrl_SpezAtk_EV, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_staticLabel_SpezAtk_IV = new wxStaticText( Layer2staticSizer_Stats->GetStaticBox(), wxID_ANY, _("SpezAtk IV"), wxDefaultPosition, wxSize( 100,-1 ), 0 );
	m_staticLabel_SpezAtk_IV->Wrap( -1 );
	Layer3Sizer5_SpezAtk->Add( m_staticLabel_SpezAtk_IV, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_spinCtrl_SpezAtk_IV = new wxSpinCtrl( Layer2staticSizer_Stats->GetStaticBox(), wxID_ANY, wxT("31"), wxDefaultPosition, wxSize( -1,-1 ), wxSP_ARROW_KEYS, 0, 31, 31 );
	Layer3Sizer5_SpezAtk->Add( m_spinCtrl_SpezAtk_IV, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );


	Layer2staticSizer_Stats->Add( Layer3Sizer5_SpezAtk, 1, wxEXPAND, 5 );

	wxBoxSizer* Layer3Sizer5_SpezDef;
	Layer3Sizer5_SpezDef = new wxBoxSizer( wxHORIZONTAL );

	m_staticLabel_SpezDef_EV = new wxStaticText( Layer2staticSizer_Stats->GetStaticBox(), wxID_ANY, _("SPEZDEF EV  "), wxDefaultPosition, wxSize( 100,-1 ), 0 );
	m_staticLabel_SpezDef_EV->Wrap( -1 );
	Layer3Sizer5_SpezDef->Add( m_staticLabel_SpezDef_EV, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_spinCtrl_SpezDef_EV = new wxSpinCtrl( Layer2staticSizer_Stats->GetStaticBox(), wxID_ANY, wxT("0"), wxDefaultPosition, wxSize( -1,-1 ), wxSP_ARROW_KEYS, 0, 252, 7 );
	Layer3Sizer5_SpezDef->Add( m_spinCtrl_SpezDef_EV, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_staticLabel_SpezDef_IV = new wxStaticText( Layer2staticSizer_Stats->GetStaticBox(), wxID_ANY, _("SpezDef IV"), wxDefaultPosition, wxSize( 100,-1 ), 0 );
	m_staticLabel_SpezDef_IV->Wrap( -1 );
	Layer3Sizer5_SpezDef->Add( m_staticLabel_SpezDef_IV, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_spinCtrl_SpezDef_IV = new wxSpinCtrl( Layer2staticSizer_Stats->GetStaticBox(), wxID_ANY, wxT("31"), wxDefaultPosition, wxSize( -1,-1 ), wxSP_ARROW_KEYS, 0, 31, 31 );
	Layer3Sizer5_SpezDef->Add( m_spinCtrl_SpezDef_IV, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );


	Layer2staticSizer_Stats->Add( Layer3Sizer5_SpezDef, 1, wxEXPAND, 5 );

	wxBoxSizer* Layer3Sizer5_Speed;
	Layer3Sizer5_Speed = new wxBoxSizer( wxHORIZONTAL );

	m_staticLabel_Speed_EV = new wxStaticText( Layer2staticSizer_Stats->GetStaticBox(), wxID_ANY, _("Speed EV  "), wxDefaultPosition, wxSize( 100,-1 ), 0 );
	m_staticLabel_Speed_EV->Wrap( -1 );
	Layer3Sizer5_Speed->Add( m_staticLabel_Speed_EV, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_spinCtrl_Speed_EV = new wxSpinCtrl( Layer2staticSizer_Stats->GetStaticBox(), wxID_ANY, wxT("0"), wxDefaultPosition, wxSize( -1,-1 ), wxSP_ARROW_KEYS, 0, 252, 7 );
	Layer3Sizer5_Speed->Add( m_spinCtrl_Speed_EV, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_staticLabel_Speed_IV = new wxStaticText( Layer2staticSizer_Stats->GetStaticBox(), wxID_ANY, _("Speed IV"), wxDefaultPosition, wxSize( 100,-1 ), 0 );
	m_staticLabel_Speed_IV->Wrap( -1 );
	Layer3Sizer5_Speed->Add( m_staticLabel_Speed_IV, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_spinCtrl_Speed_IV = new wxSpinCtrl( Layer2staticSizer_Stats->GetStaticBox(), wxID_ANY, wxT("31"), wxDefaultPosition, wxSize( -1,-1 ), wxSP_ARROW_KEYS, 0, 31, 31 );
	Layer3Sizer5_Speed->Add( m_spinCtrl_Speed_IV, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );


	Layer2staticSizer_Stats->Add( Layer3Sizer5_Speed, 1, wxEXPAND, 5 );


	Layer1Sizer4->Add( Layer2staticSizer_Stats, 0, 0, 5 );


	TopLevelSizer->Add( Layer1Sizer4, 0, 0, 5 );


	/*TopLevelSizer->Add( 0, 20, 0, wxEXPAND, 5 );

	m_buttonStart = new wxButton( this, wxID_ANY, _("Start "), wxDefaultPosition, wxDefaultSize, 0 );
	TopLevelSizer->Add( m_buttonStart, 0, wxALIGN_CENTER|wxALL, 5 );
*/
	wxBoxSizer* Layer1Sizer5;
	Layer1Sizer5 = new wxBoxSizer( wxHORIZONTAL );

	m_staticLabel_SLOT = new wxStaticText( this, wxID_ANY, _("Pokemon Slot"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticLabel_SLOT->Wrap( -1 );
	Layer1Sizer5->Add( m_staticLabel_SLOT, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_spinCtrl_Slot = new wxSpinCtrl( this, wxID_ANY, wxT("1"), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 6, 1 );
	Layer1Sizer5->Add( m_spinCtrl_Slot, 0, wxALL, 5 );

	m_buttonGenerate = new wxButton( this, wxID_ANY, _("Generate Pokemon"), wxDefaultPosition, wxDefaultSize, 0 );
	Layer1Sizer5->Add( m_buttonGenerate, 0, wxALL, 5 );

	m_PokemonSprite = new wxStaticBitmap( this, wxID_ANY, wxBitmap( wxT("PokemonSprites/bulbasaur/front.png"), wxBITMAP_TYPE_ANY ), wxDefaultPosition, wxSize( 100,100 ), 0 );
	Layer1Sizer5->Add( m_PokemonSprite, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );



	TopLevelSizer->Add( Layer1Sizer5, 0, wxEXPAND, 5 );

	wxBoxSizer* Layer1Sizer6;
	Layer1Sizer6 = new wxBoxSizer( wxHORIZONTAL );

	m_staticLabel_Start = new wxStaticText( this, wxID_ANY, _("Start Trading Server"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticLabel_Start->Wrap( -1 );
	Layer1Sizer6->Add( m_staticLabel_Start, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_buttonStart = new wxButton( this, wxID_ANY, _("Start "), wxDefaultPosition, wxDefaultSize, 0 );
	Layer1Sizer6->Add( m_buttonStart, 0, wxALIGN_CENTER|wxALL|wxALIGN_CENTER_VERTICAL, 5 );


	TopLevelSizer->Add( Layer1Sizer6, 0, wxEXPAND, 5 );


	this->SetSizer( TopLevelSizer );
	this->Layout();

	this->Centre( wxBOTH );

	// Connect Events
	m_buttonGenerate->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MyFrame::m_buttonGenerateOnButtonClick ), NULL, this );
	m_buttonStart->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MyFrame::m_buttonStartOnButtonClick ), NULL, this );
	
	m_spinCtrl_HP_EV->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( MyFrame::EVsOnSpin ), NULL, this );
	m_spinCtrl_Attack_EV->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( MyFrame::EVsOnSpin ), NULL, this );
	m_spinCtrl_Defence_EV->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( MyFrame::EVsOnSpin ), NULL, this );
	m_spinCtrl_SpezAtk_EV->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( MyFrame::EVsOnSpin ), NULL, this );
	m_spinCtrl_SpezDef_EV->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( MyFrame::EVsOnSpin ), NULL, this );
	m_spinCtrl_Speed_EV->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( MyFrame::EVsOnSpin ), NULL, this );

	m_Choice_Species->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( MyFrame::SpeciesOnChoice ), NULL, this );
}

MyFrame::~MyFrame()
{
}

void MyFrame::EVsOnSpin(wxSpinEvent& event)
{
    int total = 0;
	total = 
	m_spinCtrl_HP_EV->GetValue()+
	m_spinCtrl_Attack_EV->GetValue()+
	m_spinCtrl_Defence_EV->GetValue()+
	m_spinCtrl_SpezAtk_EV->GetValue()+
	m_spinCtrl_SpezDef_EV->GetValue()+
	m_spinCtrl_Speed_EV->GetValue();

    if (total > MaxEvValue) {
        wxSpinCtrl* Tempctrl = static_cast<wxSpinCtrl*>(event.GetEventObject());
        Tempctrl->SetValue(Tempctrl->GetValue() - 1);
        //wxMessageBox("The total EVs value cannot exceed 510.", "Limit Exceeded", wxOK | wxICON_INFORMATION);
    }
}
void MyFrame::SpeciesOnChoice(wxCommandEvent& event){

	wxString speciesChoice = m_Choice_Species->GetStringSelection();
	LoadSprite(speciesChoice);
}

void MyFrame::LoadSprite(const wxString& imageName){

	std::string lowercaseImageName = std::string(imageName.mb_str());
	lowercaseImageName[0] = tolower(lowercaseImageName[0]);
	std::cout << lowercaseImageName << "\n";
	wxString imagePath = ImageFolderPath + lowercaseImageName + "/front.png";
	std::cout << imagePath << "\n";

	if (wxFileExists(imagePath))
    {
        wxBitmap bitmap(imagePath, wxBITMAP_TYPE_PNG);
        m_PokemonSprite->SetBitmap(bitmap);
        Layout();
    }
    else
    {
        wxLogError("Image file %s not found!", imagePath);
    }

}

void MyFrame::m_buttonGenerateOnButtonClick(wxCommandEvent& event){
		//Get nickname from textfield
	wxString nickname_text = m_textCtrl_Nickname->GetValue();
	std::string stdNickname = std::string(nickname_text.mb_str());
	std::cout << stdNickname << "\n";

	//Get species as string from choice field
	wxString species_text = m_Choice_Species->GetString(m_Choice_Species->GetSelection());
	std::string stdSpecies = std::string(species_text.mb_str());
	std::cout << stdSpecies << "\n";

	//Deteermine if shiny or not
	bool Shiny = m_checkBox_Shiny->GetValue();
	int shinyvalue;
	if(Shiny){
		std::cout << "Shiny\n";
		shinyvalue = 1;
	}else{
		std::cout << "Not Shiny\n";
		shinyvalue = 0;
	}

	//Determine Level
	int Level = m_spinCtrl_Level->GetValue();
	std::cout << "Level : " << Level << "\n";

	//Gender
	int Gender = m_choice_Gender->GetSelection();
	wxString gender_text = m_choice_Gender->GetString(Gender);
	std::string stdGender = std::string(gender_text.mb_str());
	std::cout << "Gender : " << gender_text << " " << Gender << "\n";

	wxString natures_text = m_choice_Nature->GetString(m_choice_Nature->GetSelection());
	std::string stdNatures = std::string(natures_text.mb_str());
	std::cout << "Nature : " << stdNatures << "\n";

	wxString item_text = m_choice_Item->GetString(m_choice_Item->GetSelection());
	std::string stdItems = std::string(item_text.mb_str());
	std::cout << "Items : " << stdItems << "\n";

	int Ability = m_choice_Ability->GetSelection();
	wxString ability_text = m_choice_Ability->GetString(Ability);
	std::string stdAbility = std::string(ability_text.mb_str());
	std::cout << "Ability : " << ability_text << " " <<  Ability<< "\n";

	wxString move1_text = m_choice_Move1->GetString(m_choice_Move1->GetSelection());
	std::string stdMove1 = std::string(move1_text.mb_str());
	std::cout << "Move1 : " << stdMove1 << "\n";

	wxString move2_text = m_choice_Move2->GetString(m_choice_Move2->GetSelection());
	std::string stdMove2 = std::string(move2_text.mb_str());
	std::cout << "Move2 : " << stdMove2 << "\n";

	wxString move3_text = m_choice_Move3->GetString(m_choice_Move3->GetSelection());
	std::string stdMove3 = std::string(move3_text.mb_str());
	std::cout << "Move3 : " << stdMove3 << "\n";

	wxString move4_text = m_choice_Move4->GetString(m_choice_Move4->GetSelection());
	std::string stdMove4 = std::string(move4_text.mb_str());
	std::cout << "Move4 : " << stdMove4 << "\n";

	struct PokemonStructureGUI CPP_ARgs;
	struct PokemonStructureGUI *CPP_Args_Ptr = &CPP_ARgs;
	
	int HP_EV = m_spinCtrl_HP_EV->GetValue();
	int HP_IV = m_spinCtrl_HP_IV->GetValue();
	int ATK_EV = m_spinCtrl_Attack_EV->GetValue();
	int ATK_IV = m_spinCtrl_Attack_IV->GetValue();
	int DEF_EV = m_spinCtrl_Defence_EV->GetValue();
	int DEF_IV = m_spinCtrl_Defence_IV->GetValue();
	int SpezAtk_EV = m_spinCtrl_SpezAtk_EV->GetValue();
	int SpezAtk_IV = m_spinCtrl_SpezAtk_IV->GetValue();
	int SpezDef_EV = m_spinCtrl_SpezDef_EV->GetValue();
	int SpezDef_IV = m_spinCtrl_SpezDef_IV->GetValue();
	int Speed_EV = m_spinCtrl_Speed_EV->GetValue();
	int Speed_IV = m_spinCtrl_Speed_IV->GetValue();

	std::cout << "HP : " << HP_EV << " EVs - " << HP_IV << " IVs \n";
	std::cout << "Attack : " << ATK_EV << " EVs - " << ATK_IV << " IVs \n";
	std::cout << "Defence : " << DEF_EV << " EVs - " << DEF_IV << " IVs \n";
	std::cout << "SpezAtk : " << SpezAtk_EV << " EVs - " << SpezAtk_IV << " IVs \n";
	std::cout << "SpezDef : " << SpezDef_EV << " EVs - " << SpezDef_IV << " IVs \n";
	std::cout << "Speed : " << Speed_EV << " EVs - " << Speed_IV << " IVs \n";

	CPP_Args_Ptr->ev_HP = (uint16_t)HP_EV;
	CPP_Args_Ptr->ev_Attack = (uint16_t)ATK_EV;
	CPP_Args_Ptr->ev_Defense = (uint16_t)DEF_EV;
	CPP_Args_Ptr->ev_SpAttack = (uint16_t)SpezAtk_EV;
	CPP_Args_Ptr->ev_SpDefense = (uint16_t)SpezDef_EV;
	CPP_Args_Ptr->ev_Speed = (uint16_t)Speed_EV;
	CPP_Args_Ptr->iv_HP = (uint16_t)HP_IV;
	CPP_Args_Ptr->iv_Attack = (uint16_t)ATK_IV;
	CPP_Args_Ptr->iv_Defense = (uint16_t)DEF_IV;
	CPP_Args_Ptr->iv_SpAttack = (uint16_t)SpezAtk_IV;
	CPP_Args_Ptr->iv_SpDefense = (uint16_t)SpezDef_IV;
	CPP_Args_Ptr->iv_Speed = (uint16_t)Speed_IV;

	CPP_Args_Ptr->genderValue = Gender;
	CPP_Args_Ptr->shiny = shinyvalue;
	CPP_Args_Ptr->level = Level;
	CPP_Args_Ptr->ability = Ability;
	
	
	memcpy(CPP_Args_Ptr->nickname,stdNickname.c_str(),20);
	memcpy(CPP_Args_Ptr->item,stdItems.c_str(),20);
	memcpy(CPP_Args_Ptr->species,stdSpecies.c_str(),20);
	memcpy(CPP_Args_Ptr->nature,stdNatures.c_str(),20);
	memcpy(CPP_Args_Ptr->Move1,stdMove1.c_str(),20);
	memcpy(CPP_Args_Ptr->Move2,stdMove2.c_str(),20);
	memcpy(CPP_Args_Ptr->Move3,stdMove3.c_str(),20);
	memcpy(CPP_Args_Ptr->Move4,stdMove4.c_str(),20);

	std::cout << "Generated Pointer for C functions\n";

	int Slot = m_spinCtrl_Slot->GetValue()-1;
	createTransmitbuffer(Slot,CPP_Args_Ptr);
	std::string Message = "Successfully generated structure";
	wxMessageBox(Message,"info", wxOK | wxICON_INFORMATION);

}

void MyFrame::m_buttonStartOnButtonClick(wxCommandEvent& event){
	std::cout << "Start Server Function\n";
	main_C_function();
}

