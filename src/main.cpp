#include <st25vf010/st25vf010.hpp>
#include <memory>


int main (){

    auto st25vf {std::make_unique<ST25VF010::St25vf010_t>()};
    //st25vf->erase();
    //st25vf->write_rand();
    st25vf->write();
    st25vf->read();

return 0;    
}