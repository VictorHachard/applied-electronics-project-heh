// ===============================================
// File:   rtc_ds1307.h
// Desc:   Header file for rtc_ds1307.h
// ===============================================

#ifndef RTC_DS1307_H
#define RTC_DS1307_H

#include <stdint.h>
#include "core/types.h"

app_err_t rtc_init(void);

app_err_t rtc_set_time(const rtc_time_t* time);

app_err_t rtc_get_time(rtc_time_t* time);

#endif // RTC_DS1307_H
