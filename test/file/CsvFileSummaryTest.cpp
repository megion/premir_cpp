#include "CsvFileSummaryTest.h"

namespace test {
    namespace file_csvfilesummary {

        void test_collect_summary() {
            size_t dim = 20;
            // инициализация потока чтения файла с данными
            file::CsvFileReader csvReader("../test/datafiles/demo_summary.dat", ' ');
            MyCsvFileRowParser demoRowParser;
            file::stream::CsvFileStreamReader<MyInRow> dataReader(&csvReader, &demoRowParser);
            file::CsvFileSummary<MyInRow> summary(dim);
            summary.collectSummary(0, &csvReader, &demoRowParser);
//            summary.getSummary()->print();

            models::ColSummary* stats = summary.getSummary()->getArray();

            assert(38.08 == stats[0].sum);
            assert(1.86 == stats[19].sum);

            assert(-10.38 == stats[0].min);
            assert(-2.22 == stats[19].min);

            assert(21.47 == stats[0].max);
            assert(2.12 == stats[19].max);

            assert(6 == stats[0].count);
            assert(6 == stats[19].count);
            assert(4 == stats[4].count);

            assert_range(6.3466666667, stats[0].average, 0.00001);
            assert(0.31 == stats[19].average);

            assert_range(0.52517, stats[0].scaledAverage, 0.00001);
            assert_range(0.582949, stats[19].scaledAverage, 0.00001);

            utils::CArrayList<models::ColSummary> copySummary(*summary.getSummary());
            summary.writeSummary("test_csv_file_summary.cod");
            summary.readSummary("test_csv_file_summary.cod");

            assert_range(copySummary[6].sum, (*summary.getSummary())[6].sum, 0.001);
            assert_range(copySummary[8].scaledAverage, (*summary.getSummary())[8].scaledAverage, 0.001);
        }

        void file_csvFileSummary_test() {
            suite("CsvFileSummary");
            mytest(collect_summary);
        }
    }
}



