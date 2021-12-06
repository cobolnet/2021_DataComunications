#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int count = 0; // for number of address
char string[18]; // delete command I or S at the front of buffer
char string2[15];
char array[14]; // delete '.', '/' and fix length to 14 by fill 0's
char array2[12];
char** table;

void input (void); // alloc table
void search (void);
void fix_length (void); // delete '.', '/' and fix length to 14 by fill 0's
void fix_length_s (void);

int main (void)
{
    char buffer[100];
    table = (char**)calloc(1, sizeof(char));
    table[count] = (char*)calloc(14, sizeof(char)); // make first list
    while(1)
    {
        scanf("%[^\n]s", buffer);
        scanf("%*c"); // flush scanf's buffer

        if (buffer[0] == 'I')
        {
            for (int i = 0; i < 18; i++)
                string[i] = buffer[i + 2];
            
            fix_length();
            input();
        }
        else if (buffer[0] == 'S')
        {
            for (int i = 0; i < 15; i++)
            {
                string2[i] = buffer[i + 2];
            }
            fix_length_s();
            search();
            break;
        }
        else
            continue;
    }
    
    return 0;
}

void input (void)
{
    // copy
    for (int i = 0; i < 14; i++)
    {
        table[count][i] = array[i];
    }
    count++;
    // realloc
    realloc(table, count + 1);
    table[count] = (char*)calloc(14, sizeof(char));
}

void search (void)
{
    int* mask_info;
    int* cal_info;
    int** matching_table;
    int int_array[4];
    char temp_array[4];
    int bi_array[32];

    // char to int
    temp_array[3] = '\0';
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            temp_array[j] = array2[(i * 3) + j];
        }
        int_array[i] = atoi(temp_array);
    }
    
    // int to binary
    int k;
    for (int i = 0; i < 4; i++)
    {
        k = 0;
        for (int j = 7; j >= 0; j--)
        {
            bi_array[(i * 8) + k] = (int_array[i] >> j) & 1;
            k++;
        }
    }

    // malloc binary array
    matching_table = (int**)calloc(count, sizeof(int));
    for (int i = 0; i < count; i++)
    {
        matching_table[i] = (int*)calloc(32, sizeof(int));
    }
    
    // all array to binary
    int l;
    for (int i = 0; i < count; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            for (int k = 0; k < 3; k++)
            {
                temp_array[k] = table[i][(j * 3) + k];
            }
            int_array[j] = atoi(temp_array);
        }
        
        for (int j = 0; j < 4; j++)
        {
            l = 0;
            for (int k = 7; k >= 0; k--)
            {
                matching_table[i][(j * 8) + l] = (int_array[j] >> k) & 1;
                l++;
            }
        }
    }

    // all array's mask to int
    mask_info = (int*)calloc(count, sizeof(int));
    char char_mask[3];
    char_mask[2] = '\0';
    for (int i = 0; i < count; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            char_mask[j] = table[i][j + 12];
        }
        mask_info[i] = atoi(char_mask);
    }
    
    // cal prefix
    cal_info = (int*)calloc(count, sizeof(int));
    for (int i = 0; i < count; i++)
    {
        for (int j = 0; j < mask_info[i]; j++)
        {
            if (matching_table[i][j] == bi_array[j])
            {
                cal_info[i]++;
            }
            else
                break;
        }
    }
    
    // compare longest prefix
    int prefix = -1;
    int temp = 0;
    for (int i = 0; i < count; i++)
    {
        if (cal_info[i] > temp)
        {
            temp = cal_info[i];
            prefix = i;
        }
    }
    
    // print port
    if (prefix == -1)
    {
        printf("%d\n", count);
    }
    else
    {
        printf("%d\n", prefix);
    }
}

void fix_length (void)
{
    char temp[3];

    int string_i = 0;
    int array_i = 0;
    int temp_i = 0;

    while (array_i < 14)
    {
        if ((string[string_i] != '.') && (string[string_i] != '/'))
        {
            temp[temp_i] = string[string_i];
            string_i++;
            temp_i++;
        }
        else if (string[string_i] == '.')
        {
            for (int i = 3 - temp_i; i > 0; i--)
            {
                array[array_i] = '0';
                array_i++;
            }

            for (int i = 0; i < temp_i; i++)
            {
                array[array_i] = temp[i];
                array_i++;
            }

            string_i++;
            temp_i = 0;
        }
        else if (string[string_i] == '/')
        {
            for (int i = 3 - temp_i; i > 0; i--)
            {
                array[array_i] = '0';
                array_i++;
            }

            for (int i = 0; i < temp_i; i++)
            {
                array[array_i] = temp[i];
                array_i++;
            }

            string_i++;

            for (int i = 0; i < 2; i++)
            {
                array[array_i] = string[string_i];
                array_i++;
                string_i++;
            }
            
        }
    }
}

void fix_length_s (void)
{
    char temp[3];
    int count = 0;
    int string_i = 0;
    int array_i = 0;
    int temp_i = 0;

    for (int i = 0; i < 12; i++)
    {
        array2[i] = 0;
    }
    

    while (count < 3)
    {    
        if (string2[string_i] != '.')
        {
            temp[temp_i] = string2[string_i];
            string_i++;
            temp_i++;
        }
        else if (string2[string_i] == '.')
        {
            for (int i = 3 - temp_i; i > 0; i--)
            {
                array2[array_i] = '0';
                array_i++;
            }

            for (int i = 0; i < temp_i; i++)
            {
                array2[array_i] = temp[i];
                array_i++;
            }

            string_i++;
            temp_i = 0;
            count++;
        }        
    }

    int len = 0;
    len = strlen(string2);
    
    for (int i = 3 - (len - string_i); i > 0; i--)
    {
        array2[array_i] = '0';
        array_i++;
    }

    for (int i = array_i; i < 12; i++)
    {
        array2[array_i] = string2[string_i];
        array_i++;
        string_i++;
    }
}