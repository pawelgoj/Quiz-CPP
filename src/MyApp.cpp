#include "MyApp.hpp"
#include "MyFrame.h"
#include "Quiz.xpm"



// function on initialization of app
bool MyApp::OnInit()
{
    MyFrame *frame = new MyFrame("Quiz C++", wxPoint(50, 50), wxSize(450, 340));
    wxTaskBarIcon *taskBarIcon = new wxTaskBarIcon(); // icon on taskBar
    wxIcon *icon = new wxIcon(Quiz_xpm);
    frame->SetIcon(*icon);
    taskBarIcon->SetIcon(*icon);
    frame->Show(true);
    return true;
}
MyApp::~MyApp()
{
}

// wxIMPLEMENT_APP(MyApp); implement dynamic constructor for MyApp
wxIMPLEMENT_APP(MyApp);
