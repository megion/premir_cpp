#ifndef TEST_SSPY_SSPYDATA_H
#define TEST_SSPY_SSPYDATA_H

#include <stdio.h>
#include <stdlib.h>
#include <cerrno>
#include <cstring>

//#include <string>
#include <exception>
#include <stdexcept>
#include <iostream>

#include "test.h"

#include "kohonen/NetworkInitializer.h"
#include "file/CsvFileReader.h"
#include "file/CsvFileWriter.h"
#include "file/CsvFileRowParser.h"
#include "file/stream/CsvFileStreamReader.h"
#include "utils/SMatrix.h"
#include "utils/RMatrix.h"
#include "utils/hash/StringHash.h"
#include "kohonen/RandomGenerator.h"
#include "kohonen/winner/EuclideanWinnerSearch.h"
#include "kohonen/alphafunc/InverseAlphaFunction.h"
#include "kohonen/alphafunc/LinearAlphaFunction.h"

#include "kohonen/neighadap/BubbleNeighborAdaptation.h"
#include "kohonen/neighadap/GaussianNeighborAdaptation.h"

#include "kohonen/mapdist/HexaMapDistance.h"
#include "kohonen/mapdist/RectMapDistance.h"

#include "kohonen/SomTrainer.h"
#include "kohonen/SomKeeper.h"
#include "kohonen/SammonMap.h"
#include "kohonen/labeling/SomLabeling.h"

#include "kohonen/umat/UMat.h"
#include "kohonen/umat/RectUMat.h"
#include "kohonen/umat/HexaUMat.h"

#include "graphics/ChartThread.h"
#include "graphics/SammonMapChart.h"
#include "graphics/PointChart.h"
#include "graphics/UMatChart.h"


namespace test {
    namespace sspy {

