#include <bytevault.hpp>
#include <iostream>

namespace bytevault {
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

    db_key db::get_key(std::string name) {
        auto it = header.find(name);
        if (it == header.end()) {
            // key not found, return a default db_key
            return db_key{BIN, 0, nullptr};
        }

        int key_index = it->second;
        u_char* key_data_ptr = static_cast<u_char*>(data.data_ptr) + key_index;

        // Read the key type (4 bytes)
        key_type type = read_key_type(key_data_ptr);
        key_data_ptr += sizeof(int);

        // Read the key size (4 bytes)
        int key_size = read_int_from_buffer(key_data_ptr);
        key_data_ptr += sizeof(int);

        // Read the key value data
        void* key_value = read_key_data(key_data_ptr, key_size);

        // Create and return the db_key
        return db_key{type, key_size, key_value};
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

    int read_int_from_buffer(const u_char* buffer) {
        int value;
        memcpy(&value, buffer, sizeof(int));
        return value;
    }

    key_type read_key_type(const u_char* buffer) {
        int type;
        memcpy(&type, buffer, sizeof(int));
        return static_cast<key_type>(type);
    }

    void* read_key_data(const u_char* buffer, int size) {
        void* data = malloc(size);
        memcpy(data, buffer, size);
        return data;
    }
}