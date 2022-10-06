#include "questions.h"
#include <iostream>


Answer::Answer() {
            answer = "";
            isCorrect = false;
            letter = 'X';
        };
        
Answer::Answer(string x, bool y, char letter) {
            this->answer = x;
            this->isCorrect = y;
            this->letter = letter;
        };

string Answer::GetAnswer() {
        return answer;
    };

bool Answer::GetIsCorrect() {
        return isCorrect;
    };

char Answer::GetLetter() {
        return letter;
    };


Question::Question() {
        
        this->question = "Pytanie?";
        Answer answer1 = Answer("Correct answer!!!", true, 'A');
        Answer answer2 = Answer("Incorrect answer 1", false, 'B');
        Answer answer3 = Answer("Incorrect answer 2", false, 'C');
        Answer answer4 = Answer("Incorrect answer 3", false, 'D');
        this->id = 0;
        this->answers[0] = answer1;
        this->answers[1] = answer2;
        this->answers[2] = answer3;
        this->answers[3] = answer4;
    };

int Question::GetId() {
        return id;
    };

void Question::SetAnswers(Answer answers[4]) {
        int j = 0;
        for (int i = 0; i < 4; i++) {
            if (answers[i].isCorrect) {
                this->answers[0] = answers[i];
            } else {
                if (j == 0) {
                this->answers[1] = answers[i];
                }
                else if (j==1) {
                this->answers[2] = answers[i];
                }
                else if (j==2) {
                this->answers[3] = answers[i];
                }
                else {
                    throw "To many incorrect answers!!!!";
                }
                j++;
            }
        }
    };

void Question::SetQuestion(string x, int id) {
        question = x;
        this->id = id;
    };

Answer Question::GetCorrectAnswer(){
        for (int i=0; i < 4; i++) {
            if (this->answers[i].isCorrect) {
            return this->answers[i];
            }
        }
        
    };

string Question::GetQuestion() {
        return question;
    };

void Question::changeOrderOfAnswers() {
        
        Answer temp;
        for (int i = 0; i < 3; i++) {

            srand(time(NULL));
            int v = rand() % 3;

            temp = this->answers[i];
            this->answers[i] = this->answers[v];
            this->answers[v] = temp;
        }

        char letters[4] = {'A', 'B', 'C', 'D'};

        for (int i = 0; i < 3; i++) {
            this->answers[i].letter = letters[i];
        }
    };

Answer * Question::GetAnswers() {
        return answers;
    };


QuestionList::QuestionList(string fileName) {

            ifstream file(fileName);
            auto data = json::parse(file);
            file.close();

            for (int i = 0; i < data.size(); i++) {

                Question question;
                question.SetQuestion(data[i]["Question"], data[i]["id"]);
                auto Answers = data[i]["Answers"];
                char letters[4] = {'A', 'B', 'C', 'D'};
                Answer answersList[4];

                for (int i = 0; i < Answers.size(); i++) {

                    Answer answer;
                    answer.answer = Answers[i]["text"];

                    if ("true" == Answers[i]["Correct"]) {
                        answer.isCorrect = true;
                    } else {
                        answer.isCorrect = false;
                    }

                    answer.letter = letters[i];
                    answersList[i] = answer;
                }

                question.SetAnswers(answersList);
                this->questionsList.push_back(question);
            }
        };

void QuestionList::changeQuestionsOrder() {

            Question temp;

            for (int i = 0; i < questionsList.size(); i++) {

                srand(time(NULL));
                int v = rand() % 3;

                temp = questionsList[i];
                questionsList[i] = questionsList[v];
                questionsList[v] = temp;
            }
        };

vector<Question> QuestionList::getQuestions(int questionNumber) {
        vector<Question> out(questionNumber);
        //return slice of array
        copy(this->questionsList.begin(), this->questionsList.begin() + questionNumber,
            out.begin());
            return out;
        };



