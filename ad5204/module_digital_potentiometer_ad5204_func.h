#pragma once

#include "module_digital_potentiometer_ad5204_struct.h"

template < uint8_t COUNT >
constexpr ad5204< COUNT >::ad5204 ( ad5204_struct_cfg_t* cfg ) : cfg( cfg ) {}

template < uint8_t COUNT >
void ad5204< COUNT >::connect_on ( void ) {
    this->cfg->shdn->set();
}

template < uint8_t COUNT >
void ad5204< COUNT >::connect_off ( void ) {
    this->cfg->shdn->reset();
}

// Смотрим, сколько целых байт занимает буфер +
// если хоть 1 бит еще нужен сверху целого количества, добавляем 1 байт.
#define AD5204_BUF_SIZE(COUNT)                ( ( COUNT * 11 ) / 8 ) + ( ( ( ( COUNT * 11 ) % 8) != 0 ) ? 1 : 0 )

template < uint8_t COUNT >
EC_AD5204_ANSWER ad5204< COUNT >::value_set ( uint8_t chip_number, uint8_t reg, uint8_t value ) {
    if ( ( chip_number >= COUNT ) | ( reg >= 4 ) ) return EC_AD5204_ANSWER::ARG_ERROR;

    this->buf[ chip_number ][reg] = value;      // Сохраняем у себя дубликат.
    uint8_t b[ AD5204_BUF_SIZE(COUNT) + 1 ] = { 0 };       // Буфер на вывод по SPI.
    uint8_t p_b = 0;                            // Указатель на элемент буфера.
    uint8_t sm = 0;                             // Смещение от начала байта (справа).


    for ( uint32_t l = 0; l < COUNT; l++ ) {
        b[ p_b ]    |= this->buf[ l ][reg] << sm;
        b[ p_b + 1] |= this->buf[ l ][reg] >> ( 8 - sm );
        p_b++;
        b[ p_b ]    |= reg << sm;
        b[ p_b + 1] |= reg >> ( 8 - sm );
        sm += 3;
        if ( sm >= 8 ) {
            p_b++;
            sm -= 8;
        }
    }

    // Передаем пакет.
    if ( this->cfg->mutex != nullptr)
        USER_OS_TAKE_MUTEX( *this->cfg->mutex, portMAX_DELAY );

    BASE_RESULT tx_res;

    this->cfg->cs->reset();
    tx_res = this->cfg->spi->tx( &b[ AD5204_BUF_SIZE(COUNT) - 1 ], AD5204_BUF_SIZE(COUNT), 10, SPI::STEP_MODE::DEC );
    this->cfg->cs->set();

    if ( this->cfg->mutex != nullptr)
        USER_OS_GIVE_MUTEX( *this->cfg->mutex );

    if ( tx_res == BASE_RESULT::OK ) {
    	return EC_AD5204_ANSWER::OK;
    } else {
    	return EC_AD5204_ANSWER::SPI_ERROR;
    }
}

template < uint8_t COUNT >
EC_AD5204_ANSWER ad5204< COUNT >::value_read ( uint8_t chip_number, uint8_t reg, uint8_t& out_result_data ) {
    if ( ( chip_number < COUNT ) && ( reg < 4 ) ) {
        out_result_data = this->buf[ chip_number ][reg];
        return EC_AD5204_ANSWER::OK;
    }
    return EC_AD5204_ANSWER::ARG_ERROR;
}