        struct SspyData {
            long counter; // Счетчик принимаемых пакетов, считается индивидуально по каждому STB.
            // Позволяет обнаружить пропадание пакетов статистики.
            char msg_type; // Тип события
            char stream_type; // Тип медиапотока
            char date[11]; // Дата события DD/MM/YYYY
            char time[13]; // Дата события HH:MM:SS.mmm
            char interval[10]; // MM:SS.mmm Интервал времени, за который собрана статистика, с точностью до миллисекунд
            char mac[18]; // xx:xx:xx:xx:xx:xx
            char stream_addr[16]; // xxx.xxx.xxx.xxx
            long recieved; // Количество UDP-пакетов, полученных за интервал времени
            long link_faults; // Количество сбоев вещания TS-потока (CC discontinuity) за интервал
            long inner_cc_br_eaks; // Количество сбоев CC внутри UDP пакета (т.е. сбоев в исходном TS-потоке)
            // Поле не используется
            long lost_overflow; // Флаг сигнализирующий о точности поля LOST.
            // "=" - значение точно, "~" - возможна погрешность (X = нет PLC)
            long lost; // Количество потерянных UDP-датаграмм, обнаруженных с помощью SmartTUBE PLC.
            // Поле не заполняется, если нет PLC-потока (-1 = NULL)
            long restored; // Количество UDP-датаграмм, восстановленных SmartTUBE PLC.
            // Поле не заполяется если нет PLC потока.
            long overflow; // Количество событий переполнения виртуального буфера (overflow).
            // Событие переполнения возникает если медиаданные приходят раньше срока не более чем на 1 секунду
            long underflow; // Количество событий «антипереполнения» (опустошения) виртуального буфера (underflow).
            // Событие «антипереполнения» возникает если медиаданные приходят позже срока на более чем на 1 секунду
            long mdi_df; // Delay Factor - значение характеризующее минимально размер буфера (в миллисекундах),
            // необходимый для устранения текущего джиттера (задержки и опережения в доставке пакетов) равный
            // разнице между самыми большими отклонениями времени доставки за учетный интервал
            double mdi_mlr; // Media Loss Rate - среднее количество пакетов потерянное в потоке за секунду,
            // по данным PLC -1 = нет PLC
            char plc[2]; // +/- Флаг использования SmartTUBE PLC на данной приставке. X = n/a
            long region_id; // Идентификатор региона приставки -1 = n/a
            char service_account_number[24]; // Идентификатор счета приставки.
            // -1 - для приставок со старой версией прошивки. -1 = старая прошивка N/A = пустая строка
            char stb_ip[16]; // Адрес приставки (c версии клиента 1.4.3 - реальный адрес приставки)
            char server_date[11]; // DD/MM/YYYY Дата сервера, в момент получения сообщения от приставки
            char server_time[13]; // HH:MM:SS.mmm Время сервера, в момент получения сообщения от приставки
            char spy_version[24]; // Версия агента Spy на приставке
            char player_url[64]; // Текущий (последний запущенный) URL в плеере STB X = нет данных
            int content_type; // Тип контента (см. раздел Значения поля CONTENT_TYPE) X = нет данных
            int transport_outer; // Тип транспорта, см раздел Значения поля "Типы транспорта" X = нет данных
            int transport_inner; // Подтип транспорта (внутренний/дополнительный подтип), X = нет данных
            long play_session; // Идентификатор сессии воспроизведения. 0 = n/a X = нет данных
            char extra_data; // Дополнительные данные в hex без пробелов. Поле устарело. Не используется X = нет данных
            int scrambled; // В потоке есть скремблированные фреймы (CAS) (-1) - состояние не определено
            int power_state; // Режим работы приставки: 0 – Sleep 1 – Standby 2 – Active (-1) - неизвестно
            long uptime; // Аптайм приставки (с момента ее включения), секунды 0 - неизвестно
            int cas_type; // Тип CAS: 1 – Verimatrix 2 – Securemedia 0 - неизвестно
            size_t cas_key_time; // Время выдачи текущего ключа CAS (time_t), в формате Unix time 0 - неизвестно
            long vid_frames; // Количество декодированных видеокадров 0 - видеостатистика недоступна
            long vid_decode_errors; // Количество ошибочно декодированных кадров из предыдущего значения
            long vid_data_errors; // Количество ошибок в данных потока
            long aud_frames; // Количество декодированных аудиофреймов 0 -аудиостати стика недоступна
            long aud_data_errors; // Количество ошибок в аудиоданных
            long av_time_skew; // Дельта от времени сообщения кточному времени получения AV-статистики (миллисекунды)
            long av_period_skew; // Дельта от периода сообщения к точному периоду AV-статистики
            // (миллисекунды с точностью до 0.1сек).
            long buf_underruns; // Подсчет случаев фактической остановки воспроизведения
            // из-за перебоев доставки медиапотока, которые не буфер конвейера не смог нивелировать
            long buf_overruns; // Подсчет случаев фактического отброса медиаданных из-за переполнения буфера
            // конвейера врезультате неконтролируемого наплыва данных
            long sdp_object_id; // Идентификатор источника воспроизведения в SDP.
            // Для источников воспроизведения не из SDP, поле должно иметь значение (-1)
            int dvb_level_good; // Признак нахождения уровня сигнала DVB-приемника не ниже допустимого порога
            // (50 %) Используется только для устройств, оборудованных DVB-ресивером
            int dvb_level; // Текущий уровень сигнала DVB-приемника в процентах. Используется только для устройств,
            // оборудованных DVB-ресивером
            long dvb_frequency; // Текущая несущая частота DVB-приемника (KГц). Используется только для устройств,
            // оборудованных DVB-ресивером
            long cur_bitrate; // Текущий выбранный битрейт при адаптивном стриминге (HLS).
            double stb_tz; // дробное со знаками, смещение в часах относительно GMT
            // На сколько смещены указанные дата/время STB относительно GMT (для MSK =4) Поле не используется
            double server_tz; // дробное со знаками, смещение в часах относительно GMT
            // На сколько смещены указанные дата/время сервера относительно GMT (для MSK =4) Поле не используется
            long vid_misshown_frames; // Количество кадров возможно показанных с артефактами. Поле не используется
            long session_on_period; // Период текущей сессии – для отсева тех STB, которые включены,
            // но нереально не используются (телевизор выключен). Например,. если канал включен
            // более N часов, то его не учитываем в рейтинге. Поле не используется
            long tv_on_period; // Время, в течение которого был включен телевизор (по HDMI), 0 = телевизор выключен,
            // u32(-1) HDMI не использовался. Поле не используется
            long stb_on_period; // Время работы STB после Standby. Поле не используется
            long user_idle_period; // Время, прошедшее с последнего нажатия на пульте. Поле не используется
            char stb_model[64]; // Название модели приставки (пробелы заменяются на "_") X = не определен о
            char source_ip[256]; // IP-адрес источника датаграммы (IPv4 или IPv6) внешний адрес STB находящегося за NAT.

        };

        class SspyRowParser : public file::CsvFileRowParser<SspyData, double> {

            bool parseRow(SspyData &row, models::DataSample<double> *samples, file::CsvFileReader *csvReader) {


//                reader.read(row.counter);
//                reader.read(row.msg_type);
//                reader.read(row.stream_type);
//                reader.read(row.date, 11);
//                reader.read(row.time, 13);
//                reader.read(row.interval, 10);

//                size_t colSize = 20;
//                for (size_t i = 0; i < colSize; ++i) {
//                    readNextDataSample(samples[i], csvReader);
//                }
//                // get label
//                char buffer[2];
//                *buffer = '\0';
//                size_t bytesRead = csvReader->read(buffer, 2);
//                if (bytesRead == 0) {
//                    row.label = '\0';
//                } else {
//                    row.label = buffer[0];
//                }

                return true;
            }

            void initReadFile(file::CsvFileReader *csvReader) {
            }

        private:

            size_t readNextDataSample(models::DataSample<double> &sample, file::CsvFileReader *csvReader) {
                char buffer[64];
                *buffer = '\0';
                size_t bytesRead = csvReader->read(buffer, 64);
                if (bytesRead == 0 || buffer[0] == 'x' || buffer[0] == 'X') {
                    sample.skipped = true;
                } else {
                    sample.skipped = false;
                    sample.value = std::atof(buffer);
                }
                return bytesRead;
            }

        };

        void sspy_data_read_test();
    }
}

#endif

