#ifndef TEST_SSPY_SSPYDATA_H
#define TEST_SSPY_SSPYDATA_H

#include <stdio.h>
#include <stdlib.h>
#include <cerrno>
#include <cstring>

#include <string>
#include <exception>
#include <stdexcept>
#include <iostream>

#include "test.h"


namespace test {
    namespace sspy {

        struct SspyData {
            long counter;
            char msg_type;
            char stream_type;
            char date[11]; // DD/MM/YYYY
            char time[13]; // HH:MM:SS.mmm
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


        };

        void sspy_data_read_test();
    }
}

#endif

