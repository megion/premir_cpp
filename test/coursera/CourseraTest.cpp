#include "CourseraTest.h"

namespace test {
    namespace coursera {

        void test_titanic() {
            file::CsvFileReader<char> reader(
                    "../test/datafiles/coursera/titanic.csv", ',');
            reader.setStrQuote('\"');
            reader.toEndLine();
            std::string male("male");
            std::string female("female");
            while (reader.hasNext()) {

                Titanic row;
                char sex[7];
                reader.read(row.PassengerId).
                        read(row.Survived).
                        read(row.Pclass).
                        read(row.Name, 256, true).
                        read(sex, 7).
                        read(row.Age);


                if (male == sex) {
                    row.Sex = MALE;
                } else if (female == sex) {
                    row.Sex = FEMALE;
                } else {
                    row.Sex = NO;
                }

                reader.toEndLine();
                if (!reader.isEmptyRead()) {
                    std::cout << "PassengerId: " << row.PassengerId <<
                    " Name: " << row.Name << " sex: " << row.Sex << std::endl;
                }
            }
        }

        void coursera_test() {
            suite("Sspy_test");
            test(titanic);
        }
    }
}



