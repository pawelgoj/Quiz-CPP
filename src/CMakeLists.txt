cmake_minimum_required(VERSION 3.6 FATAL_ERROR)

project(wx_cmake_template_core LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# hack for buggy CMake's FindwxWidgets script
if (DEFINED ENV_WX_CONFIG)
    set (ENV{WX_CONFIG} ${ENV_WX_CONFIG})
endif()

add_compile_options(/Zi /MDd)
#flags for msvc compilation /Zi - Generates complete debugging information
# /MDd - Use /MDd or /MTd to define _DEBUG and allow _CrtDumpMemoryLeaks()

set(wxWidgets_USE_STATIC 1)

find_package(wxWidgets REQUIRED)


set(SRCS MyApp.cpp MyFrame.cpp questions.cpp ../libs/nlohmann_json/json.hpp)


include(${wxWidgets_USE_FILE})

# the WIN32 is needed for Windows in order for it to look for WinMain
# instead of the main function. This is ignored on other systems,
# so it works on all platforms
add_executable(MyApp WIN32 ${SRCS})


target_link_libraries(MyApp PRIVATE ${wxWidgets_LIBRARIES})