// For compilers that support precompilation, includes "wx/wx.h".
#include "questions.h"
#include <cstdlib>
#include <string>
#include <wx/utils.h>
#include <mutex>
#include <chrono> //e.g. sleep function
#include <thread>

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

using std::cout;
using std::endl;
using std::string;
using std::thread;
using std::vector;

class MyApp : public wxApp
{
public:
    // function on initialization
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
    u_int timeToShowResponseToUser = 1000;
    u_int currentQuestion = 0;
    u_int buttonIDList[4] = {31, 32, 33, 34};
    bool buttonClicked = false;

private:
    void OnHello(wxCommandEvent &event);
    void OnExit(wxCommandEvent &event);
    void OnAbout(wxCommandEvent &event);
    void OnCorrectButtonClicked(wxCommandEvent &event);
    void OnInCorrectButtonClicked(wxCommandEvent &event);
    void OnButtonStartClicked(wxCommandEvent &event);
    void WaitToShowResponseToUserThread();
    std::mutex dataMutex;
    std::thread backgroundThread;
    wxDECLARE_EVENT_TABLE();

public:
    void prepareQuestionAndAnswersButtons(Question question, u_int *buttonIDList);
};

enum
{
    ID_Hello = 1
};

wxIMPLEMENT_APP(MyApp);
// wxIMPLEMENT_APP(MyApp); implement dynamic constructor for MyApp

// event table macros
wxBEGIN_EVENT_TABLE(MyFrame, wxFrame)
    EVT_BUTTON(20, MyFrame::OnButtonStartClicked)
        EVT_BUTTON(31, MyFrame::OnCorrectButtonClicked)
            EVT_BUTTON(32, MyFrame::OnInCorrectButtonClicked)
                EVT_BUTTON(33, MyFrame::OnInCorrectButtonClicked)
                    EVT_BUTTON(34, MyFrame::OnInCorrectButtonClicked)
                        EVT_MENU(ID_Hello, MyFrame::OnHello)
                            EVT_MENU(wxID_EXIT, MyFrame::OnExit)
                                EVT_MENU(wxID_ABOUT, MyFrame::OnAbout)

                                    wxEND_EVENT_TABLE()

    // function on initialization of app
    bool MyApp::OnInit()
{
    MyFrame *frame = new MyFrame("Millionaires C++", wxPoint(50, 50), wxSize(450, 340));
    frame->Show(true);
    return true;
}
MyFrame::MyFrame(const wxString &title, const wxPoint &pos, const wxSize &size)
    : wxFrame(NULL, wxID_ANY, title, pos, size)
{
    // Crate menu
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
    // create grid for widgets
    grid = new wxGridSizer(2, 1, 0, 0);

    // Crate text filed

    // To achieve text vertically centered wxPanel is used.
    wxPanel *pseudoText = new wxPanel(this, wxID_ANY, wxDefaultPosition,
                                      wxDefaultSize, wxBORDER_THEME | wxTAB_TRAVERSAL);
    m_txt1 = new wxStaticText(pseudoText, 21, "Millionaires C++", wxDefaultPosition,
                              wxDefaultSize, wxALIGN_CENTRE_HORIZONTAL);
    m_txt1->SetBackgroundColour(wxColour(240, 248, 255, 0));
    pseudoText->SetBackgroundColour(m_txt1->GetBackgroundColour());

    wxBoxSizer *szr = new wxBoxSizer(wxVERTICAL);
    szr->AddStretchSpacer();
    szr->Add(m_txt1, wxSizerFlags().Expand());
    szr->AddStretchSpacer();
    pseudoText->SetSizer(szr);

    grid->Add(pseudoText, 1, wxEXPAND | wxALL);
    // Create button
    m_btn1 = new wxButton(this, 20, "Start game");
    // Add button to grid, expand button to fill all space of cal
    grid->Add(m_btn1, 1, wxEXPAND | wxALL);
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
    // Add String to list from text box.
    // draw a question
    // prepare answers

    QuestionList questionList = QuestionList(".\\Answers.json");

    questionList.changeQuestionsOrder();

    this->listOfQuestions = questionList.getQuestions(7); // will be 10 questions

    Question question = listOfQuestions[0];

    MyFrame::prepareQuestionAndAnswersButtons(question, this->buttonIDList);
    // finish event
    this->currentQuestion = this->currentQuestion + 1;
}
void MyFrame::prepareQuestionAndAnswersButtons(Question question, u_int *buttonIDList)
{
    if (this->buttonClicked)
    {
        this->buttonClicked = false;
        this->backgroundThread.join();
    }
    auto answers = question.GetAnswers();
    grid->Clear(true);
    grid->SetCols(1);
    grid->SetRows(2);
    wxGridSizer *inGrid = new wxGridSizer(2, 2, 0, 0);
    string answerChar = "A-";
    buttonA = new wxButton(this, *buttonIDList, answerChar.append(answers[0].GetAnswer()), wxDefaultPosition,
                           wxDefaultSize);
    inGrid->Add(buttonA, 1, wxEXPAND | wxALL);
    answerChar = "B-";
    buttonB = new wxButton(this, *(buttonIDList + 1), answerChar.append(answers[1].GetAnswer()), wxDefaultPosition,
                           wxDefaultSize);
    inGrid->Add(buttonB, 1, wxEXPAND | wxALL);
    answerChar = "C-";
    buttonC = new wxButton(this, *(buttonIDList + 2), answerChar.append(answers[2].GetAnswer()), wxDefaultPosition,
                           wxDefaultSize);
    inGrid->Add(buttonC, 1, wxEXPAND | wxALL);
    answerChar = "D-";
    buttonD = new wxButton(this, *(buttonIDList + 3), answerChar.append(answers[3].GetAnswer()), wxDefaultPosition,
                           wxDefaultSize);
    inGrid->Add(buttonD, 1, wxEXPAND | wxALL);

    wxString questionString = question.GetQuestion();
    // The flag wxST_NO_AUTORESIZE is must be used with wxALIGN_CENTRE_HORIZONTAL then
    // SetLabel work correctly. The size of wxStaticText not resise while Method SetLabel is used.
    m_txt1 = new wxStaticText(this, 21, questionString, wxDefaultPosition,
                              wxDefaultSize, wxALIGN_CENTRE_HORIZONTAL | wxST_NO_AUTORESIZE);
    m_txt1->SetBackgroundColour(wxColour(240, 248, 255, 0));
    m_txt1->SetForegroundColour(wxColour(0, 0, 0, 0));
    m_txt1->SetFont(wxFontInfo(12).Bold());

    grid->Add(m_txt1, 1, wxEXPAND | wxALL);
    grid->Add(inGrid, 1, wxEXPAND | wxALL);
    // Force the layout of children anew
    grid->Layout();
}

