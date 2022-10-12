// For compilers that support precompilation, includes "wx/wx.h".
#include "questions.h"
#include <cstdlib>
#include <string>
#include <wx/utils.h>
#include <chrono> //e.g. sleep function
#include <thread>

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

// bellow code for debug memory Leaks
#ifdef __WXMSW__
#include <wx/msw/msvcrt.h> // redefines the new() operator
#endif

using std::cout;
using std::endl;
using std::string;
using std::thread;
using std::to_string;
using std::vector;

class MyApp : public wxApp
{
public:
    // function on initialization
    virtual bool OnInit();
    ~MyApp();
};
class MyFrame : public wxFrame
{
public:
    MyFrame(const wxString &title, const wxPoint &pos, const wxSize &size);

private:
    vector<Question> listOfQuestions;
    wxButton *m_btn1 = nullptr;
    wxButton *correctButton = nullptr;
    wxStaticText *m_txt1 = nullptr;
    wxGridSizer *grid = nullptr;
    wxColor inCorrectColour = wxColour(220, 20, 60, 0);
    wxColor correctColour = wxColour(65280);
    wxColor questionBackgroundColor = wxColour(240, 248, 255, 0);
    u_int timeToShowResponseToUser = 1000;
    u_int currentQuestion = 0;
    u_int inCorrectButtonIDList[3] = {32, 33, 34};
    u_int numberOfPoints = 0;
    u_int numberOfQuestions = 10;
    bool buttonClicked = false;

private:
    void OnHello(wxCommandEvent &event);
    void OnExit(wxCommandEvent &event);
    void OnAbout(wxCommandEvent &event);
    void OnCorrectButtonClicked(wxCommandEvent &event);
    void OnInCorrectButtonClicked(wxCommandEvent &event);
    void OnButtonStartClicked(wxCommandEvent &event);
    void OnWeatherApp(wxCommandEvent &event);
    void OnRustyCrossword(wxCommandEvent &event);
    void WaitToShowResponseToUserThread();
    std::thread backgroundThread;
    wxDECLARE_EVENT_TABLE();

public:
    void prepareQuestionAndAnswersButtons(Question question, u_int *buttonIDList);
    void theEndFrameStateIfUserAnswerAllQuestions();
};

enum
{
    ID_RustyCrossword = 1,
    ID_WeatherAPP = 2,
    ID_CorrectButton = 31,
    ID_StartButton = 20
};

wxIMPLEMENT_APP(MyApp);
// wxIMPLEMENT_APP(MyApp); implement dynamic constructor for MyApp

