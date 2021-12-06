/************************************************
 * Jun Yuseung
 * 20163080
*************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int make_parity(char);
int make_binary_array(int);
int make_2d_array(int);
void sum_array(int);
void wrap_around(void);
void print_checksum(void);

int chekcsum_array[16]; // Global Variable
int* int_array; // Global Variable
short* binary_array; // Global Variable
short** check_array; // Global Variable

int main()
{
    char input[1000];
    int string_length;
    int array_size;
    int row_size;

    scanf("%s", input);
    string_length = strlen(input);
    int_array = (int*)calloc(string_length, sizeof(int));

    for (int i = 0; i < string_length; i++)
    {
        int_array[i] = make_parity(input[i]);
    }

    array_size = make_binary_array(string_length);
    row_size = make_2d_array(array_size);
    sum_array(row_size);
    printf("Checksum is ");
    print_checksum();

    free(binary_array);
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

int make_binary_array(int length)
{
    int size = 1;
    int mask = 0x7F;
    binary_array = (short*)calloc(size, sizeof(short));

    for (int i = 0; i < length; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (int_array[i] >> 7)
            {
                binary_array[size - 1] = 1;
            }
            else
            {
                binary_array[size - 1] = 0;
            }

            int_array[i] &= mask;
            int_array[i] <<= 1;
            size++;
            binary_array = realloc(binary_array, sizeof(short)*size); 
        }
    }
    
    return size - 1;
}

int make_2d_array(int length)
{
    int row_count;
    int remainder;

    if (length % 16)
    {
        row_count = (length / 16) + 1;
        remainder = 1;
    }
    else
    {
        row_count = length / 16;
        remainder = 0;
    }
    
    check_array = (short**)calloc(row_count, sizeof(short*));
    for (int i = 0; i < row_count; i++)
    {
        check_array[i] = (short*)calloc(16, sizeof(short));
    }
    
    if (remainder)
    {
        for (int i = 0; i < row_count - 1; i++)
        {
            for (int j = 0; j < 16; j++)
            {
                check_array[i][j] = binary_array[(16 * i) + j];
            }
        }

        for (int i = 0; i < 8; i++)
        {
            check_array[row_count - 1][i] = binary_array[16 * (row_count - 1) + i];
        }
        
    }
    else
    {
        for (int i = 0; i < row_count; i++)
        {
            for (int j = 0; j < 16; j++)
            {
                check_array[i][j] = binary_array[(16 * i) + j];
            }
        }
    }
    return row_count;
}

void sum_array(int row_length)
{
    int sum = 0;
    int carry = 0;
    for (int i = 0; i < 16; i++)
    {
        chekcsum_array[i] = 0;
    }

    for (int i = 0; i < row_length; i++)
    {
        sum = chekcsum_array[15] ^ check_array[i][15];
        carry = chekcsum_array[15] & check_array[i][15];
        chekcsum_array[15] = sum;

        for (int j = 14; j >= 0; j--)
        {
            sum = carry ^ chekcsum_array[j] ^ check_array[i][j];
            carry = (carry & chekcsum_array[j]) | (carry & check_array[i][j]) | (chekcsum_array[j] & check_array[i][j]);
            chekcsum_array[j] = sum;
        }

        if (carry)
        {
            wrap_around();
        }
    }
}

void wrap_around(void)
{
    int sum;
    int carry;

    sum = chekcsum_array[15] ^ 1;
    carry = chekcsum_array[15] & 1;
    chekcsum_array[15] = sum;

    for (int i = 14; i >= 0; i--)
    {
        sum = chekcsum_array[i] ^ carry;
        carry = chekcsum_array[i] & carry;
        chekcsum_array[i] = sum;
    }

    if (carry)
    {
        wrap_around();
    }
}

void print_checksum(void)
{
    for (int i = 0; i < 16; i++)
    {
        printf("%d", chekcsum_array[i] ^ 1);
    }
}