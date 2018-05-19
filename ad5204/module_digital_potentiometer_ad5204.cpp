#ifdef MODULE_DIGITAL_POTENTIOMETER_ENABLED

#include "module_digital_potentiometer_ad5204.h"
#include <string.h>

ad5204::ad5204( const ad5204Cfg* const cfg ) : cfg( cfg ) {
#ifdef DEBUG
	assertParam( cfg );
#endif
}

void ad5204::connectOn ( void ) {
	this->cfg->shdn->set();
}

void ad5204::connectOff ( void ) {
	this->cfg->shdn->reset();
}

enum class AD5204_WRITE_TYPE {
	REG				=	0,
	DATA			=	1
};

EC_AD5204_ANSWER ad5204::valueSet ( const uint8_t chipNumber, const uint8_t reg, uint8_t const value ) {
	/// Проверка на на выход за пределы (несуществующий чип ли регистр).
	if ( ( chipNumber >= this->cfg->countChip ) | ( reg >= 4 ) )
		return EC_AD5204_ANSWER::ARG_ERROR;

	/// Сохраняем у себя дубликат.
	this->cfg->internalStructData[ chipNumber ].regData[ reg ]	=	value;

	int smByte	=	this->cfg->arraySize - 1;
	int smBit	=	0;

	memset( this->cfg->bufOutput, 0, this->cfg->arraySize );

	/// Для всех чипов пишем в единый массив.
	for ( uint32_t numberChip = 0; numberChip < this->cfg->countChip; numberChip++ ) {
		/// Пишем первую часть данных.
		this->cfg->bufOutput[ smByte ]	|=	this->cfg->internalStructData[ numberChip ].regData[ reg ] << smBit;
		this->cfg->bufOutput[ smByte - 1 ]	|=	this->cfg->internalStructData[ numberChip ].regData[ reg ] >> ( 8 - smBit );

		smByte--;

		/// Записываем первую часть регистра.
		this->cfg->bufOutput[ smByte ]		|=	reg << smBit;
		this->cfg->bufOutput[ smByte - 1 ]	|=	reg >> ( 8 - smBit );

		smBit += 3;
		if ( smBit > 7 ) {
			smBit -= 8;
			smByte--;
		}
	}

	// Передаем пакет.
	if ( this->cfg->mutex != nullptr)
		USER_OS_TAKE_MUTEX( *this->cfg->mutex, portMAX_DELAY );

	BASE_RESULT tx_res;

	this->cfg->cs->reset();
	tx_res = this->cfg->spi->tx( &this->cfg->bufOutput[ 1 ], this->cfg->arraySize - 1, 10 );
	this->cfg->cs->set();

	if ( this->cfg->mutex != nullptr)
		USER_OS_GIVE_MUTEX( *this->cfg->mutex );

	if ( tx_res == BASE_RESULT::OK ) {
		return EC_AD5204_ANSWER::OK;
	} else {
		return EC_AD5204_ANSWER::SPI_ERROR;
	}
}

EC_AD5204_ANSWER ad5204::valueRead ( const uint8_t chipNumber, const uint8_t reg, uint8_t& outResultData ) {
	if ( ( chipNumber <  this->cfg->countChip ) && ( reg < 4 ) ) {
		outResultData = this->cfg->internalStructData[ chipNumber ].regData[ reg ];
		return EC_AD5204_ANSWER::OK;
	}
	return EC_AD5204_ANSWER::ARG_ERROR;
}

#endif
