#ifndef UTILS_H
#define UTILS_H

void itoa(unsigned int num, char* str, int base);
void* memcpy(void* dest, const void* src, unsigned int n);
void* memset(void* dest, int val, unsigned int n);
unsigned int strlen(const char* str);
int strcmp(const char* str1, const char* str2);
void strcpy(char* dest, const char* src);
int strncmp(const char* str1, const char* str2, unsigned int n);


#endif
