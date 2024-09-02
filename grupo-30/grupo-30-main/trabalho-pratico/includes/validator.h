#ifndef VALIDATOR_H
#define VALIDATOR_H

enum account_status verify_AccountStatus (char* token);

int verify_stars(char *token);

int verify_price(char *token);

char *verify_breakfast (char *token);

int verify_date(char *token);

int verify_date_secs(char*token);

char *verify_countryCode(char *token);

int verify_rate (char *token);

int verify_email(char *token);

int verify_airport(char *code);

int verify_int(char *token);

int isInteger(const char *token);

char *toUppercase(char *str);

#endif //VALIDATOR_H
