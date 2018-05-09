#pragma once

#ifdef MODULE_DIGITAL_POTENTIOMETER_ENABLED

#include "user_os.h"
#include "mc_hardware_interfaces_spi.h"
#include "mc_hardware_interfaces_pin.h"

struct ad5204_struct_cfg_t {
	SpiMaster8BitBase*			const spi;
	USER_OS_STATIC_MUTEX*		const mutex;		// Для предотвращения коллизия на шине SPI.
	PinBase*					const cs;			// Выбор чипа(-ов). Активный 0.
	PinBase*					const shdn;			// Прямое включение (см. документацию).
};

#endif
