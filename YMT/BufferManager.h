#ifndef DATABASE_BUFFERMANAGER_H
#define DATABASE_BUFFERMANAGER_H

#include <queue>
#include <cstring>
#include "TreeNode.h"
#include "BPlusTree.h"
#include "constant.h"

/**
 * each B-plus tree file is arranged as: root-offset, append-offset, head-offset.
 * then: node, node, node, node.
 *
 * each node: isLeaf, key number, val number, child number, then vector values sequentially.
 * finally is one offsetNumber, means the next node.
 *
 * Write back manually is needed to maintain consistency. */

namespace sjtu {

    template <class KeyType, class ValType>
    class BufferManager {
    public:
        friend class debugger;
        template <class K, class V>
        friend class BPlusTree;
    private:
        typedef TreeNode<KeyType, ValType> Node;

        char filename[50];
        FILE *fp;

        bool isOpened;              /// has file pointer opened file.

        Node pool;
        /** when B+ tree do insert and erase, it has to update head_off and tail_off.
         * I don't know how to use friend class... I have to admit. */
    public:
        /** if these three offsets are 0, then add an utility_off. */
        /// use -1 to denote non-exist values.
        offsetNumber root_off;      /// get root block.
        offsetNumber head_off;      /// get head block when sequential scan.
        offsetNumber tail_off;      /// get tail block when sequential scan.
        /// append_off always exist. This can't be modified.
        offsetNumber append_off;    /// get a new block when append.

        /** logicSize means how many piece of data info in current B+ tree.
         * allocSize means how mnay blocks have been allocated by bufferManager.
         *
         * These two things must be read and write in file head as basic information,
         * so they are members of buffermanager.
         *
         * allocSize is invisible to B+ tree --------- set as private.
         * BufferManager knows little about logicSize. -------- set as public, letting B+ tree to maintain it.
         * */
    private:
        int allocSize;
    public:
        int logicSize;
        offsetNumber trash_off;     /// get head block of trash can.

    private:
        /**
         * try to open file, if fail, return false. */
        bool exist() {
            fp = fopen(filename, "r");
            if(fp == nullptr)
                return false;
            else {
                fclose(fp);
                return true;
            }
        }

        /**
         * create database file, assume file non-exist.
         * write in some basic information, including info for tree and node. */
        void createFile() {
            fp = fopen(filename, "w");

            trash_off = root_off = head_off = tail_off = -1;
            append_off = 0;
            logicSize = allocSize = 0;
            fwrite(&root_off, sizeof(int), 1, fp);
            fwrite(&head_off, sizeof(int), 1, fp);
            fwrite(&tail_off, sizeof(int), 1, fp);
            fwrite(&append_off, sizeof(int), 1, fp);
            fwrite(&trash_off, sizeof(int), 1, fp);
            fwrite(&logicSize, sizeof(int), 1, fp);
            fwrite(&allocSize, sizeof(int), 1, fp);

            fclose(fp);
        }

        /** open file, and read in some vital B-plus file field. */
        void init() {
            if(!exist()) {
                createFile();   // fields are init(ed) inside.
                fp = fopen(filename, "r+");
            }
            else {
                fp = fopen(filename, "r+");
                fread(&root_off, sizeof(int), 1, fp);
                fread(&head_off, sizeof(int), 1, fp);
                fread(&tail_off, sizeof(int), 1, fp);
                fread(&append_off, sizeof(int), 1, fp);
                fread(&trash_off, sizeof(int), 1, fp);
                fread(&logicSize, sizeof(int), 1, fp);
                fread(&allocSize, sizeof(int), 1, fp);
            }
        }

    public:
        BufferManager() {
            filename[0] = '\0';
            isOpened = false;
            trash_off = root_off = head_off = tail_off = -1;
            append_off = -1;        /// -1 is a special value for append_off under closed file condition.
            allocSize = logicSize = -1;
            fp = nullptr;
        }

        ~BufferManager() {
            if(isOpened) {
                close_file();
            }
        }

        /**
         * associate filename with BufferManager. */
        void set_fileName(const char *fname) {
            strcpy(filename, fname);
        }
        void clear_fileName() {
            filename[0] = '\0';
        }

