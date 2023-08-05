#include <bytevault.hpp>

namespace bytevault{
    /* Data-related stuff */
    __db_data::~__db_data() {
        free(data_ptr);
    }

    /* Some functions */
    db from_file(std::string path) {
        // 1. Read the file to a byte array
        std::ifstream file(path, std::ios::binary);

        if(!file) {
            /* Throw an exception */
        }

        file.seekg(0, std::ios::end);
        std::streampos file_size = file.tellg();
        file.seekg(0, std::ios::beg);

        u_char buff[file_size];

        file.read(reinterpret_cast<char*>(buff), file_size);

        // 2. Get the header binary
        int pos = 4;
        int header_size;
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

            // REad the length of the key (4 bytes)
            memcpy($key_length, static_cast<u_char*>(header_bin) + i, 4);
            i += 4;

            //  read the key name
            char key_name[key_length + 1];
            memcpy(key_name, statuc_cast<u_char*>(header_bin) + index, key_length);
            key_name[key_length] = "\0"; // null-terminate the string
            i += key_length;

            // кead the index of the key (4 bytes)
            int key_index;
            memcpy($key_index, static_cast<u_char*>(header_bin) + i, 4);
            i += 4;

            header_map[std::string(key_name)] = key_index;
        }
    }
}