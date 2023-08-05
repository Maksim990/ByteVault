#include <bytevault.hpp>
#include <iostream>

namespace bytevault{
    /* Data-related stuff */

    void __db_data::destroy() {
        free(data_ptr);
    }

    /* Database class related stuff */

    db::db(__db_header h, __db_data d) {
        header = h;
        data = d;
    }

    void db::print_header() {
        for (auto &&k : header){
            std::cout << k.first << " : " << k.second << '\n';
        }
        
    }

    /* Some functions */
    db from_file(std::string path) {
        // 1. Read the file to a byte array
        std::ifstream file(path, std::ios::binary);

        if(!file) {
            /* Throw an exception */
        }

        file.seekg(0, std::ifstream::end);
        std::streampos file_size = file.tellg();
        file.seekg(0, std::ios::beg);

        u_char buff[(int)file_size];

        file.read(reinterpret_cast<char*>(buff), file_size);

        // 2. Get the header binary
        int pos = 4;
        int header_size = 0;
        memcpy(&header_size, &buff, 4);
        void* header_bin = malloc(header_size);
        memcpy(header_bin, buff+4, header_size);

        pos += header_size;

        //3. Get the data binary
        int data_size = file_size-pos-1;
        void* data_bin = malloc(data_size);
        memcpy(data_bin, buff+pos, data_size);

        //4. Convert header to map
        __db_header header_map;

        for (int i = 0; i < header_size; i++) {
            int key_length;

            // Read the length of the key (4 bytes)
            memcpy(&key_length, static_cast<u_char*>(header_bin) + i, 4);
            i += 4;

            // Read the key name
            char key_name[key_length + 1];
            memcpy(key_name, static_cast<u_char*>(header_bin) + i, key_length);
            key_name[key_length] = 0; // null-terminate the string
            i += key_length;

            // Read the index of the key (4 bytes)
            int key_index;
            memcpy(&key_index, static_cast<u_char*>(header_bin) + i, 4);
            i += 4;
            header_map[std::string(key_name)] = key_index;
        }

        //5. Build the db class
        __db_data data;
            data.data_ptr = data_bin;
            data.data_size = data_size;
        
        db database(header_map, data);
        file.close();
        return database;
    }
}