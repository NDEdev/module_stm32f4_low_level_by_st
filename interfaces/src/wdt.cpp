#include "wdt.h"

BASE_RESULT Wdt::reinit ( uint32_t numberCfg ) {
	if ( numberCfg >= this->countCfg )
		return BASE_RESULT::INPUT_VALUE_ERROR;

	this->nowCfg	=	numberCfg;

	*((uint32_t *) 0xE0042008)	= 0x1DFF;				// Если процессор находится в состоянии halt, watchdog будет стоять и не мешать отладке.

	IWDG->KR	= 0xCCCC;
	IWDG->KR	= 0x5555;
	IWDG->PR	= 0x6;
	IWDG->RLR	= 32 * this->cfg[ numberCfg].startupTimeMs / 256;

	while( IWDG->SR );

	IWDG->KR	= 0xAAAA;								// Перезагружаем WDT.

	USER_OS_STATIC_TASK_CREATE( this->task, "wdt", 64, ( void* )this, this->cfg->taskPrio, this->task_stack, &this->task_struct );

	return BASE_RESULT::OK;
}

void Wdt::reset ( void ) {
	IWDG->KR = 0x5555;//ключ, разрешающий запись
	IWDG->RLR = 40 * this->cfg[ this->nowCfg ].runTimeMs / 256;
	IWDG->KR = 0xAAAA;//ключ, перезагружающий таймер

}

// Перезапуск таймера для сервисных операций
void Wdt::resetService ( void ) {
	IWDG->KR = 0x5555;//ключ, разрешающий запись
	IWDG->RLR = 40 * this->cfg[ this->nowCfg  ].serviceTimeMs / 256;
	IWDG->KR = 0xAAAA;//ключ, перезагружающий таймер
}

//**********************************************************************
// Потоки.
//**********************************************************************
void Wdt::task ( void* p_obj ) {
	Wdt* obj = ( Wdt * )p_obj;
	USER_OS_TICK_TYPE last_wake_time;
	const USER_OS_TICK_TYPE time_out = 10;
	last_wake_time = USER_OS_TASK_GET_TICK_COUNT();
	while ( true ) {
		configASSERT( !obj->reboot );					// Если задана перезагрузка - виснем!
		obj->reset();
		USER_OS_TASK_DELAY_UNTIL( &last_wake_time, time_out );
	}
}
