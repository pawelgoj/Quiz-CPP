#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "../libs/doctest.h"
#include "../MilionersCPP/pytanie.h"

class AnswerFixture {
    public:
        Answer answer;
        AnswerFixture() {
            answer = Answer("Correct answer!!!", true, 'A');
        }
};


TEST_SUITE("Tests od Answer object.") {
    TEST_CASE_FIXTURE(AnswerFixture, "GetAnswer") {
        CHECK(answer.GetAnswer() == "Correct answer!!!");
    };
    TEST_CASE_FIXTURE(AnswerFixture, "GetIsCorrect") {
        CHECK(answer.GetIsCorrect() == true);
    };
    TEST_CASE_FIXTURE(AnswerFixture, "GetLetter") {
        CHECK(answer.GetLetter() == 'A');
    };

}

TEST_SUITE("Tests of Question object.") {
    Question makeQuestionWithAnswers() {
        Answer answers[4];
        answers[0] = Answer("Correct answer!!!", true, 'A');
        answers[1] = Answer("Incorrect answer!!!", false, 'B');
        answers[2] = Answer("Incorrect answer!!!", false, 'C');
        answers[3] = Answer("Incorrect answer!!!", false, 'D');

        Question question;

        question.SetAnswers(answers);
        question.SetQuestion("Test question?", 0);
        return question;
    }
    SCENARIO("Get correct answer and question.") {
        GIVEN("Question object, question and answers was set") {
            Question question = makeQuestionWithAnswers();
            WHEN("Use GetCorrectAnswer method") {

                Answer answer = question.GetCorrectAnswer();

                THEN("Correct answer should be returned") {
                    CHECK(answer.GetAnswer() == "Correct answer!!!");
                }
            };

            WHEN("Use GetQuestion method") {

                THEN("Question should be returned") {
                    CHECK(question.GetQuestion() == "Test question?");
                }
            }
        }
    }
    SCENARIO("Change order of answers") {
        GIVEN("Question object, question and answers was set") {
            Question question = makeQuestionWithAnswers();
            WHEN("Use ChangeOrderOfAnswers method ann GetAnswers") {
                question.changeOrderOfAnswers();
                Answer *ptr_answers = question.GetAnswers();

                THEN("Answers should be in different order") {
                    Answer answer1 = *ptr_answers;
                    Answer answer2 = *(ptr_answers + 1);
                    Answer answer3 = *(ptr_answers + 2);
                    Answer answer4 = *(ptr_answers + 3);

                    // It is wary rare but randomly selected order of answer can 
                    // be the same as before
                    bool check = (answer1.GetAnswer() == "Correct answer!!!") &&
                                (answer2.GetAnswer() == "Incorrect answer!!!") &&
                                (answer3.GetAnswer() == "Incorrect answer!!!") &&
                                (answer4.GetAnswer() == "Incorrect answer!!!");
                    CHECK_FALSE(check);

                    check = (answer1.GetAnswer() == "Correct answer!!!") ||
                                (answer2.GetAnswer() == "Correct answer!!!") ||
                                (answer3.GetAnswer() == "Correct answer!!!") ||
                                (answer4.GetAnswer() == "Correct answer!!!");
                    CHECK(check);

                }
            }

        }
    }
}

TEST_SUITE("Question list tests") {
    SCENARIO("Change question list order and get question list.") {
        GIVEN("Question list oject and json file with questions") {

            QuestionList questionList("../MilionersCPP/Answers.json");

            WHEN("Use question list method: getQuestions") {
                vector<Question> questions = questionList.getQuestions();
                THEN("Question list should be returned") {
                    CHECK(questions.size() == 4);
                    CHECK(questions[0].GetId() == 1);
                }
            }
            WHEN("Use question list method: getQuestions") {
                questionList.changeQuestionsOrder();
                vector<Question> questions = questionList.getQuestions();
                THEN("Question list should be returned") {
                    CHECK(questions.size() == 4);
                    CHECK(questions[0].GetId() != 1);
                }
            }
        }
    } 
}
