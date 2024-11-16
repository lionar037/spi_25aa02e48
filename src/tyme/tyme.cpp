//tyme.cpp
#include <tyme/tyme.hpp>
#include <chrono>
#include <thread>
#include <iomanip>
#include <sstream>
//#include <unistd.h> // Libreria para usleep

namespace TYME{

    Time_t::Time_t(){
        #ifdef DBG_TYME
            std::cout << " Time_t()\n";
        #endif
    }

    void 
    Time_t::delay_ms(const uint32_t t){
        std::chrono::microseconds delay_loc(t);
        std::this_thread::sleep_for(delay_loc);
    return;
    }

    const 
    std::string  Time_t::get_tyme()
    {
        auto now = std::chrono::system_clock::now();

        // Convertir el tiempo actual a una estructura de tiempo local
        std::time_t currentTime = std::chrono::system_clock::to_time_t(now);
        std::tm localTime = *std::localtime(&currentTime);

        // Formatear la fecha y hora según tu especificación
        std::ostringstream oss;                        
        oss << std::put_time(&localTime, "%Y%m%d%H%M%S");
        #ifdef DBG_TYME
            std::cout<< oss.str()<< "\n";
        #endif
        std::string tyme = oss.str();
        return tyme;
    }


    void 
    delay_ms(const uint64_t t){
        std::chrono::microseconds delay_loc(t);
        std::this_thread::sleep_for(delay_loc);
    }
}