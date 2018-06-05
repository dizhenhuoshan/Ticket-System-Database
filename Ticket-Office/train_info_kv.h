#ifndef MYYMT_TRAIN_H
#define MYYMT_TRAIN_H

#include "constant.h"
#include "utility.h"
#include <cstdio>
#include <cstring>
/**
 * basic information about train.
 * it's used for train_storer.
 *
 * Note that since it's a piled file, we can write and read it in the same way as std stream.
 * But this uses a lot of checking, and is slow.
 * We choose to use binary storage at first.
 * If space is expensive, we switch to the other. */
namespace sjtu {
    /** station is a struct used to hold station info associated with each train. */
    struct station {
        char station_name[NAME_SIZE];
        short arrive;
        short start;
        short stopover;   // stop and wait time. departure time = start + stopover.
        double price[MAXSEATSCATALOG];
        char start_date_offset;

        station() {}

        /// read in data from stdin.
        void read(int seat_catalog_num) {
            char time_tmp[6];

            reader(station_name);

            // read in time.
            reader(time_tmp);
            arrive = time_to_int(time_tmp);
            reader(time_tmp);
            start = time_to_int(time_tmp);
            reader(time_tmp);
            stopover = time_to_int(time_tmp);

            // read in prices for each kind of seat.
            for (int i = 0; i < seat_catalog_num; ++i) {
                // skip ¥.
                char tmp = (char) getchar();
                while(tmp < '0' || tmp > '9') {
                    tmp = (char) getchar();
                }
                ungetc(tmp, stdin);
                scanf("%lf", &price[i]);
                /*
                int offset = 0;     // we want pure double.
                reader(price_tmp);

                while (price_tmp[offset] < '0' || price_tmp[offset] > '9')
                    ++offset;

                sscanf(price_tmp + offset, "%lf", &price[i]);
                 */
            }
        }

        /// write station in file in binary.
        void file_write(FILE *&fp, int seat_catalog_num) const {
            fwrite(station_name, sizeof(station_name), 1, fp);
            fwrite(&arrive, sizeof(arrive), 1, fp);
            fwrite(&start, sizeof(start), 1, fp);
            fwrite(&stopover, sizeof(stopover), 1, fp);
            fwrite(&start_date_offset, sizeof(start_date_offset), 1, fp);
            // write prices.
            for (int i = 0; i < seat_catalog_num; ++i)
                fwrite(&price[i], sizeof(double), 1, fp);
        }


        /// read station from file in binary.
        void file_read(FILE *&fp, int seat_catalog_num) {
            fread(station_name, sizeof(station_name), 1, fp);
            fread(&arrive, sizeof(arrive), 1, fp);
            fread(&start, sizeof(start), 1, fp);
            fread(&stopover, sizeof(stopover), 1, fp);
            fread(&start_date_offset, sizeof(start_date_offset), 1, fp);
            // write prices.
            for (int i = 0; i < seat_catalog_num; ++i)
                fread(&price[i], sizeof(double), 1, fp);
        }

        /// debug code.
        void view(int seat_catalog_num) {
            char time_tmp[10];
            printf("%s ", station_name);

            time_to_str(arrive, time_tmp);
            printf("%s ", time_tmp);
            time_to_str(start, time_tmp);
            printf("%s ", time_tmp);
            time_to_str(stopover, time_tmp);
            printf("%s ", time_tmp);

            for (int j = 0; j < seat_catalog_num; ++j) {
                printf("￥");
                print_double(price[j]);
                printf(" ");
            }
            // printf("start_date_offset %d", (int)start_date_offset);
            printf("\n");
        }
    };

    typedef int train_info_key;

    class train_info_val {
    public:
        char train_id[ID_SIZE];
        char train_name[NAME_SIZE];
        char train_catalog;

        char station_num;
        station stations[MAXSTATION];

        char seat_catalog_num;
        char seat_catalog[MAXSEATSCATALOG][TICKET_KIND_SIZE];

    public:
        train_info_val() {}

        /**
         * read from stdin. fill data fields. */
        void read() {
            reader(train_id);
            reader(train_name);
            reader(&train_catalog);

            int tmp1, tmp2;
            scanf("%d %d ", &tmp1, &tmp2);
            station_num = (char) (tmp1);
            seat_catalog_num = (char) (tmp2);

            // read in seat_catalog name.
            for (int i = 0; i < seat_catalog_num; ++i)
                reader(seat_catalog[i]);
            // read in stations.
            for (int i = 0; i < station_num; ++i)
                stations[i].read(seat_catalog_num);
            // set up start_date_offset.
            stations[0].start_date_offset = 0;
            for(int i = 1; i < station_num; ++i) {
                stations[i].start_date_offset = stations[i-1].start_date_offset;
                if(stations[i-1].start > stations[i].start)
                    ++stations[i].start_date_offset;
            }
        }

