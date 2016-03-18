/**
 * Сохранение нейронов карты
 */

#ifndef SRC_KOHONEN_SOMKEEPER_H
#define SRC_KOHONEN_SOMKEEPER_H

#include <stdio.h>
#include <stdlib.h>
#include <cerrno>
#include <cstring>


#include <string>
#include <exception>
#include <stdexcept>
#include <iostream>


#include "utils/RMatrix.h"
#include "file/CsvFileWriter.h"

#include "utils/console_colors.h"

namespace kohonen {

    template<typename Out>
    class SomKeeper {
    public:

        typedef utils::RMatrix<models::NeuronInfo, Out> OutCodes;
        typedef typename utils::RMatrix<models::NeuronInfo, Out>::Row Neuron;

        SomKeeper() {
        }

        ~SomKeeper() {
        }

        void saveSom(OutCodes *trainedSom, file::CsvFileWriter *csvWriter) {
            size_t colSize = trainedSom->getColSize();
            for (size_t i = 0; i < trainedSom->getRowSize(); ++i) {
                Neuron &ni = trainedSom->getRow(i);
                for (size_t j = 0; j < colSize; ++j) {
                    csvWriter->write(ni.points[j]);
                }
                csvWriter->endLine();
            }
        }



    private:

    };
}


#endif
