#ifndef TEST_KOHONEN_KOHONENTEST_H
#define TEST_KOHONEN_KOHONENTEST_H

#include <iostream>
#include <cmath>

#include "test.h"
#include "kohonen/NetworkInitializer.h"
#include "file/CsvFileReader.h"
#include "file/CsvFileRowParser.h"
#include "file/stream/CsvFileStreamReader.h"
#include "utils/SMatrix.h"
#include "utils/RMatrix.h"
#include "kohonen/RandomGenerator.h"
#include "kohonen/winner/EuclideanWinnerSearch.h"
#include "kohonen/alphafunc/InverseAlphaFunction.h"
#include "kohonen/alphafunc/LinearAlphaFunction.h"

#include "kohonen/neighadap/BubbleNeighborAdaptation.h"
#include "kohonen/neighadap/GaussianNeighborAdaptation.h"

#include "kohonen/mapdist/HexaMapDistance.h"
#include "kohonen/mapdist/RectMapDistance.h"

#include "kohonen/SomTrainer.h"
#include "kohonen/SammonMap.h"

#include "kohonen/umat/UMat.h"
#include "kohonen/umat/RectUMat.h"
#include "kohonen/umat/HexaUMat.h"

#include "graphics/ChartThread.h"
#include "graphics/SammonMapChart.h"
#include "graphics/PointChart.h"
#include "graphics/UMatChart.h"

namespace test {
    namespace kohonen {

        struct DemoInRow {
            bool value1;
        };

        class KohonenDemoCsvFileRowParser : public file::CsvFileRowParser<DemoInRow, float> {

            bool parseRow(DemoInRow& row, models::DataSample<float>* samples, file::CsvFileReader<char> *csvReader) {
                return true;
            }

            void initReadFile(file::CsvFileReader<char> *csvReader) {

            }

        };

        void kohonen_test();
    }
}

#endif

