#ifndef SRC_FILE_STREAM_CSVFILESTREAMREADER_H
#define SRC_FILE_STREAM_CSVFILESTREAMREADER_H

#include <cstdio>
#include <cmath>

#include "file/CsvFileReader.h"
#include "file/CsvFileRowParser.h"
#include "utils/RMatrix.h"
#include "StreamReader.h"
#include "models/models.h"

namespace file {

    namespace stream {

        template<typename Row, typename Num>
        class CsvFileStreamReader : public StreamReader<Row, Num> {
        public:

            typedef typename utils::RMatrix<Row, models::DataSample<Num>>::Row MatrixRow;

            CsvFileStreamReader(file::CsvFileReader *_csvReader, file::CsvFileRowParser<Row, Num> *_rowParser):
                    csvReader(_csvReader), rowParser(_rowParser),
                    useBuffer(false),
                    rowIndex(0),
                    bufferMatrix(nullptr) {
                csvReader->rewindReader();
                rowParser->initReadFile(csvReader);
            }

            CsvFileStreamReader(file::CsvFileReader *_csvReader,
                                file::CsvFileRowParser<Row, Num> *_rowParser,
                                size_t colSize, bool _useBuffer) :
                    csvReader(_csvReader), rowParser(_rowParser),
                    useBuffer(_useBuffer),
                    rowIndex(0),
                    bufferMatrix(nullptr) {
                if (useBuffer) {
                    bufferMatrix = new utils::RMatrix<Row, models::DataSample<Num>>(0, colSize);
                }
                csvReader->rewindReader();
                rowParser->initReadFile(csvReader);
            }

            ~CsvFileStreamReader() {
                if (bufferMatrix) {
                    delete bufferMatrix;
                    bufferMatrix = nullptr;
                }
            }

            bool readNext(Row &row, models::DataSample<Num> *samples) {
                if (useBuffer && rowIndex < bufferMatrix->getRowSize()) {
                    const MatrixRow& mr = bufferMatrix->getRow(rowIndex);
                    bufferMatrix->copyRow(row, samples, mr);
                    rowIndex++;
                    return true;
                }

                if (!csvReader->hasNext()) {
                    return false;
                }

                // парсинг текущей строки данных из файла
                size_t lNo1 = csvReader->getLineNumber();
                rowParser->parseRow(row, samples, csvReader);
                size_t lNo2 = csvReader->getLineNumber();
                if (lNo1 == lNo2) {
                    // TODO: строка прочитана не доконца, тогда прочитать до конца
                    csvReader->toEndLine();
                }

                if (csvReader->isEmptyRead()) {
                    // были считаны пустые данные, значит их использовать нельзя
                    // останавливаем чтение
                    return false;
                } else {
                    if (useBuffer) {
                        MatrixRow mr;
                        mr.data = row;
                        mr.points = samples;
                        // сохранить в буфер
                        bufferMatrix->pushRow(mr);
                        rowIndex++;
                    }
                    return true;
                }
            }

            void rewindReader() {
                if (useBuffer) {
                    rowIndex = 0;
                } else {
                    csvReader->rewindReader();
                    rowParser->initReadFile(csvReader);
                }
            }

        private:
            file::CsvFileReader *csvReader;
            file::CsvFileRowParser<Row, Num> *rowParser;

            // buffering
            utils::RMatrix<Row, models::DataSample<Num>> *bufferMatrix;
            size_t rowIndex;
            bool useBuffer;
            size_t typeSizeof;
        };
    }

}


#endif
