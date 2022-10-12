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

using std::cout;
using std::endl;
using std::string;
using std::thread;
using std::to_string;
using std::vector;

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
    void prepareQuestionAndAnswersButtons(Question question);
    void theEndFrameStateIfUserAnswerAllQuestions();
};
