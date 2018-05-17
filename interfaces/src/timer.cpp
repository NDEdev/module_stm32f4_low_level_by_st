#include "timer.h"

// Включаем тактирование SPI.
static void clkTimInit ( TIM_TypeDef* tim ) {
	switch ( ( uint32_t )tim ) {
#ifdef TIM
		case	TIM_BASE:		__HAL_RCC_TIM_CLK_ENABLE();			break;
#endif
#ifdef TIM1
		case	TIM1_BASE:		__HAL_RCC_TIM1_CLK_ENABLE();		break;
#endif
#ifdef TIM2
		case	TIM2_BASE:		__HAL_RCC_TIM2_CLK_ENABLE();		break;
#endif
#ifdef TIM3
		case	TIM3_BASE:		__HAL_RCC_TIM3_CLK_ENABLE();		break;
#endif
#ifdef TIM4
		case	TIM4_BASE:		__HAL_RCC_TIM4_CLK_ENABLE();		break;
#endif
#ifdef TIM5
		case	TIM5_BASE:		__HAL_RCC_TIM5_CLK_ENABLE();		break;
#endif
#ifdef TIM6
		case	TIM6_BASE:		__HAL_RCC_TIM6_CLK_ENABLE();		break;
#endif
#ifdef TIM7
		case	TIM7_BASE:		__HAL_RCC_TIM7_CLK_ENABLE();		break;
#endif
#ifdef TIM8
		case	TIM8_BASE:		__HAL_RCC_TIM8_CLK_ENABLE();		break;
#endif
#ifdef TIM9
		case	TIM9_BASE:		__HAL_RCC_TIM9_CLK_ENABLE();		break;
#endif
#ifdef TIM10
		case	TIM10_BASE:		__HAL_RCC_TIM10_CLK_ENABLE();		break;
#endif
#ifdef TIM11
		case	TIM11_BASE:		__HAL_RCC_TIM11_CLK_ENABLE();		break;
#endif
#ifdef TIM12
		case	TIM12_BASE:		__HAL_RCC_TIM12_CLK_ENABLE();		break;
#endif
#ifdef TIM13
		case	TIM13_BASE:		__HAL_RCC_TIM13_CLK_ENABLE();		break;
#endif
#ifdef TIM14
		case	TIM14_BASE:		__HAL_RCC_TIM14_CLK_ENABLE();		break;
#endif
#ifdef TIM15
		case	TIM15_BASE:		__HAL_RCC_TIM15_CLK_ENABLE();		break;
#endif
#ifdef TIM16
		case	TIM16_BASE:		__HAL_RCC_TIM16_CLK_ENABLE();		break;
#endif
	};
}

/*!
 * TimCounter.
 */
TimCounter::TimCounter ( const timCounterCfg* const cfg ) : cfg( cfg ) {
	this->tim.Instance						= this->cfg->tim;

	this->tim.Init.ClockDivision			= TIM_CLOCKDIVISION_DIV1;
	this->tim.Init.CounterMode				= TIM_COUNTERMODE_UP;
}

BASE_RESULT TimCounter::reinit ( uint32_t numberCfg ) {
	if ( numberCfg >= this->cfg->countCfg )
		return BASE_RESULT::INPUT_VALUE_ERROR;

	this->tim.Init.Period					= this->cfg->cfg[ numberCfg ].period;
	this->tim.Init.Prescaler				= this->cfg->cfg[ numberCfg ].prescaler;

	clkTimInit( this->tim.Instance );

	if ( HAL_TIM_Base_DeInit( &this->tim ) != HAL_OK )
		return BASE_RESULT::ERROR_INIT;

	if ( HAL_TIM_Base_Init( &this->tim ) != HAL_OK )
		return BASE_RESULT::ERROR_INIT;

	return BASE_RESULT::OK;
}

BASE_RESULT TimCounter::on ( void ) {
	if ( this->tim.State == HAL_TIM_STATE_RESET )
		return BASE_RESULT::ERROR_INIT;

	HAL_TIM_Base_Start( &this->tim );

	return BASE_RESULT::OK;
}

void TimCounter::off ( void ) {
	HAL_TIM_Base_Stop( &this->tim  );
}

uint32_t TimCounter::getCounter ( void ) {
	return this->tim.Instance->CNT;
}

