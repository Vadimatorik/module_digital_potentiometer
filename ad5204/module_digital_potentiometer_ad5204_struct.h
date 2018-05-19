#pragma once

#ifdef MODULE_DIGITAL_POTENTIOMETER_ENABLED

#include "user_os.h"
#include "mc_hardware_interfaces_spi.h"
#include "mc_hardware_interfaces_pin.h"

/// Смотрим, сколько целых байт занимает буфер +
/// если хоть 1 бит еще нужен сверху целого количества, добавляем 1 байт.
/// +1 байт еще для "разбега" алгоритма. Чтобы не делать лишних проверок
/// на выход за пределы массива
#define AD5204_BUF_SIZE(COUNT)				( ( COUNT * 11 ) / 8 ) + ( ( ( ( COUNT * 11 ) % 8) != 0 ) ? 1 : 0 ) + 1

/// Внутренняя структура буфер, которая хранит текущие
/// значения внутри потенциометра (одной микросхемы).
struct ad5204chipData {
	uint8_t				regData[ 4 ];
};

struct ad5204Cfg {
	SpiMaster8BitBase*			const spi;
	USER_OS_STATIC_MUTEX*		const mutex;		/// Для предотвращения коллизия на шине SPI.
	PinBase*					const cs;			/// Выбор чипа(-ов). Активный 0.
	PinBase*					const shdn;			/// Прямое включение (см. документацию).
	const uint32_t				countChip;			/// Количество потенциометров, включенных последовательно.
	uint8_t*					bufOutput;			/// Массив для внутренних нужд
													/// (размер получать макросом AD5204_BUF_SIZE).
	uint32_t					arraySize;			/// AD5204_BUF_SIZE.
	ad5204chipData*				internalStructData;	/// Количество структур равно countChip.
};

#endif
