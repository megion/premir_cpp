#include "CourseraTest.h"

namespace test {
    namespace coursera {

        void test_lesson1_titanic() {
            file::CsvFileReader reader("../test/datafiles/coursera/titanic.csv", ',');
            reader.setStrQuote('\"');
//            reader.toEndLine();
//            std::string male("male");
//            std::string female("female");
            size_t maleCount = 0;
            size_t femaleCount = 0;
            size_t survivedCount = 0;
            size_t firstClassCount = 0;
            size_t count = 0;
            utils::CArrayList<int> ages;
            utils::CArrayList<models::DataSample> sibSpAr;
            utils::CArrayList<models::DataSample> parchAr;

            // Extraction of several sub-matches
//            std::regex pieces_regex("([a-z]+)\\,([a-z]+)");
//            std::smatch pieces_match;

//            regex_t regex;
            /* Compile regular expression */
//            int reti = regcomp(&regex, "Cann,", 0);
//            if (reti) {
//                fprintf(stderr, "Could not compile regex\n");
//                exit(1);
//            }

            TitanicCsvFileRowParser rowParser;
            file::stream::CsvFileStreamReader<Titanic> dataReader(&reader, &rowParser);
            Titanic row;
            models::DataSample samples[6];
            char msgbuf[100];
            while (dataReader.readNext(row, samples)) {
                if (row.Survived == 1 && !samples[0].skipped) {
                    survivedCount++;
                }

                if (row.Sex == MALE) {
                    maleCount++;
                } else if (row.Sex == FEMALE) {
                    femaleCount++;
                }

                if (row.Pclass == 1 && !samples[1].skipped) {
                    firstClassCount++;
                }
                if (!samples[3].skipped) {
                    ages.push(row.Age);
                }

                sibSpAr.push(samples[4]);
                parchAr.push(samples[5]);

//                if (std::regex_match(row.Name, pieces_match, pieces_regex)) {
//                    std::cout << row.Name << '\n';
//                    for (size_t i = 0; i < pieces_match.size(); ++i) {
//                        std::ssub_match sub_match = pieces_match[i];
//                        char* piece = sub_match.str();
//                        std::cout << "  submatch " << i << ": " << piece << '\n';
//                    }
//                }

                /* Execute regular expression */
//                reti = regexec(&regex, row.Name, 0, NULL, 0);
//                if (!reti) {
//                    std::cout << "Match " << '\n';
//                } else if (reti == REG_NOMATCH) {
////                    std::cout << "No Match " << '\n';
//                } else {
//                    regerror(reti, &regex, msgbuf, sizeof(msgbuf));
//                    fprintf(stderr, "Regex match failed: %s\n", msgbuf);
//                    exit(1);
//                }

//                std::cout << "Id: " << row.PassengerId << " S: " << row.SibSp << " P: " << row.Parch << std::endl;

                count++;
            }

            /* Free memory allocated to the pattern buffer by regcomp() */
//            regfree(&regex);

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

            double med = utils::ArrayUtils<int, int, double>::median(ages.getArray(), ages.size());
            double aver = utils::ArrayUtils<int, int, double>::average(ages.getArray(), ages.size());

            std::cout << "Female count: " << femaleCount << " maleCount: " << maleCount << std::endl;
            std::cout << "Survived %: " << survivedPer << " firstClass %: " << firstClassPer << std::endl;
            std::cout << "med: " << med << " aver: " << aver << std::endl;

            // 5. Коррелируют ли число братьев/сестер/супругов с числом родителей/детей?
            // Посчитайте корреляцию Пирсона между признаками SibSp и Parch.
            double corp = ml::pearsonCorrelation(sibSpAr.getArray(), parchAr.getArray(), count);
            std::cout << "Pearson correlation: " << corp << std::endl;

            // 6. Какое самое популярное женское имя на корабле?
            // Извлеките из полного имени пассажира (колонка Name) его личное имя (First Name).
        }

        void coursera_test() {
            suite("Coursera");
            mytest(lesson1_titanic);
        }
    }
}



