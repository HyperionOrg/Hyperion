#include "Hyperion/HyperUtilities/VarLong.h"

#include "Hyperion/Core.h"

namespace VarLong
{
    std::vector<uint8_t> Encode(int64_t value)
    {
        std::vector<uint8_t> data{};
        do
        {
            int8_t temp = static_cast<int8_t>(value & 0b01111111);
            value = value >> 7;

            if (value != 0)
                temp |= 0b10000000;

            data.push_back(temp);
        } while (value != 0);
        return data;
    }

    int64_t Decode(std::vector<uint8_t>& bytes)
    {
        size_t index = 0;

        int32_t numbersRead = 0;
        int64_t result = 0;
        int8_t byteRead = 0;
        do
        {
            byteRead = bytes[index];
            int64_t value = (byteRead & 0b01111111);
            result |= (value << (7 * numbersRead));

            HP_ASSERT(numbersRead++ <= 10, "VarLong is too big!");

            index++;
        } while ((byteRead & 0b10000000) != 0);

        bytes.erase(bytes.begin(), bytes.begin() + index);

        return result;
    }
}