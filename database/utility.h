
#ifndef MYYMT_UTILITY_H
#define MYYMT_UTILITY_H

#include <cstdio>

namespace sjtu {
    /**
     * skip all the space before head.
     * read from std::cin stream until one empty space is encountered.
     * it also checks EOF message.
     *
     * If EOF is hit, return false to indicate the end of "read" process.
     * Otherwise, return true. */
    bool reader(char str[]) {
        int i;
        char tmp;
        tmp = ' ';

        // skip spaces.
        while(tmp == ' ' || tmp == '\n') {
            tmp = (char) getchar();
        }
        // check end of file.
        if(tmp == EOF) {
            str[0] = '\0';
            return false;
        }
        // put back the last useful character.
        ungetc(tmp, stdin);

        i = -1;
        do {
            ++i;
            str[i] = (char) getchar();
        } while(str[i] != ' ' && str[i] != '\n' && str[i] != EOF);
        // when str[i] == ' ', break the loop.
        if(str[i] == EOF) {
            str[i] = '\0';
            return false;
        }
        else {
            str[i] = '\0';
            return true;
        }
    }

    /**
     * exactly the same function as read(), but it's used to read from file.
     * freopen("filename", "mode", stdin) is too slow to tolerant. */
    bool file_reader(FILE *&fp, char str[]) {
        int i;
        char tmp;
        tmp = ' ';

        // skip spaces.
        while(tmp == ' ' || tmp == '\n') {
            fread(&tmp, sizeof(char), 1, fp);
        }
        // check end of file.
        if(tmp == EOF) {
            str[0] = '\0';
            return false;
        }
        // put back the last useful character.
        fseek(fp, -1, SEEK_CUR);

        i = -1;
        do {
            ++i;
            fread(&str[i], sizeof(char), 1, fp);
        } while(str[i] != ' ' && str[i] != '\n' && str[i] != EOF);
        // when str[i] == ' ', break the loop.
        if(str[i] == EOF) {
            str[i] = '\0';
            return false;
        }
        else {
            str[i] = '\0';
            return true;
        }
    }

    /**
     * interpret time in format "xx:xx" to integer.
     * which is the time from 00:00 measured in minutes.
     * if the string is exactly xx:xx, then return -1. */
    int time_to_int(const char *str) {
        int hours;
        int minutes;

        if(str[0] == 'x')
            return -1;

        hours = 0;
        hours = hours * 10 + (str[0] - '0');
        hours = hours * 10 + (str[1] - '0');
        minutes = 0;
        minutes = minutes * 10 + (str[3] - '0');
        minutes = minutes * 10 + (str[4] - '0');

        return hours * 60 + minutes;
    }

    void time_to_str(const int &time, char *str) {
        if(time == -1) {
            strcpy(str, "xx:xx");
        }
        else {
            int hours;
            int minutes;

            minutes = time % 60;
            hours = time / 60;

            str[0] = (char)((hours / 10) + '0');
            str[1] = (char)((hours % 10) + '0');
            str[2] = ':';
            str[3] = (char)((minutes / 10) + '0');
            str[4] = (char)((minutes % 10) + '0');
            str[5] = '\0';
        }
    }

    int date_to_int(const char *str) {
        int date;
        int cnt = 0;
        date = 0;

        date = date * 10 + (str[0] - '0');
        date = date * 10 + (str[1] - '0');
        date = date * 10 + (str[2] - '0');
        date = date * 10 + (str[3] - '0');

        date = date * 10 + (str[5] - '0');
        date = date * 10 + (str[6] - '0');

        date = date * 10 + (str[8] - '0');
        date = date * 10 + (str[9] - '0');

        return date;
    }
    void date_to_str(const int &time, char *str) {
        str[0] = (char) ((time / 10000000) + '0');
        str[1] = (char) ((time / 1000000) % 10 + '0');
        str[2] = (char) ((time / 100000) % 10 + '0');
        str[3] = (char) ((time / 10000) % 10 + '0');
        str[4] = '-';
        str[5] = (char) ((time / 1000) % 10 + '0');
        str[6] = (char) ((time / 100) % 10 + '0');
        str[7] = '-';
        str[8] = (char) ((time / 10) % 10 + '0');
        str[9] = (char) (time % 10 + '0');
    }
	void print_double(double f) {
        if(f == 0)
            printf("0.0");
        else {
            // if(((int)(f * 100)) % 10 == 0)
                // printf("%.2f", f);
            // else
            printf("%.6lf", f);
        }
    }

    int cstrcmp(const char *str1, const char *str2) {
        int i = 0;
        while(i < 100) {
            if(str1[i] == '\0' || str2[i] == '\0') {
                if(str1[i] == '\0' && str2[i] == '\0')
                    return 0;
                else if(str1[i] == '\0')
                    return -1;
                else
                    return 1;
            }
            if(str1[i] < str2[i])
                return -1;
            else if(str1[i] > str2[i])
                return 1;
            ++i;
        }
        return 0;
    }
};

#endif //MYYMT_UTILITY_H
