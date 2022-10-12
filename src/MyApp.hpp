#include <wx/app.h>
#include <wx/taskbar.h>

// bellow code for debug memory Leaks
#ifdef __WXMSW__
#include <wx/msw/msvcrt.h> // redefines the new() operator
#endif

class MyApp : public wxApp
{
public:
    // function on initialization
    virtual bool OnInit();
    ~MyApp();
};

// the place of macro is very important must be between definition and implementation
wxDECLARE_APP(MyApp);