        /**
         * read a train from file in binary. */
        void file_read(FILE *&fp) {
            fread(train_id, sizeof(char), TRAIN_ID_SIZE, fp);
            fread(train_name, sizeof(char), NAME_SIZE, fp);
            fread(&train_catalog, sizeof(char), 1, fp);

            fread(&station_num, sizeof(char), 1, fp);
            fread(&seat_catalog_num, sizeof(char), 1, fp);
            // write seat_catalog name.
            for (int i = 0; i < seat_catalog_num; ++i)
                fread(seat_catalog[i], sizeof(char), TICKET_KIND_SIZE, fp);
            // write stations.
            for (int i = 0; i < station_num; ++i)
                stations[i].file_read(fp, seat_catalog_num);
        }

        /**
         * write a train into file. */
        void file_write(FILE *&fp) const {
            fwrite(train_id, sizeof(char), TRAIN_ID_SIZE, fp);
            fwrite(train_name, sizeof(char), NAME_SIZE, fp);
            fwrite(&train_catalog, sizeof(char), 1, fp);

            fwrite(&station_num, sizeof(char), 1, fp);
            fwrite(&seat_catalog_num, sizeof(char), 1, fp);
            // write seat_catalog name.
            for (int i = 0; i < seat_catalog_num; ++i)
                fwrite(seat_catalog[i], sizeof(char), TICKET_KIND_SIZE, fp);
            // write stations.
            for (int i = 0; i < station_num; ++i)
                stations[i].file_write(fp, seat_catalog_num);
        }


        /// debug code.
        void view() {
            printf("%s %s %c %d %d ", train_id, train_name, train_catalog, station_num, seat_catalog_num);

            for (int i = 0; i < seat_catalog_num; ++i)
                printf("%s ", seat_catalog[i]);
            printf("\n");

            for (int i = 0; i < station_num; ++i)
                stations[i].view(seat_catalog_num);
        }
    };

    /**
     * a class specifically used to store train class as pile file.
     * it can provide:
     *  current file tail position.
     *  read a whole train object given initial file offset.
     *  write a whole train object given initial file offset.
     *  append a new train object, and return its initial file offset. */
    class pile_train_storer {
    private:
        FILE *fp;
        int tail_offset;    // this is stored at the file head.
        char filename[20];

    private:
        /**
         * create pile file, assume file non-exist.
         * write in initial tail_offset as PILE_HEAD_OFFSET. */
        void create_file() {
            tail_offset = PILE_HEAD_OFFSET;
            fp = fopen(filename, "w");
            fwrite(&tail_offset, sizeof(int), 1, fp);
            fclose(fp);
        }

    public:
        /** fp and tail_offset's initial values are to show storer is closed. */
        pile_train_storer() {
            fp = nullptr;
            tail_offset = -1;
        }

        ~pile_train_storer() {
            if(fp != nullptr)
                close_file();
        }

        void set_filename(const char *name) {
            strcpy(filename, name);
        }

        void open_file() {
            fp = fopen(filename, "r+");

            // file doesn't exist, then create one.
            if (fp == nullptr) {
                create_file();
                fp = fopen(filename, "r+");
            }
            fread(&tail_offset, sizeof(int), 1, fp);
        }

        /**
         * everything is concurrently stored into file without delay
         * because this program may be shut down from time to time.
         * but I store tail_offset again when closing for carefulness. */
        void close_file() {
            fseek(fp, 0, SEEK_SET);
            fwrite(&tail_offset, sizeof(int), 1, fp);
            fclose(fp);
            fp = nullptr;
            tail_offset = -1;
        }

        int get_tail_offset() {
            return tail_offset;
        }
        /**
         * read a whole train object given initial file offset. */
        void get_train_by_offset(int offset, train_info_val &ret) {
            fseek(fp, offset, SEEK_SET);
            ret.file_read(fp);
        }

        /**
         * write a whole train object given initial file offset. */
        void write_train_by_offset(int offset, const train_info_val &val) {
            fseek(fp, offset, SEEK_SET);
            val.file_write(fp);
        }


        /**
         * append a train object, modify tail_offset. Write it back concurrently just in case of shut down.
         * return the head address where newTrain is inserted. */
        int append_train(const train_info_val &newTrain) {
            int insert_address = tail_offset;

            write_train_by_offset(tail_offset, newTrain);
            // printf("%ld\n", ftell(fp));
            tail_offset = ftell(fp);
            fseek(fp, 0, SEEK_SET);
            fwrite(&tail_offset, sizeof(int), 1, fp);
            return insert_address;
        }
    };
};

#endif //MYYMT_TRAIN_H
