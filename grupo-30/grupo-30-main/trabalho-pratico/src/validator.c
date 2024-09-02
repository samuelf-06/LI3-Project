#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#include "../includes/user.h"
#include "../includes/date.h"

// Verifica o estado de uma conta (Active,Inactive,NoStatus) e a sua validação.
enum account_status verify_AccountStatus(char *token) {
    if (strcasecmp(token,"active") == 0)
        return Active;
    else if (strcasecmp(token,"inactive") == 0)
        return Inactive;
    else
        return NoStatus;
}

// Validação de Estrelas do Hotel.
int verify_stars(char *token) {
    int value = atoi(token);
    return (value > 0);
}

// Validação de Preço.
int verify_price(char *token) {
    int price = strtol(token, NULL, 10);
    return (price > 0) ? price : -1;
}


// Verifica se tem pequeno almoço incluído e a sua validação.
char *verify_breakfast(char *token) {
    if(strcasecmp(token, "t") == 0 || strcasecmp(token, "true") == 0 || strcasecmp(token, "1") == 0)
        return "True";
    else if(strcasecmp(token, "f") == 0 || strcasecmp(token, "False") == 0 || strcasecmp(token, "0") == 0 || strcasecmp(token, "") == 0)
        return "False";
    return NULL;    
}

// Validação de Datas.
int verify_date(char *token) {
    DATE result;
    char *tokenCopy = strdup(token);
    char *yearStr = strtok(tokenCopy, "/");
    char *monthStr = strtok(NULL, "/");
    char *dayStr = strtok(NULL, "/");

    if (yearStr == NULL || monthStr == NULL || dayStr == NULL) {
        free(tokenCopy);
        return 0;
    }
    
    if (strlen(yearStr) != 4 || strlen(monthStr) != 2 || strlen(dayStr) != 2) {
        free(tokenCopy);
        return 0;
    }
    free(tokenCopy);
    
    if (sscanf(token, "%d/%02d/%02d", &result.year, &result.month, &result.day) != 3)
        return 0;

    if (result.year < 0 || result.month < 1 || result.month > 12 || result.day < 1 || result.day > 31) 
        return 0;
    
    if (days_from_epoch(result) - days_from_epoch(convertStringToDate(CURRENT_DATE)) > 0)
        return 0;

    int max_days = days_in_month_of_year(result.month, result.year);
    if (result.day < 1 || result.day > max_days) 
        return 0;

    for (int i = 0; token[i] != '\0'; i++) 
        if (!isdigit(token[i]) && token[i] != '/') 
            return 0;
        
    return 1;
}

// Validação de Data com tempo
int verify_date_secs(char *token) {
    DATE_SECS result;
    if (sscanf(token, "%d/%02d/%02d %02d:%02d:%02d", &result.year, &result.month, &result.day,
               &result.hour, &result.minutes, &result.seconds) != 6)
        return 0;

    if (result.year < 0 || result.month < 1 || result.month > 12 ||
        result.day < 1 || result.day > 31 ||
        result.hour < 0 || result.hour > 23 ||
        result.minutes < 0 || result.minutes > 59 ||
        result.seconds < 0 || result.seconds > 59) 
        return 0;
    
    int max_days = days_in_month_of_year(result.month, result.year);
    if (result.day < 1 || result.day > max_days) 
        return 0;

    for (int i = 0; token[i] != '\0'; i++) 
        if (!isdigit(token[i]) && token[i] != '/' && token[i] != ':' && token[i] != ' ') 
            return 0;
        
    return 1;
}

// Validação de Código de País.
char *verify_countryCode(char *token) {
    if (token == NULL || strlen(token) != 2)
        return NULL;
    char *result = strdup(token);
    if (!isdigit(result[0]) && !isdigit(result[1])) 
        return result;
    free(result);
    return NULL;
}

// Validação de Classificação de um Hotel
int verify_rate(char *token) {
    int rate = strtol(token, NULL, 10);
    return (rate >= 1 && rate <= 5) ? rate : -1;
}

// Validação de email.
bool verify_email(char *email) {
    if (email == NULL) return false;

    int emailLength = strlen(email);
    int atCount = 0;
    int dotCount = 0;

    for (int i = 0; i < emailLength; i++) {
        if (email[i] == '@') 
            atCount++;
        else if (email[i] == '.') 
            dotCount++;
    }

    if (atCount == 1 && dotCount > 0) {
        int atIndex = 0;
        while (atIndex < emailLength && email[atIndex] != '@') 
            atIndex++;
        
        if (atIndex < emailLength - 1) 
            return true; 
    }
    return false; 
}

// Validação de Código de Aeroporto
int verify_airport(char *code) {
    size_t len = strlen(code);
    if (len != 3)
        return 0;
    for (size_t i = 0; i < len; i++)
        if (!isalpha((unsigned char)code[i]))
            return 0;
    return 1;  // Código válido
}

int verify_int(char *token) {
    if (token == NULL)
        return 0;
    for (int i = 0; token[i] != '\0'; i++) {
        if (!isdigit(token[i])) {
            return 0; 
        }
    }
    return 1; 
}

// Verifica se é um número inteiro ou um decimal
int isInteger(const char *token) {
    char *endptr; 
    strtol(token, &endptr, 10);  
    return (*endptr == '\0');
}

char *toUppercase(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = toupper((unsigned char)str[i]);
    }
    return str;
}
