#include "CourseraTest.h"

namespace test {
    namespace coursera {

        void test_lesson1_titanic_1_2_3_4() {
            file::CsvFileReader reader("../test/datafiles/coursera/titanic.csv", ',');
            reader.setStrQuote('\"');
            reader.toEndLine();
            std::string male("male");
            std::string female("female");
            size_t maleCount = 0;
            size_t femaleCount = 0;
            size_t survivedCount = 0;
            size_t firstClassCount = 0;
            size_t count = 0;
            utils::CArrayList<int> ages;
            while (reader.hasNext()) {

                Titanic row;
                char sex[7];
                reader.read(row.PassengerId);
                reader.read(row.Survived);
                reader.read(row.Pclass);
                reader.read(row.Name, 256, true);
                reader.read(sex, 7);
                size_t ageCR = reader.read(row.Age);

                if (male == sex) {
                    row.Sex = MALE;
                } else if (female == sex) {
                    row.Sex = FEMALE;
                } else {
                    row.Sex = NO;
                }

                reader.read(row.SibSp);
                reader.read(row.Parch);

                reader.toEndLine();
                if (!reader.isEmptyRead()) {
                    count++;
                    if (row.Sex == MALE) {
                        maleCount++;
                    } else if (row.Sex == FEMALE) {
                        femaleCount++;
                    }
                    if (row.Survived == 1) {
                        survivedCount++;
                    }
                    if (row.Pclass == 1) {
                        firstClassCount++;
                    }
                    if (ageCR > 0) {
                        ages.push(row.Age);
                    }
//                    std::cout << "PassengerId: " << row.PassengerId <<
//                    " SibSp: " << row.SibSp << " Parch: " << row.Parch << std::endl;
                }
            }

//            std::string s = "Some people, when confronted with a problem, think "
//                    "\"I know, I'll use regular expressions.\" "
//                    "Now they have two problems.";
//
//            std::regex self_regex("REGULAR EXPRESSIONS", std::regex_constants::icase);
//            if (std::regex_search(s, self_regex)) {
//                std::cout << "Text contains the phrase 'regular expressions'\n";
//            }

            double survivedPer = ((double) survivedCount) / count;
            double firstClassPer = ((double) firstClassCount) / count;

            double med = utils::ArrayUtils<int, int, double>::median(
                    ages.getArray(), ages.size());
            double aver = utils::ArrayUtils<int, int, double>::average(
                    ages.getArray(), ages.size());

            std::cout << "Female count: " << femaleCount <<
            " maleCount: " << maleCount << std::endl;
            std::cout << "Survived %: " << survivedPer <<
            " firstClass %: " << firstClassPer << std::endl;
            std::cout << "med: " << med <<
            " aver: " << aver << std::endl;

            // 5. Коррелируют ли число братьев/сестер/супругов с числом родителей/детей?
            // Посчитайте корреляцию Пирсона между признаками SibSp и Parch.
        }

        void coursera_test() {
            suite("Coursera");
            mytest(lesson1_titanic_1_2_3_4);
        }
    }
}