//**********************************************************************
// tim_comp_one_channel
//**********************************************************************
TimCompOneChannel::TimCompOneChannel ( const timCompOneChannelCfg* const cfg ) : cfg( cfg ) {
	this->tim.Instance						= this->cfg->tim;

	this->tim.Init.ClockDivision			= TIM_CLOCKDIVISION_DIV1;
	this->tim.Init.CounterMode				= TIM_COUNTERMODE_UP;

	this->timCh.OCMode						= TIM_OCMODE_TOGGLE;
	this->timCh.OCPolarity					= this->cfg->polarity;
	this->timCh.Pulse						= 0;
}

BASE_RESULT TimCompOneChannel::reinit ( uint32_t numberCfg ) {
	if ( numberCfg >= this->cfg->countCfg )
		return BASE_RESULT::INPUT_VALUE_ERROR;

	this->tim.Init.Period					= this->cfg->cfg[ numberCfg ].period;
	this->tim.Init.Prescaler				= this->cfg->cfg[ numberCfg ].prescaler;
	this->timCh.Pulse						= this->cfg->cfg[ numberCfg ].pulse;

	clkTimInit( this->tim.Instance );

	if ( HAL_TIM_OC_DeInit( &this->tim ) != HAL_OK )
		return BASE_RESULT::ERROR_INIT;

	if ( HAL_TIM_OC_Init( &this->tim ) != HAL_OK )
		return BASE_RESULT::ERROR_INIT;

	if ( HAL_TIM_OC_ConfigChannel( &this->tim, &this->timCh, this->cfg->outChannel ) != HAL_OK )
		return BASE_RESULT::ERROR_INIT;

	return BASE_RESULT::OK;
}

BASE_RESULT TimCompOneChannel::on ( void ) {
	if ( this->tim.State == HAL_TIM_STATE_RESET )
		return BASE_RESULT::ERROR_INIT;

	HAL_TIM_OC_Start( &this->tim, this->cfg->outChannel );

	return BASE_RESULT::OK;
}

void TimCompOneChannel::off ( void ) {
	HAL_TIM_OC_Stop( &this->tim, this->cfg->outChannel );
}

//**********************************************************************
// tim_pwm_one_channel
//**********************************************************************
TimPwmOneChannel::TimPwmOneChannel ( const timPwmOneChannelCfg* const cfg ) : cfg( cfg ) {
	this->tim.Instance						= this->cfg->tim;

	this->tim.Init.ClockDivision			= TIM_CLOCKDIVISION_DIV1;
	this->tim.Init.CounterMode				= TIM_COUNTERMODE_UP;

	this->timCh.OCMode						= TIM_OCMODE_PWM2;
	this->timCh.OCPolarity					= this->cfg->polarity;
}

BASE_RESULT TimPwmOneChannel::reinit ( uint32_t numberCfg ) {
	if ( numberCfg >= this->cfg->countCfg )
		return BASE_RESULT::INPUT_VALUE_ERROR;

	this->tim.Init.Period					= this->cfg->cfg[ numberCfg ].period;
	this->tim.Init.Prescaler				= this->cfg->cfg[ numberCfg ].prescaler;

	clkTimInit( this->cfg->tim );

	if ( HAL_TIM_PWM_DeInit( &this->tim ) != HAL_OK )
		return BASE_RESULT::ERROR_INIT;

	if ( HAL_TIM_PWM_Init( &this->tim ) != HAL_OK )
		return BASE_RESULT::ERROR_INIT;

	if ( HAL_TIM_PWM_ConfigChannel( &this->tim, &this->timCh, this->cfg->outChannel ) != HAL_OK )
		return BASE_RESULT::ERROR_INIT;

	return BASE_RESULT::OK;
}

BASE_RESULT TimPwmOneChannel::on ( void ) {
	if ( this->tim.State == HAL_TIM_STATE_RESET )
		return BASE_RESULT::ERROR_INIT;

	HAL_TIM_PWM_Start( &this->tim, this->cfg->outChannel );

	return BASE_RESULT::OK;
}

void TimPwmOneChannel::off ( void ) {
	HAL_TIM_PWM_Stop( &this->tim, this->cfg->outChannel );
}

