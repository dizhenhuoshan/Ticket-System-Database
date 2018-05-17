#ifndef DATABASE_VECTOR_H
#define DATABASE_VECTOR_H

#include <cstdlib>
#include <iostream>
#include "constant.h"

namespace sjtu {

    template <class ElemType>
    class vector {
    private:
        // ElemType *arr;
        ElemType arr[blockSize];
        short len;
        int mallocSize;

    public:
        explicit vector(const int &&size) {
            mallocSize = size;
            // arr = new ElemType[mallocSize];
            len = 0;
        }
        ~vector() {
            // delete [] arr;
        }

        void push_back(const ElemType &elem) {
            // if(len == mallocSize) std::cerr << "vector is full" << std::endl;
            arr[len++] = elem;
        }

        void insert(const short &pos, const ElemType &elem) {
//            if(pos > len || pos < 0 || len == mallocSize)
//                std::cerr << "insert error" << std::endl;

            for (int i = len; i > pos; --i)
                arr[i] = arr[i - 1];
            arr[pos] = elem;
            ++len;
        }

        void erase(const short &pos) {
//            if(pos >= len || pos < 0)
//                fprintf(stderr, "invalid erase position. pos: %d, len: %d\n", pos, len);

            for(short i = pos; i < len - 1; ++i)
                arr[i] = arr[i+1];              // a[len-2] <-- a[len-1]
            --len;
        }

        void pop_back() {
            /*
            if(len == 0)
                fprintf(stderr, "pop empty vector\n");
            */
            --len;
        }

        void clear() {
            len = 0;
        }

        short size() const {
            return (short)len;
        }

        ElemType &back() {
            // if(len == 0) std::cerr << "back empty vector." << std::endl;
            return arr[len-1];
        }

        ElemType &front() {
            // if(len == 0) std::cerr << "front empty vector." << std::endl;
            return arr[0];
        }

        // mallocSize must be identical because of same type.
        // this invariant is maintained in TreeNode.h.
        vector<ElemType> &operator= (const vector<ElemType> &other) {
            if(this == &other) return *this;

            len = other.len;
            for(int i = 0; i < len; ++i)
                arr[i] = other.arr[i];
            return *this;
        }

        ElemType& operator[] (const short &pos) {
            /*
            if(pos < 0 || pos >= len)
                std::cerr << "vector out of bound" << std::endl;
            */
            return arr[pos];
        }

        /** make current vector a sub-vector of another vector. */
        void assign(const vector<ElemType> &other, const short &left, const short &right) {
            /*
            if(left < 0 || right > other.size() || left > right)
                std::cerr << "assign out of range" << std::endl;
            */
            for(int i = left; i < right; ++i) {
                arr[i-left] = other.arr[i];
            }
            len = right - left;
        }

        void file_read(FILE *&fp, const short &item_num) {
            len = item_num;
            fread(arr, sizeof(ElemType), (size_t) len, fp);
        }

        void file_write(FILE *&fp) {
            fwrite(arr, sizeof(ElemType), (size_t) len, fp);
        }
    };
};

#endif //DATABASE_VECTOR_H