// event table macros
wxBEGIN_EVENT_TABLE(MyFrame, wxFrame)
    EVT_BUTTON(ID_StartButton, MyFrame::OnButtonStartClicked)
        EVT_BUTTON(ID_CorrectButton, MyFrame::OnCorrectButtonClicked)
            EVT_BUTTON(32, MyFrame::OnInCorrectButtonClicked)
                EVT_BUTTON(33, MyFrame::OnInCorrectButtonClicked)
                    EVT_BUTTON(34, MyFrame::OnInCorrectButtonClicked)
                        EVT_MENU(wxID_EXIT, MyFrame::OnExit)
                            EVT_MENU(wxID_ABOUT, MyFrame::OnAbout)
                                EVT_MENU(ID_RustyCrossword, MyFrame::OnRustyCrossword)
                                    EVT_MENU(ID_WeatherAPP, MyFrame::OnWeatherApp)
                                        wxEND_EVENT_TABLE()

    // function on initialization of app
    bool MyApp::OnInit()
{
    MyFrame *frame = new MyFrame("Millionaires C++", wxPoint(50, 50), wxSize(450, 340));
    frame->Show(true);
    return true;
}
MyApp::~MyApp()
{
}
MyFrame::MyFrame(const wxString &title, const wxPoint &pos, const wxSize &size)
    : wxFrame(NULL, wxID_ANY, title, pos, size)
{
    // Crate menu
    wxMenu *menuOtherProjects = new wxMenu;
    menuOtherProjects->Append(ID_RustyCrossword, "&Rusty Crossword",
                              "cmd crossword written in Rust");
    menuOtherProjects->AppendSeparator();
    menuOtherProjects->Append(ID_WeatherAPP, "&Weather APP",
                              "Weather APP is web widget using openweather API");
    wxMenu *menuHelp = new wxMenu;
    menuHelp->Append(wxID_ABOUT);
    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append(menuHelp, "&Help");
    menuBar->Append(menuOtherProjects, "&Other projects");
    SetMenuBar(menuBar);
    CreateStatusBar();
    SetStatusText("Millionaires C++");
    // create grid for widgets
    this->grid = new wxGridSizer(2, 1, 0, 0);

    // Crate text filed
    // To achieve text vertically centered wxPanel is used.
    wxPanel *pseudoText = new wxPanel(this, wxID_ANY, wxDefaultPosition,
                                      wxDefaultSize, wxBORDER_THEME | wxTAB_TRAVERSAL);
    this->m_txt1 = new wxStaticText(pseudoText, 21, "Millionaires C++", wxDefaultPosition,
                                    wxDefaultSize, wxALIGN_CENTRE_HORIZONTAL);
    this->m_txt1->SetBackgroundColour(this->questionBackgroundColor);
    pseudoText->SetBackgroundColour(m_txt1->GetBackgroundColour());

    wxBoxSizer *szr = new wxBoxSizer(wxVERTICAL);
    szr->AddStretchSpacer();
    szr->Add(m_txt1, wxSizerFlags().Expand());
    szr->AddStretchSpacer();
    pseudoText->SetSizer(szr);

    this->grid->Add(pseudoText, 1, wxEXPAND | wxALL);
    // Create button
    this->m_btn1 = new wxButton(this, ID_StartButton, "Start game");
    // Add button to grid, expand button to fill all space of cal
    this->grid->Add(m_btn1, 1, wxEXPAND | wxALL);
    this->SetSizer(grid);

    this->m_txt1->SetFont(wxFontInfo(12).Bold());
    this->grid->Layout();
    srand(time(NULL));
}
void MyFrame::OnRustyCrossword(wxCommandEvent &event)
{
    wxLaunchDefaultBrowser("https://github.com/pawelgoj/Rusty_Crossword");
}
void MyFrame::OnWeatherApp(wxCommandEvent &event)
{
    wxLaunchDefaultBrowser("https://github.com/pawelgoj/Weather-APP");
}
void MyFrame::OnExit(wxCommandEvent &event)
{
    Close(true);
}
void MyFrame::OnAbout(wxCommandEvent &event)
{
    wxMessageBox("This is quiz about C++",
                 "About C++", wxOK | wxICON_INFORMATION);
}

void MyFrame::OnButtonStartClicked(wxCommandEvent &event)
{
    // Add String to list from text box.
    // draw a question
    // prepare answers

    QuestionList questionList = QuestionList(".\\Answers.json");

    questionList.changeQuestionsOrder();

    this->listOfQuestions = questionList.getQuestions(this->numberOfQuestions); // will be 10 questions

    Question question = listOfQuestions[0];

    MyFrame::prepareQuestionAndAnswersButtons(question, this->inCorrectButtonIDList);
    // finish event
    this->currentQuestion++;
}
void MyFrame::prepareQuestionAndAnswersButtons(Question question, u_int *inCorrectButtonIDList)
{
    if (this->buttonClicked)
    {
        this->buttonClicked = false;
        this->backgroundThread.join();
    }
    auto answers = question.GetAnswers();

    this->grid->Clear(true);
    this->grid->SetCols(1);
    this->grid->SetRows(2);
    wxGridSizer *inGrid = new wxGridSizer(2, 2, 0, 0);

    // random order of questions
    Answer temp;
    for (int i = 0; i < 4; i++)
    {
        int index = rand() % 4;
        temp = answers[index];
        answers[index] = answers[i];
        answers[i] = temp;
    };
    wxButton *buttonList[4];
    string answerChars[4] = {"A-", "B-", "C-", "D-"};
    u_int j = 0;

    for (int i = 0; i < 4; i++)
    {
        if (answers[i].GetIsCorrect())
        {
            this->correctButton = new wxButton(this, ID_CorrectButton, answerChars[i].append(answers[i].GetAnswer()), wxDefaultPosition,
                                               wxDefaultSize);
            inGrid->Add(this->correctButton, 1, wxEXPAND | wxALL);
        }
        else
        {
            if (j > 2)
            {
                throw std::runtime_error("to many wrong answers!!!!");
            };
            buttonList[j] = new wxButton(this, *inCorrectButtonIDList + j, answerChars[i].append(answers[i].GetAnswer()), wxDefaultPosition,
                                         wxDefaultSize);
            inGrid->Add(buttonList[j], 1, wxEXPAND | wxALL);
            j++;
        };
    };

    wxString questionString = question.GetQuestion();
    // The flag wxST_NO_AUTORESIZE is must be used with wxALIGN_CENTRE_HORIZONTAL then
    // SetLabel work correctly. The size of wxStaticText not resise while Method SetLabel is used.
    m_txt1 = new wxStaticText(this, 21, questionString, wxDefaultPosition,
                              wxDefaultSize, wxALIGN_CENTRE_HORIZONTAL | wxST_NO_AUTORESIZE);
    m_txt1->SetBackgroundColour(this->questionBackgroundColor);
    m_txt1->SetForegroundColour(wxColour(0, 0, 0, 0));
    m_txt1->SetFont(wxFontInfo(12).Bold());

    this->grid->Add(m_txt1, 1, wxEXPAND | wxALL);
    this->grid->Add(inGrid, 1, wxEXPAND | wxALL);
    // Force the layout of children anew
    this->grid->Layout();
}