        /**
         * open and close file for BufferManager.
         * when closing file, note that root_off, head_off, tail_off, append_off
         * are maintained by BufferManager class without written into the head of
         * file. They need writing into, of course. */
         bool open_file() {
            if(isOpened) {
                // std::cerr << "buffermanager open an opened file" << std::endl;
                return false;
            }
            else {
                init();
                isOpened = true;
                return true;
            }
         }
         bool close_file() {
             if(!isOpened) {
                 // std::cerr << "buffermanager close an closed file" << std::endl;
                 return false;
             }
             else {
                 fseek(fp, 0, SEEK_SET); /// write and close.
                 fwrite(&root_off, sizeof(int), 1, fp);
                 fwrite(&head_off, sizeof(int), 1, fp);
                 fwrite(&tail_off, sizeof(int), 1, fp);
                 fwrite(&append_off, sizeof(int), 1, fp);
                 fwrite(&trash_off, sizeof(int), 1, fp);
                 fwrite(&logicSize, sizeof(int), 1, fp);
                 fwrite(&allocSize, sizeof(int), 1, fp);

                 fclose(fp);

                 root_off = head_off = tail_off = -1;   /// maintain.
                 append_off = -1;        /// -1 is a special value for append_off under closed file condition.
                 logicSize = allocSize = -1;
                 fp = nullptr;
                 isOpened = false;

                 return true;
             }
         }
         bool is_opened() {
             return isOpened;
         }

        /** get things by predefined structure. */
        void get_block_by_offset(offsetNumber offset, Node &ret) {
            ret.addr = offset;

            fseek(fp, offset, SEEK_SET);

            fread(&ret.isLeaf, sizeof(bool), 1, fp);

            short K_size, V_size, Ch_size;
            fread(&K_size, sizeof(short), 1, fp);
            fread(&V_size, sizeof(short), 1, fp);
            fread(&Ch_size, sizeof(short), 1, fp);

            ret.keys.file_read(fp, K_size);
            ret.vals.file_read(fp, V_size);
            ret.childs.file_read(fp, Ch_size);

            fread(&ret.next, sizeof(offsetNumber), 1, fp);
        }

        /**
         * if fail, return false.
         * this function seems unnecessary. */
        bool get_next_block(const Node &cur, Node &ret) {
            // if(!cur.isLeaf && cur.next != -1) std::cerr << "branch node has next!" << std::endl;

            if(cur.next == -1)
                return false;
            else {
                get_block_by_offset(cur.next, ret);
                return true;
            }
        }

        /** if root_off == 0
         * root_off == -1 means no root. */
        bool get_root(Node &ret) {
            if(root_off == -1) return false;

            if(root_off == 0)
                get_block_by_offset(tree_utility_byte, ret);
            else
                get_block_by_offset(root_off, ret);
            return true;
        }
        bool get_head(Node &ret) {
            if(head_off == -1) return false;

            if(head_off == 0)
                get_block_by_offset(tree_utility_byte, ret);
            else
                get_block_by_offset(head_off, ret);
            return true;
        }
        bool get_tail(Node &ret) {
            if(tail_off == -1) return false;

            if(tail_off == 0)
                get_block_by_offset(tree_utility_byte, ret);
            else
                get_block_by_offset(tail_off, ret);
            return true;
        }

        /*
         * wrong code: block got by append_off is never wanted, becuase it will be empty.
         *              it can be replaced by append_block().
        void get_append(Node &ret) {
            if(append_off == 0)
                get_block_by_offset(utility_off, ret);
            else
                get_block_by_offset(append_off, ret);
        }
        */

        /** return a block which will be appended in file. block address is adjusted to good.
         * do memory operation first, write in disk later. */
        void append_block(Node &ret, bool isLeaf) {
            ret.clear();

            if(trash_off != -1) {
                ret.addr = trash_off;
                get_block_by_offset(trash_off, pool);
                trash_off = pool.next;
            }
            else {
                ret.addr = (append_off == 0) ? tree_utility_byte : append_off;
                append_off += blockSize;
                ++allocSize;
            }
            ret.isLeaf = isLeaf;
        }

        /** write node into where it belongs. */
        void write_block(Node &cur) {
            fseek(fp, cur.addr, SEEK_SET);

            fwrite(&cur.isLeaf, sizeof(bool), 1, fp);

            short tmp = cur.keys.size();        fwrite(&tmp, sizeof(short), 1, fp);
            tmp = cur.vals.size();              fwrite(&tmp, sizeof(short), 1, fp);
            tmp = cur.childs.size();            fwrite(&tmp, sizeof(short), 1, fp);

            cur.keys.file_write(fp);
            cur.vals.file_write(fp);
            cur.childs.file_write(fp);

            fwrite(&cur.next, sizeof(offsetNumber), 1, fp);
        }


        void traverse() {
            if(head_off == -1) {
                printf("traverse empty tree\n");
                return;
            }

            Node cur;
            get_block_by_offset(head_off, cur);

            while(true) {
                for(int i = 0; i < cur.keys.size(); ++i)
                    std::cout << cur.keys[i] << ' ' << cur.vals[i] << std::endl;

                if(cur.next == -1)
                    break;
                get_block_by_offset(cur.next, cur);
            }
        }
    };
};


#endif //DATABASE_BUFFERMANAGER_H
