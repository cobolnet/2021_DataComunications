/************************************************
 * Jun Yuseung
 * 20163080
*************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int make_parity(char);
int make_crcarray(int); 
int print_remainder(int);
void print_binary(int);

short* crc_array; // Global variable
int* int_array; // Global Variable

int main()
{
    char input[1000];
    int string_length;
    int array_size;
    int remainder;

    scanf("%s", input);
    string_length = strlen(input);
    int_array = (int*)calloc(string_length, sizeof(int));

    for (int i = 0; i < string_length; i++)
    {
        int_array[i] = make_parity(input[i]);
    }

    array_size = make_crcarray(string_length);
    remainder = print_remainder(array_size);

    printf("Decimal is %d\n", remainder);
    printf("Binary is ");
    print_binary(remainder);

    free(int_array);
    free(crc_array);
    return 0;
}

int make_parity(char ascii)
{
    int i_ascii = ascii;
    int count = 0;

    // Count 1 bits
    while (ascii)
    {
        count += ascii % 2;
        ascii /= 2;
    }

    // Left shit 1 bit and add parity bit
    if (count % 2 == 0)
    {
        i_ascii <<= 1;
        i_ascii |= 0;
    }
    else
    {
        i_ascii <<= 1;
        i_ascii |= 1;
    }
    
    return i_ascii;
}

int make_crcarray(int length)
{
    int size = 1;
    int mask = 0x7F;
    crc_array = (short*)calloc(size, sizeof(short));

    for (int i = 0; i < length; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (int_array[i] >> 7)
            {
                crc_array[size - 1] = 1;
            }
            else
            {
                crc_array[size - 1] = 0;
            }

            int_array[i] &= mask;
            int_array[i] <<= 1;
            size++;
            crc_array = realloc(crc_array, sizeof(short)*size); 
        }
    }
    
    return size - 1;
}

int print_remainder(int length)
{
    unsigned short generator = 0x8005;
    unsigned short remainder = 0x0000;
    int top_bit;

    // CRC operation
    for (int i = 0; i < length; i++)
    {
        top_bit = remainder >> 15;
        if (top_bit ^ crc_array[i])
        {
            remainder = (remainder << 1) ^ generator;
        }
        else
        {
            remainder <<= 1;
        }
    }

    return remainder;
}

void print_binary(int remainder)
{
    int mask = 0x7FFF;
    while (remainder)
    {
        printf("%d", remainder >> 15);
        remainder &= mask;
        remainder <<= 1;
    }
}