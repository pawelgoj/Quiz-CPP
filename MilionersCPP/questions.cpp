#include "questions.h"


Answer::Answer() {
            //Aby wykorzystać obiekty w innych klasach potrzebny jest konstruktor domyślny 
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
        this->correctAnswer = Answer("Correct answer!!!", true, 'A');
        this->answerIncorect1 = Answer("Incorrect answer 1", false, 'B');
        this->answerIncorect2 = Answer("Incorrect answer 2", false, 'C');
        this->answerIncorect3 = Answer("Incorrect answer 3", false, 'D');
        this->id = 0;
        this->answers[0] = correctAnswer;
        this->answers[1] = answerIncorect1;
        this->answers[2] = answerIncorect2;
        this->answers[3] = answerIncorect3;
    };

int Question::GetId() {
        return id;
    };

void Question::SetAnswers(Answer answers[4]) {
        int j = 0;
        for (int i = 0; i < 4; i++) {
            if (answers[i].isCorrect) {
                this->correctAnswer = answers[i];
                this->answers[0] = correctAnswer;
            } else {
                if (j == 0) {
                    answerIncorect1 = answers[i];
                this->answers[1] = answerIncorect1;
                }
                else if (j==1) {
                this->answerIncorect2 = answers[i];
                this->answers[2] = answerIncorect2;
                }
                else if (j==2) {
                this->answerIncorect2 = answers[i];
                this->answers[3] = answerIncorect3;
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
        return correctAnswer;
    };

string Question::GetQuestion() {
        return question;
    };

void Question::changeOrderOfAnswers() {
        
        Answer temp;
        for (int i = 0; i < 3; i++) {

            srand(time(NULL));
            int v = rand() % 3;

            temp = answers[i];
            answers[i] = answers[v];
            answers[v] = temp;
        }
    };

Answer * Question::GetAnswers() {
        return answers;
    };



QuestionList::QuestionList(string fileName) {

            ifstream file(fileName);
            json data = json::parse(file);
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

vector<Question> QuestionList::getQuestions() {
            return questionsList;
        };



