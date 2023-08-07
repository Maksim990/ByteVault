#ifndef BYTEVAULT_H
#define BYTEVAULT_H

#include <map>
#include <string>
#include <malloc.h>
#include <fstream>
#include <memory.h>
#include <vector>

namespace bytevault {

    extern "C" {

        typedef std::map<std::string, int> __db_header;

        // Indicates type of the key
        enum key_type {
            BIN, LIST, MAP
        };

        // Contains information about the key
        struct db_key {
            key_type type;
            int size;
            void* value;
        };
        
        // Contains data of the database
        class __db_data {
        public:
            int data_size;
            void* data_ptr;

            void destroy();
        };

        // Main database class
        class db {
        private:
            __db_header header;
            __db_data data;
        public:
            db(__db_header h, __db_data d);

            void set_key(std::string name, db_key value);
            // function to retrieve a key from the database by name
            db_key get_key(std::string name);

            void to_file(std::string path);

            /* Debug-related stuff */
            void print_header();       
        };

        // Loads database from a file
        db from_file(std::string path);
        // function to read an integer from a byte array
        int read_int_from_buffer(const u_char* buffer);
        // function to read the key type from a byte array
        key_type read_key_type(const u_char* buffer);
        // function to read the key data from a byte array
        void* read_key_data(const u_char* buffer, int size);
    }
}
#endif