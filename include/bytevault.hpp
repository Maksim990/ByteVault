#ifndef BYTEVAULT_H
#define BYTEVAULT_H

#include <map>
#include <string>
#include <malloc.h>

namespace bytevault {

    typedef std::map<std::string, int> __db_header;

    // Indicates type of the key
    enum key_type {
        BIN, LIST, MAP
    };

    // Contains information about the key
    struct db_key {
        int size;
        void* value;
    };
    
    // Contains data of the database
    class __db_data {
    public:
        int data_size;
        void* data_ptr;

        ~__db_data();
    };

    // Main database class
    class db {
    private:
        __db_header header;
        __db_data data;
    public:
        db(__db_header h, __db_data d);

        void set_key(std::string name, db_key value);
        db_key get_key(std::string name);

        void to_file(std::string path);        
    };

    // Loads database from a file
    db from_file(std::string path);
}

#endif