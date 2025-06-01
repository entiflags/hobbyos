#include "Utils.h"

void itoa(unsigned int num, char* str, int base)
{
    int i = 0;
    if (num == 0)
    {
        str[i++] = '0';
        str[i] = '\0';
        return;
    }

    while (num != 0)
    {
        int rem = num % base;
        str[i++] = (rem > 9) ? (rem - 10) + 'a' : rem + '0';
        num = num / base;
    }

    str[i] = '\0';

    int start = 0;
    int end = i - 1;
    while (start < end)
    {
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        start++;
        end--;
    }
}

void* memcpy(void* dest, const void* str, unsigned int n)
{
    char* d = (char*)dest;
    const char* s = (const char*)str;
    for (unsigned int i = 0; i < n; i++)
        d[i] = s[i];
    return dest;
}

void* memset(void* dest, int val, unsigned int n)
{
    char* d = (char*)dest;
    for (unsigned int i = 0; i < n; i++)
        d[i] = (char)val;
    return dest;
}

unsigned int strlen(const char* str)
{
    unsigned int len = 0;
    while (str[len] != '\0')
        len++;
    return len;
}

int strcmp(const char* str1, const char* str2)
{
    while (*str1 && (*str1 == *str2))
    {
        str1++;
        str2++;
    }
    return *(unsigned char*)str1 - *(unsigned char*)str2;
}

void strcpy(char* dest, const char* src)
{
    while (*src != '\0')
        *dest++ = *src++;
    *dest = '\0';
}

int strncmp(const char* str1, const char* str2, unsigned int n)
{
    for (unsigned int i = 0; i < n; i++)
    {
        if (str1[i] == '\0' || str1[i] != str2[i])
            return *(unsigned char*)&str1[i] - *(unsigned char*)&str2[i];
    }
    return 0;
}