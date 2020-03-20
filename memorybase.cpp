// memorybase.cpp : This file contains the 'main' function. Program execution begins and ends there.
//


#include <iostream>
#include <cstdint>
#include <cstdlib>


enum  class byte_order : char {
    LITTLE_ENDIAN = 0x00, // place the the lsb , msb
    BIG_ENDIAN = 0x01 // place the most signicant bit first , then less signicant bits.
};

class memory_base {

private:
    size_t start_address = 0x00;
    char* memory_ptr; 
    size_t size;
    byte_order byteorder;

public:
    memory_base(const size_t size = 1024, byte_order byteorder = byte_order::BIG_ENDIAN) {
        memory_ptr = new char[size];
        this->size = size;
        for (size_t i = 0; i < size; i++) {
            memory_ptr[i] = 0x00;
        }
        this->byteorder = byteorder;
    }

    const char read(const size_t index) {
        char result = '\0';
        auto address = (start_address + index);
        if ((address >= start_address) && (address <= size)) {
            result = memory_ptr[index];
        }
        return result;
    }

    size_t write(const size_t index, char const sbyte) {
        size_t nsize = 0;
        auto address = (start_address + index);
        if ((address >= start_address) && (address <= size)) {
            memory_ptr[address] = sbyte;
        }
        return nsize;
    }

    const float read_float16_t(const size_t index) {
        auto result = this->read_int16_t(index);
        return (float)result;
    }

    const int32_t read_int32_t(const size_t index) {
        int32_t result = 0;
        int32_t msb = 0;
        int32_t lsb = 0;

        auto word1 = this->read_int16_t(index);
        auto word2 = this->read_int16_t(index + 2);

        if (this->byteorder == byte_order::BIG_ENDIAN) {
            msb = (0xFFFFFFFF & word1) << 16;
            lsb = (0xFFFFFFFF & word2);
        }
        else {
            msb = (0xFFFFFFFF & word2) << 16;
            lsb = (0xFFFFFFFF & word1);
        }
        result = (lsb | msb);
        return result;
    }

    const float read_float_32(const size_t index) {
        return static_cast<float>(this->read_int32_t(index));
    }

    const int16_t read_int16_t(size_t index) {
        auto byte1 = static_cast<int16_t>(read(index));
        auto byte2 = static_cast<int16_t>(read(index + 1));
        int16_t msb = 0;
        int16_t lsb = 0;

        if (this->byteorder == byte_order::BIG_ENDIAN) {
            msb = (0xFFFF & byte1) << 8;
            lsb = (0xFFFF & byte2);

        }
        else {
            msb = (0xFFFF & byte2) << 8;
            lsb = (0xFFFF & byte1);
        }
        return (int16_t)(msb | lsb);
    }
};


using namespace std;

int main()
{
    memory_base memory(10,byte_order::BIG_ENDIAN);

    memory.write(1, 89);
    memory.write(2, 89);

    auto value = memory.read_float16_t(1);
    cout << value << endl;
  
    return 0;
}

