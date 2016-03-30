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

    class SomKeeper {
    public:

        typedef utils::RMatrix<models::NeuronInfo, double> OutCodes;
        typedef typename utils::RMatrix<models::NeuronInfo, double>::Row Neuron;

        SomKeeper() {
        }

        ~SomKeeper() {
        }

        void saveSom(OutCodes *trainedSom, const char *filename) {
            file::CsvFileWriter writer(filename);
            size_t colSize = trainedSom->getColSize();
            for (size_t i = 0; i < trainedSom->getRowSize(); ++i) {
                Neuron &ni = trainedSom->getRow(i);
                for (size_t j = 0; j < colSize; ++j) {
                    writer.write(ni.points[j]);
                }
                writer.endLine();
            }
        }



    private:

    };
}


#endif
