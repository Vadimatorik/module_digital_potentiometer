#include "module_digital_potentiometer_ad5204_struct.h"

enum class EC_AD5204_ANSWER {
    OK              = 0,
    ARG_ERROR       = 1,
	SPI_ERROR		= 2
};

template < uint8_t COUNT >              // Колличество потенциометров на шине SPI.
class ad5204 {
public:
    constexpr ad5204 ( ad5204_struct_cfg_t* cfg );

    void    connect_on     ( void );
    void    connect_off    ( void );

    EC_AD5204_ANSWER    value_set           ( uint8_t chip_number, uint8_t reg, uint8_t value );
    EC_AD5204_ANSWER    value_read          ( uint8_t chip_number, uint8_t reg, uint8_t& out_result_data );

private:
    const ad5204_struct_cfg_t*          const cfg;
    // Номер чипа, данные каналов.
    mutable uint8_t buf[ COUNT ][ 4 ] = { 0 };   // Состояние каждого регистра каждого потенциометра.
};

#include "module_digital_potentiometer_ad5204_func.h"
