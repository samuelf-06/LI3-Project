#ifndef DATE_H
#define DATE_H

#include <stdbool.h>

#define CURRENT_DATE "2023/10/01"

typedef struct data {
    int day;
    int month;
    int year;
} DATE;

typedef struct data_segundos{
    int seconds;
    int minutes;
    int hour;
    int day;
    int month;
    int year;
} DATE_SECS;

DATE convertStringToDate(char* token);

DATE_SECS convertStringToDateSecs(char *token);

DATE DATE_SECS_to_DATE(DATE_SECS date_secs);

int is_leap_year(int year);

int days_in_year(int year);

int days_in_month_of_year(int month, int year);

int days_from_epoch(DATE date);

int secs_from_epoch(DATE_SECS date);

int calculateAge(DATE data);

int calculateDelay(DATE_SECS start, DATE_SECS end);

int compareDates(DATE date1, DATE date2);

int compareDates_Secs(DATE_SECS date1, DATE_SECS date2);

int calculateNightsWithinRange(DATE start_date, DATE end_date, DATE range_start, DATE range_end);

bool dateIsInRange(DATE_SECS date, DATE_SECS begin_date, DATE_SECS end_date);

int getYear(DATE_SECS date);

#endif //DATE_H
