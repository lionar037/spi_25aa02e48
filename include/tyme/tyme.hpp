//tyme.hpp
#pragma once
#include <iostream>
#include <cstring>

namespace TYME{

struct Time_t
    {
        public:
        explicit Time_t();
        ~Time_t()=default;
        void delay_ms(const uint32_t);
        const std::string get_tyme();
    };

    void delay_ms(const uint64_t t);

}//end time