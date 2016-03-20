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
            long received; // Количество UDP-пакетов, полученных за интервал времени
            long link_faults; // Количество сбоев вещания TS-потока (CC discontinuity) за интервал
            long inner_cc_breaks; // Количество сбоев CC внутри UDP пакета (т.е. сбоев в исходном TS-потоке)
            // Поле не используется
            char lost_overflow[2]; // Флаг сигнализирующий о точности поля LOST.
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
            // -1 = не определено (старая прошивка)
            double mdi_mlr; // Media Loss Rate - среднее количество пакетов потерянное в потоке за секунду,
            // по данным PLC -1 = нет PLC
            char plc[2]; // +/- Флаг использования SmartTUBE PLC на данной приставке. X = n/a
            long region_id; // Идентификатор региона приставки -1 = n/a
            char service_account_number[64]; // Идентификатор счета приставки.
            // -1 - для приставок со старой версией прошивки. -1 = старая прошивка N/A = пустая строка
            char stb_ip[16]; // Адрес приставки (c версии клиента 1.4.3 - реальный адрес приставки)
            char server_date[11]; // DD/MM/YYYY Дата сервера, в момент получения сообщения от приставки
            char server_time[13]; // HH:MM:SS.mmm Время сервера, в момент получения сообщения от приставки
            char spy_version[64]; // Версия агента Spy на приставке
            char player_url[1024]; // Текущий (последний запущенный) URL в плеере STB X = нет данных
            int content_type; // Тип контента (см. раздел Значения поля CONTENT_TYPE) X = нет данных
            int transport_outer; // Тип транспорта, см раздел Значения поля "Типы транспорта" X = нет данных
            int transport_inner; // Подтип транспорта (внутренний/дополнительный подтип), X = нет данных
            long channel_id; // Идентификатор канала (общий для live-вещания и TS/PVR/PL) u32(-1) = n/a X = нет данных
            long play_session; // Идентификатор сессии воспроизведения. 0 = n/a X = нет данных
            char extra_data[64]; // Дополнительные данные в hex без пробелов. Поле устарело. Не используется X = нет данных
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
            char stb_model[128]; // Название модели приставки (пробелы заменяются на "_") X = не определен о
            char source_ip[256]; // IP-адрес источника датаграммы (IPv4 или IPv6) внешний адрес STB находящегося за NAT.

        };

        class SspyRowParser : public file::CsvFileRowParser<SspyData, double> {

            bool parseRow(SspyData &row, models::DataSample<double> *samples, file::CsvFileReader *reader) {
                size_t i = 0;
                readRequiredSample(samples[i], reader); // 1 counter
                ++i;

                reader->read(row.msg_type);
                reader->read(row.stream_type);
                reader->read(row.date, 11);
                reader->read(row.time, 13);
                reader->read(row.interval, 10);

                double interval = calculateInterval(row);
//                if (interval==0) {
//                    // статистика не доступна
//                    std::cout << "interval == 0. Statistics not available" << std::endl;
//                }

                reader->read(row.mac, 18);
                reader->read(row.stream_addr, 16);

                readRequiredSampleForInterval(samples[i], reader, interval); // 2 received
                ++i;

                readRequiredSampleForInterval(samples[i], reader, interval); // 3 link_faults
                ++i;

                readRequiredSample(samples[i], reader); // 4 inner_cc_breaks
                ++i;

                reader->read(row.lost_overflow, 2); // 5 lost_overflow
                if (row.lost_overflow[0]=='=') {
                    samples[i].skipped = false;
                    samples[i].value = 0;
                } else if (row.lost_overflow[0]=='~') {
                    samples[i].skipped = false;
                    samples[i].value = 1;
                } else {
                    samples[i].skipped = true;
                }
                ++i;

                readUnsignedSampleForInterval(samples[i], reader, interval); // 6 lost
                ++i;

                readUnsignedSampleForInterval(samples[i], reader, interval); // 7 restored
                ++i;

                readUnsignedSampleForInterval(samples[i], reader, interval); // 8 overflow
                ++i;

                readUnsignedSampleForInterval(samples[i], reader, interval); // 9 underflow
                ++i;

                readUnsignedSample(samples[i], reader); // 10 mdi_df
                ++i;

                readUnsignedSample(samples[i], reader); // 11 mdi_mlr
                ++i;

                reader->read(row.plc, 2); // 12 plc
                if (row.plc[0]=='-') {
                    samples[i].skipped = false;
                    samples[i].value = 0;
                } else if (row.plc[0]=='+') {
                    samples[i].skipped = false;
                    samples[i].value = 1;
                } else {
                    samples[i].skipped = true;
                }
                ++i;

                reader->read(row.region_id);
                reader->read(row.service_account_number, 64);
                reader->read(row.stb_ip, 16);
                reader->read(row.server_date, 11);
                reader->read(row.server_time, 13);
                reader->read(row.spy_version, 64);
                reader->read(row.player_url, 1024);
                reader->read(row.content_type);
                reader->read(row.transport_outer);
                reader->read(row.transport_inner);
                reader->read(row.channel_id);
                reader->read(row.play_session);
                reader->read(row.extra_data, 64);

                readUnsignedSample(samples[i], reader); // 13 scrambled
                ++i;

                readUnsignedSample(samples[i], reader); // 14 power_state
                ++i;

                readMoreZeroSample(samples[i], reader); // 15 uptime
                ++i;

                reader->read(row.cas_type);
                reader->read(row.cas_key_time);

                readMoreZeroSampleForInterval(samples[i], reader, interval); // 16 vid_frames
                ++i;

                readMoreZeroSampleForInterval(samples[i], reader, interval); // 17 vid_decode_errors
                if (samples[i-1].skipped) {
                    samples[i].skipped = true;
                }
                ++i;

                readMoreZeroSampleForInterval(samples[i], reader, interval); // 18 vid_data_errors
                if (samples[i-1].skipped) {
                    samples[i].skipped = true;
                }
                ++i;

                readMoreZeroSampleForInterval(samples[i], reader, interval); // 19 aud_frames
                ++i;

                readMoreZeroSampleForInterval(samples[i], reader, interval); // 20 aud_data_errors
                if (samples[i-1].skipped) {
                    samples[i].skipped = true;
                }
                ++i;

                readRequiredSample(samples[i], reader); // 21 av_time_skew
                ++i;

                readRequiredSample(samples[i], reader); // 22 av_period_skew
                ++i;

                readRequiredSampleForInterval(samples[i], reader, interval); // 23 buf_underruns
                ++i;

                readRequiredSampleForInterval(samples[i], reader, interval); // 24 buf_overruns
                ++i;

                reader->read(row.sdp_object_id);
                reader->read(row.cas_key_time);

                readRequiredSample(samples[i], reader); // 25 dvb_level_good
                if (samples[i].skipped || samples[i].value!=0 || samples[i].value!=1) {
                    samples[i].skipped = true;
                }
                ++i;

                readUnsignedSample(samples[i], reader); // 26 dvb_level
                ++i;

                readUnsignedSample(samples[i], reader); // 27 dvb_frequency
                ++i;

                readUnsignedSample(samples[i], reader); // 28 cur_bitrate
                ++i;

                reader->read(row.stb_tz);
                reader->read(row.server_tz);

                readMoreZeroSampleForInterval(samples[i], reader, interval); // 29 vid_misshown_frames
                ++i;

                readMoreZeroSample(samples[i], reader); // 30 session_on_period
                ++i;

                readMoreZeroSample(samples[i], reader); // 30 session_on_period
                ++i;

                readUnsignedSampleForInterval(samples[i], reader, interval); // 31 tv_on_period
                ++i;

                readUnsignedSampleForInterval(samples[i], reader, interval); // 32 stb_on_period
                ++i;

                readUnsignedSampleForInterval(samples[i], reader, interval); // 33 user_idle_period
                ++i;

                reader->read(row.stb_model, 128);
                reader->read(row.source_ip, 256);

                return true;
            }

            void initReadFile(file::CsvFileReader *reader) {
            }

        private:

            double calculateInterval(SspyData &row) {
                // calculate interval in msec.
                char mm[3];
                mm[0] = row.interval[0];
                mm[1] = row.interval[1];
                mm[2] = '\0';
                char ss[3];
                ss[0] = row.interval[3];
                ss[1] = row.interval[4];
                ss[2] = '\0';
                char mmm[4];
                mmm[0] = row.interval[6];
                mmm[1] = row.interval[7];
                mmm[2] = row.interval[8];
                mmm[3] = '\0';

                return 60000.0 * std::atof(mm) + 1000.0 * std::atof(ss) + std::atof(mmm);
            }

            size_t readRequiredSample(models::DataSample<double> &sample, file::CsvFileReader *reader) {
                char buffer[64];
                *buffer = '\0';
                size_t bytesRead = reader->read(buffer, 64);
                if (bytesRead == 0 || buffer[0] == 'x' || buffer[0] == 'X') {
                    sample.skipped = true;
                } else {
                    sample.skipped = false;
                    sample.value = std::atof(buffer);
                }
                return bytesRead;
            }

            size_t readRequiredSampleForInterval(models::DataSample<double> &sample, file::CsvFileReader *reader, const double& interval) {
                size_t bytesRead = readRequiredSample(sample, reader);
                if (interval==0) {
                    sample.skipped = true;
                } else {
                    sample.value = sample.value/interval;
                }
                return bytesRead;
            }

            size_t readUnsignedSample(models::DataSample<double> &sample, file::CsvFileReader *reader) {
                size_t bytesRead = readRequiredSample(sample, reader);
                if (sample.skipped || sample.value <0) {
                    sample.skipped = true;
                }
                return bytesRead;
            }

            size_t readUnsignedSampleForInterval(models::DataSample<double> &sample, file::CsvFileReader *reader, const double& interval) {
                size_t bytesRead = readRequiredSample(sample, reader);
                if (interval==0 || sample.skipped || sample.value <0) {
                    sample.skipped = true;
                } else {
                    sample.value = sample.value/interval;
                }
                return bytesRead;
            }

            size_t readMoreZeroSample(models::DataSample<double> &sample, file::CsvFileReader *reader) {
                size_t bytesRead = readRequiredSample(sample, reader);
                if (sample.skipped || sample.value <=0) {
                    sample.skipped = true;
                }
                return bytesRead;
            }

            size_t readMoreZeroSampleForInterval(models::DataSample<double> &sample, file::CsvFileReader *reader, const double& interval) {
                size_t bytesRead = readRequiredSample(sample, reader);
                if (interval==0 || sample.skipped || sample.value <=0) {
                    sample.skipped = true;
                } else {
                    sample.value = sample.value/interval;
                }
                return bytesRead;
            }

        };

        void sspy_data_read_test();
    }
}

#endif

