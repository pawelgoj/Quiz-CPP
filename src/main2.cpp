// For compilers that support precompilation, includes "wx/wx.h".
#include "questions.h"
#include <cstdlib>

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

using std::string;
using std::cout;
using std::endl;

class MyApp : public wxApp
{
public:
    //function on initialization  
    virtual bool OnInit();
};
class MyFrame : public wxFrame
{
public:
    MyFrame(const wxString &title, const wxPoint &pos, const wxSize &size);

public:
    vector<Question> listOfQuestions;
    wxButton *m_btn1 = nullptr;
    wxButton *buttonA = nullptr;
    wxButton *buttonB = nullptr;
    wxButton *buttonC = nullptr;
    wxButton *buttonD = nullptr;
    wxStaticText *m_txt1 = nullptr; 
    wxGridSizer *grid = nullptr;
    wxColor inCorrectColour = wxColour(220, 20, 60, 0);
    wxColor correctColour = wxColour(65280);


private:
    void OnHello(wxCommandEvent &event);
    void OnExit(wxCommandEvent &event);
    void OnAbout(wxCommandEvent &event);
    void OnCorrectButtonClicked(wxCommandEvent &event);
    void OnInCorrectButtonClicked(wxCommandEvent &event);
    void OnButtonStartClicked(wxCommandEvent &event);
    wxDECLARE_EVENT_TABLE();
};

enum
{
    ID_Hello = 1
};

//event table macros 
wxBEGIN_EVENT_TABLE(MyFrame, wxFrame)
    EVT_BUTTON(20, MyFrame::OnButtonStartClicked)
    EVT_BUTTON(31, MyFrame::OnCorrectButtonClicked)
    EVT_BUTTON(32, MyFrame::OnInCorrectButtonClicked)
    EVT_MENU(ID_Hello, MyFrame::OnHello)
        EVT_MENU(wxID_EXIT, MyFrame::OnExit)
    EVT_MENU(wxID_ABOUT, MyFrame::OnAbout)
    
wxEND_EVENT_TABLE()
                    wxIMPLEMENT_APP(MyApp);

//function on initialization of app
bool MyApp::OnInit()
{
    MyFrame *frame = new MyFrame("Millionaires C++", wxPoint(50, 50), wxSize(450, 340));
    frame->Show(true);
    return true;
}
MyFrame::MyFrame(const wxString &title, const wxPoint &pos, const wxSize &size)
    : wxFrame(NULL, wxID_ANY, title, pos, size)
{
    //Crate menu 
    wxMenu *menuFile = new wxMenu;
    menuFile->Append(ID_Hello, "&Hello...\tCtrl-H",
                     "Help string shown in status bar for this menu item");
    menuFile->AppendSeparator();
    menuFile->Append(wxID_EXIT);
    wxMenu *menuHelp = new wxMenu;
    menuHelp->Append(wxID_ABOUT);
    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append(menuFile, "&File");
    menuBar->Append(menuHelp, "&Help");
    SetMenuBar(menuBar);
    CreateStatusBar();
    SetStatusText("Millionaires C++");
    //create grid for widgets 
    grid = new wxGridSizer(2, 1, 0, 0);

    //Crate text filed 

    //To achieve text vertically centered wxPanel is used.
    wxPanel* pseudoText = new wxPanel(this, wxID_ANY, wxDefaultPosition,
    wxDefaultSize, wxBORDER_THEME|wxTAB_TRAVERSAL);
    m_txt1 = new wxStaticText(pseudoText, 21, "Millionaires C++", wxDefaultPosition, 
    wxDefaultSize, wxALIGN_CENTRE_HORIZONTAL);
    m_txt1->SetBackgroundColour(wxColour(240, 248, 255, 0));
    pseudoText->SetBackgroundColour(m_txt1->GetBackgroundColour());

    wxBoxSizer* szr = new wxBoxSizer(wxVERTICAL);
    szr->AddStretchSpacer();
    szr->Add(m_txt1, wxSizerFlags().Expand());
    szr->AddStretchSpacer();
    pseudoText->SetSizer(szr);
    
    grid->Add(pseudoText, 1, wxEXPAND | wxALL );
    //Create button 
    m_btn1 = new wxButton(this, 20, "Start game");
    //Add button to grid, expand button to fill all space of cal 
    grid->Add(m_btn1, 1, wxEXPAND | wxALL );
    this->SetSizer(grid);

    m_txt1->SetFont(wxFontInfo(12).Bold());
    grid->Layout();
    
    
}
void MyFrame::OnExit(wxCommandEvent &event)
{
    Close(true);
}
void MyFrame::OnAbout(wxCommandEvent &event)
{
    wxMessageBox("This is a wxWidgets' Hello world sample",
                 "About Hello World", wxOK | wxICON_INFORMATION);
}
void MyFrame::OnHello(wxCommandEvent &event)
{
    wxLogMessage("Millionaires C++!");
}

void MyFrame::OnButtonStartClicked(wxCommandEvent &event)
{
    //Add String to list from text box.
    //draw a question 
    //prepare answers 

    QuestionList questionList = QuestionList(".\\Answers.json");
    
    questionList.changeQuestionsOrder();

    this->listOfQuestions = questionList.getQuestions(7); //will be 10 questions 

    grid->Clear(true);
    grid->SetCols(1);
    grid->SetRows(2);

    Question question = listOfQuestions[0];
    wxGridSizer *inGrid = new wxGridSizer(2, 2, 0, 0);
    buttonA = new wxButton(this, 31, "A", wxDefaultPosition, 
    wxDefaultSize);
    inGrid->Add(buttonA, 1, wxEXPAND | wxALL);
    buttonB = new wxButton(this, 32, "B", wxDefaultPosition, 
    wxDefaultSize);
    inGrid->Add(buttonB, 1, wxEXPAND | wxALL);
    buttonC = new wxButton(this, 33, "C", wxDefaultPosition, 
    wxDefaultSize);
    inGrid->Add(buttonC, 1, wxEXPAND | wxALL);
    buttonD = new wxButton(this, 34, "D", wxDefaultPosition, 
    wxDefaultSize);
    inGrid->Add(buttonD, 1, wxEXPAND | wxALL);

    wxString questionString = question.GetQuestion();
    m_txt1 = new wxStaticText(this, 21, questionString, wxDefaultPosition, 
    wxDefaultSize, wxALIGN_CENTRE_HORIZONTAL);
    m_txt1->SetBackgroundColour(wxColour(240, 248, 255, 0));
    m_txt1->SetFont(wxFontInfo(12).Bold());

    grid->Add(m_txt1, 1, wxEXPAND | wxALL);
    grid->Add(inGrid, 1, wxEXPAND | wxALL);
    //Force the layout of children anew 
    grid->Layout();
    //finish event 
    event.Skip();
}

void MyFrame::OnCorrectButtonClicked(wxCommandEvent &event)
{
    buttonA->SetBackgroundColour(this->correctColour); 
    m_txt1->SetLabel("Correct answer!!!!");
    grid->Layout();
}

void MyFrame::OnInCorrectButtonClicked(wxCommandEvent &event)
{
    buttonB->SetBackgroundColour(this->inCorrectColour);
    buttonA->SetBackgroundColour(this->correctColour);
}

