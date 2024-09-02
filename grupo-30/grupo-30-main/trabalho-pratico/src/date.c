#include "../includes/date.h"

#include "stdio.h"
#include <glib.h>
#include <stdbool.h>

// Converte uma string para uma estrutura DATE.
DATE convertStringToDate(char* token) {
    DATE result;
    sscanf(token, "%d/%02d/%02d", &result.year, &result.month, &result.day);
    return result;
}

// Converte uma string para uma estrutura DATE_SECS.
DATE_SECS convertStringToDateSecs(char *token) {
    DATE_SECS result;
    sscanf(token, "%d/%02d/%02d %02d:%02d:%02d", &result.year, &result.month, &result.day,
           &result.hour, &result.minutes, &result.seconds);
    return result;
}

// Converte uma estrutura DATE_SECS para uma estrutura DATE.
DATE DATE_SECS_to_DATE(DATE_SECS date_secs) {
    DATE date;
    date.day = date_secs.day;
    date.month = date_secs.month;
    date.year = date_secs.year;
    return date;
}

// Verifica se um ano é bissexto.
int is_leap_year(int year) {
    return ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0));
}

// Retorna o número de dias em um determinado ano.
int days_in_year(int year) {
    int days = 365;
    if (is_leap_year(year))
        days = 366;
    return days;
}

// Array de dias em cada mês.
int days_in_month[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

// Retorna o número de dias em um mês de um determinado ano.
int days_in_month_of_year(int month, int year) {
    if (month == 2 && is_leap_year(year))
        return 29;
    else
        return days_in_month[month];
}

// Retorna o número de dias desde o início da era UNIX (1970) até uma determinada data (DATE date).
int days_from_epoch(DATE date) {
    int days = 0;
    for (int y = 1970; y < date.year; y++) {
        days += days_in_year(y);
    }
    for (int m = 1; m < date.month; m++) {
        days += days_in_month_of_year(m, date.year);
    }
    days += date.day - 1;
    return days;
}

// Retorna o número de segundos desde o início da era UNIX (1970) até uma determinada data e hora (DATE_SECS date).
int secs_from_epoch(DATE_SECS date){
    if (date.year < 1970 || date.month < 1 || date.month > 12 || date.day < 1 ||
        date.hour < 0 || date.hour >= 24 || date.minutes < 0 || date.minutes >= 60 ||
        date.seconds < 0 || date.seconds >= 60)
        return -1;
    int days = 0;
    for (int y = 1970; y < date.year; y++)
        days += is_leap_year(y) ? 366 : 365;
    for (int m = 1; m < date.month; m++)
        days += days_in_month_of_year(m, date.year);
    days += date.day - 1;
    int seconds = days * 86400;
    seconds += date.hour * 3600 + date.minutes * 60 + date.seconds;
    return seconds;
}

// Calcula a idade com base numa dada determinada data de nascimento.
int calculateAge(DATE data) {
    DATE currentDate = convertStringToDate(CURRENT_DATE);
    int age = currentDate.year - data.year;
    if (currentDate.month < data.month || (currentDate.month == data.month && currentDate.day < data.day))
        age = age - 1;
    return age;
}

// Calcula o atraso do voo.
int calculateDelay(DATE_SECS start, DATE_SECS end) {
    int totalSeconds = (end.year - start.year) * 365 * 24 * 3600 +
                        (end.month - start.month) * 30 * 24 * 3600 +
                        (end.day - start.day) * 24 * 3600 +
                        (end.hour - start.hour) * 3600 +
                        (end.minutes - start.minutes) * 60 +
                        (end.seconds - start.seconds);
    return totalSeconds;
}

// Compara duas datas definidas numa estrutura DATE.
int compareDates(DATE date1, DATE date2) {
    if (date1.year != date2.year)
        return date1.year - date2.year;
    if (date1.month != date2.month)
        return date1.month - date2.month;
    return date1.day - date2.day;
}

// Compara duas datas e horas(em segundos) definidas numa estrutura DATE_SECS.
int compareDates_Secs(DATE_SECS date1, DATE_SECS date2) {
    if (date1.year != date2.year)
        return date1.year - date2.year;
    if (date1.month != date2.month)
        return date1.month - date2.month;
    if (date1.day != date2.day)
        return date1.day - date2.day;
    if (date1.hour != date2.hour)
        return date1.hour - date2.hour;
    if (date1.minutes != date2.minutes)
        return date1.minutes - date2.minutes;
    return date1.seconds - date2.seconds;
}

// Calula o número de noites de um intervalo de datas resultante da interseção de dois intervalos de datas.
int calculateNightsWithinRange(DATE start_date, DATE end_date, DATE range_start, DATE range_end) {
    int nightsWithinRange = 0;

    if (compareDates(end_date, range_start) >= 0 && compareDates(start_date, range_end) <= 0) {
        DATE overlap_start = (compareDates(start_date, range_start) > 0) ? start_date : range_start;
        DATE overlap_end = (compareDates(end_date, range_end) < 0) ? end_date : range_end;
        
        if(range_start.year == range_end.year && range_start.month == range_end.month){
            nightsWithinRange = days_from_epoch(overlap_end) - days_from_epoch(overlap_start) + 1;
        }
        else nightsWithinRange = days_from_epoch(overlap_end) - days_from_epoch(overlap_start);
    }

    return nightsWithinRange;
}

bool dateIsInRange(DATE_SECS date, DATE_SECS begin_date, DATE_SECS end_date) {
    return (secs_from_epoch(date) >= secs_from_epoch(begin_date) &&
            secs_from_epoch(date) <= secs_from_epoch(end_date));
}

int getYear(DATE_SECS date) {
    return date.year;
}
