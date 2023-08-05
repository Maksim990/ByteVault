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
    }
}