void MyFrame::OnCorrectButtonClicked(wxCommandEvent &event)
{
    if (this->buttonClicked)
    {
        event.Skip();
    }
    else
    {
        this->buttonClicked = true;
        wxBeginBusyCursor();
        m_txt1->SetLabel("Correct answer!!!!");
        this->buttonA->SetBackgroundColour(this->correctColour);
        // this->buttonA->Disable();
        this->buttonA->Enable(false);
        this->buttonA->Update();
        grid->Layout();
        // Czeka na inne wiadomości z gui
        this->backgroundThread = std::thread{&MyFrame::WaitToShowResponseToUserThread, this};
        // wait 1s
    }
}

void MyFrame::WaitToShowResponseToUserThread()
{ // wait in msdad
    wxThread::Sleep(2000);
    wxEndBusyCursor();

    wxGetApp().CallAfter([this]()
                         {
    Question question = listOfQuestions[this->currentQuestion];
    this->currentQuestion = this->currentQuestion + 1;
    MyFrame::prepareQuestionAndAnswersButtons(question, this->buttonIDList);
    this->buttonClicked = false; });
}

void MyFrame::OnInCorrectButtonClicked(wxCommandEvent &event)
{
    if (this->buttonClicked)
    {
        event.Skip();
    }
    else
    {
        this->buttonClicked = true;
        wxBeginBusyCursor();
        wxObject *obj = event.GetEventObject();
        ((wxButton *)obj)->SetBackgroundColour(this->inCorrectColour);
        buttonA->SetBackgroundColour(this->correctColour);
        m_txt1->SetLabel("Incorrect answer!!!!");
        ((wxButton *)obj)->Enable(false);
        ((wxButton *)obj)->Update();
        grid->Layout();
        // wait 1s
        this->backgroundThread = std::thread{&MyFrame::WaitToShowResponseToUserThread, this};
    }
}
/*
class MyThread : public wxThread
{
public:
    MyThread(MyFrame *handler)
        : wxThread(wxTHREAD_DETACHED)
        { m_pHandler = handler }
    ~MyThread();
protected:
    virtual ExitCode Entry();
    MyFrame *m_pHandler;
};
*/
