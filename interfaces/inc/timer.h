#pragma once

#include "mc_hardware_interfaces_timer.h"
#include "stm32f4xx_hal_tim.h"
#include "stm32f4xx_hal_rcc.h"

struct clkTimBaseCfg {
	const uint32_t					period;					// 0..0xFFFF или 0..0xFFFFFFFF
	const uint32_t					prescaler;				// 0..0xFFFF или 0..0xFFFFFFFF.

	/// Используется только в:
	/// 1. timCompOneChannelCfg
	const uint16_t					pulse;					// 0..0xFFFF или 0..0xFFFFFFFF.
};

struct timCounterCfg {
	// Используемый таймер.
	TIM_TypeDef*					tim;

	const clkTimBaseCfg*			const cfg;
	uint32_t						countCfg;
};

class TimCounter : public TimCounterBase {
public:
	TimCounter( const timCounterCfg* const cfg );

	BASE_RESULT		reinit					( uint32_t numberCfg = 0 );

	BASE_RESULT		on						( void );
	void			off						( void );

	uint32_t		getCounter				( void );

private:
	const timCounterCfg*		const cfg;

	TIM_HandleTypeDef				tim;
};

struct timCompOneChannelCfg {
	// Используемый таймер.
	TIM_TypeDef*					tim;

	const clkTimBaseCfg*			const cfg;
	uint32_t						countCfg;

	// Параметры выходного канала.
	uint8_t							outChannel;		// TIM_CHANNEL_x.
	uint32_t						polarity;		// TIM_OCPOLARITY_LOW  / TIM_OCPOLARITY_HIGH.

};

class TimCompOneChannel : public TimCompOneChannelBase {
public:
	TimCompOneChannel( const timCompOneChannelCfg* const cfg );

	BASE_RESULT		reinit					( uint32_t numberCfg = 0 );

	BASE_RESULT		on						( void );
	void			off						( void );

private:
	const timCompOneChannelCfg*		const cfg;

	TIM_HandleTypeDef				tim;
	TIM_OC_InitTypeDef				timCh;
};

struct timPwmOneChannelCfg {
	// Используемый таймер.
	TIM_TypeDef*					tim;

	const clkTimBaseCfg*			const cfg;
	uint32_t						countCfg;

	// Параметры выходного канала.
	uint8_t							outChannel;			// TIM_CHANNEL_x.
	uint32_t						polarity;			// TIM_OCPOLARITY_LOW  / TIM_OCPOLARITY_HIGH.
};

class TimPwmOneChannel : public TimPwmOneChannelBase {
public:
	TimPwmOneChannel( const timPwmOneChannelCfg* const cfg );

	BASE_RESULT		reinit					( uint32_t numberCfg = 0 );

	BASE_RESULT		on						( void );
	void			off						( void );

	void			setDuty					( float duty );

private:
	const timPwmOneChannelCfg*		const cfg;

	TIM_HandleTypeDef				tim;
	TIM_OC_InitTypeDef				timCh;
};

struct timInterruptCfg {
	TIM_TypeDef*					tim;
	const clkTimBaseCfg*			const cfg;
	uint32_t						countCfg;
};

class TimInterrupt : public TimInterruptBase {
public:
	TimInterrupt( const timInterruptCfg* const cfg );

	BASE_RESULT		reinit							( uint32_t numberCfg = 0 );

	BASE_RESULT		on								( void );
	void			off								( void );

	void			clearInterruptFlag				( void );

private:
	const timInterruptCfg*			const cfg;

	TIM_HandleTypeDef				tim;
};

struct timEncoderCfg {
	TIM_TypeDef		*tim;	// Сущность таймера
	uint32_t 		period;	// Потолок инкрементирования после которого счётчик сбросится в 0 и продолжить свою работу
	uint32_t		countCfg;
};


class TimEncoder : public TimEncoderBase{
public:
	TimEncoder(const timEncoderCfg* const _cfg); // cfg - указатель на структуру или на массив структур

	BASE_RESULT		reinit							( uint32_t numberCfg = 0 );

	BASE_RESULT		on								( void );
	void			off								( void );

	uint32_t		getEncoderCounts			( void );

private:
	TIM_HandleTypeDef				tim;

	const timEncoderCfg* const 		cfg;
};
