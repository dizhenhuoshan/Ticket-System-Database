#ifndef NEWDATABASE_CONST_RECORDER_H
#define NEWDATABASE_CONST_RECORDER_H

#include <cstdio>
#include <cstring>
#include "constant.h"
#include "constant.h"

namespace sjtu {
    class const_recorder {
    private:
        FILE *fp;
        char filename[20];

    public:
        int append_id;
    private:
        /**
         * create pile file, assume file non-exist.
         * write in initial tail_offset as PILE_HEAD_OFFSET. */
        void create_file() {
            fp = fopen(filename, "w");
            append_id = START_USER_ID;
            fwrite(&append_id, sizeof(int), 1, fp);
            fclose(fp);
        }

    public:
        /** fp and tail_offset's initial values are to show storer is closed. */
        const_recorder() {
            fp = nullptr;
            append_id = -1;
        }

        ~const_recorder() {
            if (fp != nullptr)
                close_file();
        }

        void set_filename(const char *name) {
            strcpy(filename, name);
        }

        void increment() {
            ++append_id;
            fseek(fp, 0, SEEK_SET);
            fwrite(&append_id, sizeof(int), 1, fp);
        }

        void open_file() {
            fp = fopen(filename, "r+");

            // file doesn't exist, then create one.
            if (fp == nullptr) {
                create_file();
                fp = fopen(filename, "r+");
            }
            fread(&append_id, sizeof(int), 1, fp);
        }

        /**
         * everything is concurrently stored into file without delay
         * because this program may be shut down from time to time.
         * but I store tail_offset again when closing for carefulness. */
        void close_file() {
            fseek(fp, 0, SEEK_SET);
            fwrite(&append_id, sizeof(int), 1, fp);
            fclose(fp);
            fp = nullptr;
            append_id = -1;
        }
    };
};

#endif //NEWDATABASE_CONST_RECORDER_H
