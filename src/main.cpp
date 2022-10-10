#include "questions.h"
#include <iostream>
#include <cstdlib>
#include <Windows.h>

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::to_string;
using std::vector;

class CLI
{
private:
    string input;
    QuestionList question_list;

public:
    CLI()
    {
        this->input = "";
        this->question_list = QuestionList("Answers.json");
    };

    void Run()
    {

        enum UserState
        {
            Menu,
            Question,
            Summary
        };

        UserState userState = Menu;
        while (true)
        {
            if (input == "E" && Menu == userState)
            {
                showExitMessage();
                break;
            }
            else if (input == "A" && (Menu == userState || Question == userState))
            {
                userState = Question;
                QuestionList &g = this->question_list;
                int points = 0;

                auto question_list_v = prepareQuestions(g, 5);
                int length = question_list_v.size();

                for (int i = 0; i < length; i++)
                {
                    system("cls");
                    question_list_v[i].changeOrderOfAnswers();
                    showQuestion(input, question_list_v[i]);

                    if (input == "E" && Question == userState)
                    {
                        break;
                    }
                    else
                    {
                        string character;
                        // convert char to string
                        character.push_back(question_list_v[i].GetCorrectAnswer().GetLetter());

                        if (character == input)
                        {
                            cout << "Correct answer!!!" << endl;
                            Sleep(2000);
                            points++;
                        }
                        else
                        {
                            cout << "Incorrect answer!!!" << endl;
                            Sleep(2000);
                        }
                    }
                }
                system("cls");
                userState = Menu;
                showSummary(input, points, length);
            }
            else if ((Menu == userState && input == "") || (Question == userState && input == "E"))
            {
                system("cls"); // clear screen
                input = showMenu(input);
                userState = Menu;
            }
            else if (Menu == userState)
            {
                system("cls"); // clear screen
                input = showWrongInputMessageInMenu(input);
            }
            else
            {
                throw("Something went wrong!!!");
            }
        }
    }

private:
    static void showQuestion(string &input, Question &question)
    {

        cout << "C++ Millionaires \n"
             << endl;
        cout << question.GetQuestion() << endl;

        Answer *answers = question.GetAnswers();
        char chars[4] = {'A', 'B', 'C', 'D'};
        char ch;
        for (int i = 0; i < 4; i++)
        {
            while (true)
            {
                ch = answers->GetLetter();
                if (chars[i] == ch)
                {
                    cout << ch << " - " << answers->GetAnswer() << endl;
                    answers = question.GetAnswers();
                    break;
                }
                else
                {
                    answers++;
                }
            }
        }
        cout << "Chose answer 'A, B, C or D' " << endl;
        cout << "E - return to menu" << endl;
        cin >> input;
    }

private:
    static void showSummary(string &input, int &points, int &max_points)
    {
        cout << "C++ Millionaires \n"
             << endl;
        cout << "Your points is: " << points << "/" << max_points << endl;
        cout << "A - play again" << endl;
        cout << "E - exit" << endl;
        points = 0;
        cin >> input;
    }

private:
    static string showMenu(string input)
    {
        cout << "C++ Millionaires \n"
             << endl;
        cout << "What do you wont to do?" << endl;
        cout << "A - draw a question" << endl;
        cout << "E - exit" << endl;
        cin >> input;
        return input;
    }

private:
    static void showExitMessage()
    {
        cout << "Goodbye :)" << endl;
    }

private:
    static string showWrongInputMessageInMenu(string input)
    {
        cout << "Insert 'A' or 'E'" << endl;
        cin >> input;
        return input;
    }

private:
    static vector<Question> prepareQuestions(QuestionList &question_list, int questionNumber)
    {
        question_list.changeQuestionsOrder();
        vector<Question> list_of_questions = question_list.getQuestions(questionNumber);
        return list_of_questions;
    }
};

class cApp : public wxApp
{
public:
    cApp();
    ~cApp();

public:
    virtual bool OnInit();
};

/* Old cmd app.
int main()
{

    CLI cli;
    cli.Run();
    return 0;
}
*/