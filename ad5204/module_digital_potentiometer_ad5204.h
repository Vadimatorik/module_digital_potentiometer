#pragma once

#ifdef MODULE_DIGITAL_POTENTIOMETER_ENABLED

#include "module_digital_potentiometer_ad5204_struct.h"

enum class EC_AD5204_ANSWER {
	OK				= 0,
	ARG_ERROR		= 1,
	SPI_ERROR		= 2
};

class ad5204 {
public:
	ad5204 ( const ad5204Cfg* const cfg );

	void	connectOn						(	void	);
	void	connectOff						(	void	);

	EC_AD5204_ANSWER	valueSet			(	const uint8_t	chipNumber,
												const uint8_t	reg,
												const uint8_t	value	);

	EC_AD5204_ANSWER	valueRead			(	const uint8_t	chipNumber,
												const uint8_t	reg,
												uint8_t&		outResultData	);

private:
	const ad5204Cfg*			const cfg;
};

#endif
