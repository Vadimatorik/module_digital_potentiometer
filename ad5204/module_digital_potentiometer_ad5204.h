#include "user_os.h"
#include "mk_hardware_interfaces_spi.h"

struct ad5204_struct_cfg_t {
    spi_master_8bit_base*       spi;
    USER_OS_STATIC_MUTEX*       mutex;      // Для предотвращения коллизия на шине SPI.

};

template < uint8_t COUNT >              // Колличество потенциометров на шине SPI.
class ad5204 {

};