void TimPwmOneChannel::setDuty ( float duty ) {
	__HAL_TIM_SET_COMPARE( &this->tim, this->cfg->outChannel, this->cfg->tim->ARR * duty );
}

//**********************************************************************
// tim_interrupt
//**********************************************************************
TimInterrupt::TimInterrupt( const timInterruptCfg* const cfg ) : cfg( cfg ) {
	this->tim.Instance						= this->cfg->tim;

	this->tim.Init.ClockDivision			= TIM_CLOCKDIVISION_DIV1;
	this->tim.Init.CounterMode				= TIM_COUNTERMODE_UP;
}

BASE_RESULT TimInterrupt::reinit ( uint32_t numberCfg ) {
	if ( numberCfg >= this->cfg->countCfg )
		return BASE_RESULT::INPUT_VALUE_ERROR;

	this->tim.Init.Period					= this->cfg->cfg[ numberCfg ].period;
	this->tim.Init.Prescaler				= this->cfg->cfg[ numberCfg ].prescaler;

	clkTimInit( this->cfg->tim );

	if ( HAL_TIM_Base_DeInit( &this->tim ) != HAL_OK )
		return BASE_RESULT::ERROR_INIT;

	if ( HAL_TIM_Base_Init( &this->tim ) != HAL_OK )
		return BASE_RESULT::ERROR_INIT;

	return BASE_RESULT::OK;
}

BASE_RESULT TimInterrupt::on ( void ) {
	if ( this->tim.State == HAL_TIM_STATE_RESET )
		return BASE_RESULT::ERROR_INIT;

	HAL_TIM_Base_Start_IT( &this->tim );

	return BASE_RESULT::OK;
}

void TimInterrupt::off ( void ) {
	HAL_TIM_Base_Stop_IT( &this->tim );
}

void TimInterrupt::clearInterruptFlag ( void ) {
	HAL_TIM_IRQHandler( &this->tim );
}

//**********************************************************************
// tim_encoder
//**********************************************************************

TimEncoder::TimEncoder(const timEncoderCfg* const _cfg) : cfg(_cfg){

}

BASE_RESULT TimEncoder::reinit(uint32_t numberCfg){
	BASE_RESULT rv = BASE_RESULT::OK;

	if ( numberCfg >= this->cfg->countCfg )
			return BASE_RESULT::INPUT_VALUE_ERROR;

	clkTimInit( this->cfg->tim );

	tim.Instance 				= this->cfg[numberCfg].tim;
	tim.Init.ClockDivision		= TIM_CLOCKDIVISION_DIV4; // вроде не влияет не на что
	tim.Init.CounterMode		= TIM_COUNTERMODE_UP;
	tim.Init.Period				= this->cfg[numberCfg].period;
	tim.Init.Prescaler			= 0; // не влияет не на что
	tim.Init.RepetitionCounter	= 0; // не влияет не на что

	TIM_Encoder_InitTypeDef sCfg;
	sCfg.EncoderMode = TIM_ENCODERMODE_TI12;
	sCfg.IC1Polarity = TIM_ICPOLARITY_RISING;
	sCfg.IC1Selection = TIM_ICSELECTION_DIRECTTI;
	sCfg.IC1Prescaler = TIM_ICPSC_DIV1;
	sCfg.IC1Filter = 0;
	sCfg.IC2Polarity = TIM_ICPOLARITY_RISING;
	sCfg.IC2Selection = TIM_ICSELECTION_DIRECTTI;
	sCfg.IC2Prescaler = TIM_ICPSC_DIV1;
	sCfg.IC2Filter = 0;

	if(HAL_TIM_Encoder_Init(&this->tim, &sCfg) != HAL_OK)
		rv = BASE_RESULT::INPUT_VALUE_ERROR;

	return rv;
}

BASE_RESULT TimEncoder::on(void){
	BASE_RESULT rv = BASE_RESULT::OK;

	if(HAL_TIM_Encoder_Start(&this->tim, TIM_CHANNEL_ALL) != HAL_OK)
		rv = BASE_RESULT::ERROR_INIT;

	return rv;
}

void TimEncoder::off(void){
	HAL_TIM_Encoder_Stop(&this->tim, TIM_CHANNEL_ALL);
}

uint32_t	TimEncoder::getEncoderCounts (void){
	return this->tim.Instance->CNT;
}

