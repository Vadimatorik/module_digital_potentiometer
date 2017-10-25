#pragma once

#include "user_os.h"
#include "mc_hardware_interfaces_spi.h"
#include "mc_hardware_interfaces_pin.h"

struct ad5204_struct_cfg_t {
    spi_master_8bit_base*       const spi;
    USER_OS_STATIC_MUTEX*       const mutex;      // Для предотвращения коллизия на шине SPI.
    pin_base*                   const cs;         // Выбор чипа(-ов). Активный 0.
    pin_base*                   const shdn;       // Прямое включение (см. документацию).
};