void MyFrame::OnCorrectButtonClicked(wxCommandEvent &event)
{
    if (this->buttonClicked)
    {
        event.Skip();
    }
    else
    {
        this->numberOfPoints++;
        this->buttonClicked = true;
        wxBeginBusyCursor();
        this->m_txt1->SetLabel("Correct answer!!!!");
        wxObject *obj = event.GetEventObject();
        ((wxButton *)obj)->SetBackgroundColour(this->correctColour);
        // this->correctButton->Disable();
        ((wxButton *)obj)->Enable(false);
        ((wxButton *)obj)->Update();
        this->grid->Layout();
        // make second thread to prepare next question.
        this->backgroundThread = std::thread{&MyFrame::WaitToShowResponseToUserThread, this};
    }
}

void MyFrame::WaitToShowResponseToUserThread()
{ // wait 2s
    if (this->numberOfQuestions > this->currentQuestion)
    {
        wxThread::Sleep(1000);
    };
    wxEndBusyCursor();

    wxGetApp().CallAfter([this]()
                         {
    if(this->numberOfQuestions == this->currentQuestion) {

        MyFrame::theEndFrameStateIfUserAnswerAllQuestions();
    } else {
        Question question = listOfQuestions[this->currentQuestion];
        this->currentQuestion++;
        MyFrame::prepareQuestionAndAnswersButtons(question, this->inCorrectButtonIDList);
    };
    this->buttonClicked = false; });
}

void MyFrame::theEndFrameStateIfUserAnswerAllQuestions()
{
    this->buttonClicked = false;
    this->backgroundThread.join();
    this->grid->Clear(true);
    this->grid->SetCols(1);
    this->grid->SetRows(2);

    // To achieve text vertically centered wxPanel is used.
    wxPanel *pseudoText = new wxPanel(this, wxID_ANY, wxDefaultPosition,
                                      wxDefaultSize, wxBORDER_THEME | wxTAB_TRAVERSAL);

    string strPoints = to_string(this->numberOfPoints);
    string strQuestions = to_string(this->numberOfQuestions);
    string text = "Your score: ";

    this->m_txt1 = new wxStaticText(pseudoText, 21, text.append(strPoints).append("/").append(strQuestions), wxDefaultPosition,
                                    wxDefaultSize, wxALIGN_CENTRE_HORIZONTAL);
    this->m_txt1->SetBackgroundColour(wxColour(240, 248, 255, 0));
    pseudoText->SetBackgroundColour(m_txt1->GetBackgroundColour());
    this->m_txt1->SetFont(wxFontInfo(12).Bold());
    wxBoxSizer *szr = new wxBoxSizer(wxVERTICAL);
    szr->AddStretchSpacer();
    szr->Add(m_txt1, wxSizerFlags().Expand());
    szr->AddStretchSpacer();
    pseudoText->SetSizer(szr);

    this->grid->Add(pseudoText, 1, wxEXPAND | wxALL);
    // Create button
    this->m_btn1 = new wxButton(this, 20, "Play again.");
    // Add button to grid, expand button to fill all space of cal
    this->grid->Add(m_btn1, 1, wxEXPAND | wxALL);
    this->currentQuestion = 0;
    this->numberOfPoints = 0;
    this->grid->Layout();
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
        this->correctButton->SetBackgroundColour(this->correctColour);
        this->m_txt1->SetLabel("Incorrect answer!!!!");
        ((wxButton *)obj)->Enable(false);
        ((wxButton *)obj)->Update();
        this->grid->Layout();
        this->backgroundThread = std::thread{&MyFrame::WaitToShowResponseToUserThread, this};
    }
}
