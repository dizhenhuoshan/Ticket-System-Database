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
        friend class debugger;
        friend class BPlusTree;
    private:
        typedef TreeNode<KeyType, ValType> Node;

        char filename[50];
        FILE *fp;

        bool isOpened;              /// has file pointer opened file.

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

            root_off = head_off = tail_off = -1;
            append_off = 0;
            fwrite(&root_off, sizeof(int), 1, fp);
            fwrite(&head_off, sizeof(int), 1, fp);
            fwrite(&tail_off, sizeof(int), 1, fp);
            fwrite(&append_off, sizeof(int), 1, fp);

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
            }
        }

    public:
        BufferManager() {
            filename[0] = '\0';
            isOpened = false;
            root_off = head_off = tail_off = -1;
            append_off = -1;        /// -1 is a special value for append_off under closed file condition.
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
                 fclose(fp);

                 root_off = head_off = tail_off = -1;   /// maintain.
                 append_off = -1;        /// -1 is a special value for append_off under closed file condition.
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

        /** if root_off == 0 */
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
            ret.addr = (append_off == 0) ? tree_utility_byte : append_off;
            ret.isLeaf = isLeaf;

            append_off += blockSize;
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

        /** forgive me, it's just such ugly. */
        void write_block_skip_father(Node &cur) {
            fseek(fp, cur.addr, SEEK_SET);

            fwrite(&cur.isLeaf, sizeof(bool), 1, fp);

            short tmp = cur.keys.size();        fwrite(&tmp, sizeof(short), 1, fp);
            tmp = cur.vals.size();              fwrite(&tmp, sizeof(short), 1, fp);
            tmp = cur.childs.size();            fwrite(&tmp, sizeof(short), 1, fp);

            cur.keys.file_write(fp);
            cur.vals.file_write(fp);
            cur.childs.file_write(fp);

            fseek(fp, sizeof(offsetNumber), SEEK_CUR);
            fwrite(&cur.next, sizeof(offsetNumber), 1, fp);
        }
        void set_root(offsetNumber rtoff) {
            root_off = rtoff;
        }

        offsetNumber get_root() {
            return root_off;
        }

        /*
        /// check whether parent and children are linked properly.
        void check_link() {
            if(root_off == -1) {
                printf("tree is empty, link properly\n");
                return;
            }

            std::queue<offsetNumber> q;
            q.push(root_off);

            while(!q.empty()) {
                offsetNumber off;
                Node node;

                off = q.front();
                q.pop();
                get_block_by_offset(off, node);

                for(int i = 0; i < node.childs.size(); ++i) {
                    Node ch;
                    get_block_by_offset(node.childs[i], ch);
                    if(ch.parent != node.addr) {
                        fprintf(stderr, "link is break\n");
                        return;
                    }
                }
            }
            printf("tree links properly\n");
        }
        */
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
