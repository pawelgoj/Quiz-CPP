#ifndef PYTANIE_H
#define PYTANIE_H

#include <iostream>
#include "json.hpp"
#include <fstream>
#include <vector>
#include <cstdlib>
#include <time.h>

/*
The question object is container for the string of the question , correct and incorrect answers. 
Also the correct answer letter is stored.
*/

using std::string;
using std::cout;
using std::cin;
using std::vector;
using json = nlohmann::json;
using std::endl;
using std::ifstream;
using std::copy;

class Answer {
    public:
        string answer;
        bool isCorrect;
        char letter;
        Answer();
        
        Answer(string x, bool y, char letter);

    string GetAnswer();

    bool GetIsCorrect();

    char GetLetter();
};

class Question {
    private:
    int id;
    string question; 
    Answer answers[4];
    public: 
    Question();

    int GetId();

    void SetAnswers(Answer answers[4]);

    void SetQuestion(string x, int id);

    Answer GetCorrectAnswer();

    string GetQuestion();

    void changeOrderOfAnswers();

    Answer * GetAnswers();
};

class QuestionList {
    public:
        vector<Question> questionsList;
        string fileName;
    QuestionList(string fileName="Answers.json");

    void changeQuestionsOrder();

    vector<Question> getQuestions(int questionNumber);

};

#endif



