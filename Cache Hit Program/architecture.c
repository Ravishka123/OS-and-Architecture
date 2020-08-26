 #include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>

#define CACHE_SIZE 4096
#define BLOCK_SIZE 64

void processor(char file_name[]);
uint32_t get_bit_address(char address[]);

int main()
{
    char file_path[] = "C://Users//shema//OneDrive//Desktop//spice.txt";
    processor(file_path);
    return 0;
}

void processor(char file_name[])
{
    FILE *fp;
    char ch = 'a';

    int cache_lines = CACHE_SIZE/BLOCK_SIZE;
    int cache_address_len = log2(cache_lines);
    int offset_len = log2(BLOCK_SIZE);
    int tag_size = 32 - offset_len - cache_address_len;
    unsigned int data_cache[cache_lines];
    int data_cache_flag[cache_lines];

    int i = 0;
    for (i; i<cache_lines;i+=1)
    {
        data_cache_flag[i] = -1;
    }
    fp = fopen(file_name, "r");
    int total = 0;
    int hits = 0;
    uint32_t bit_address;
    uint32_t cache_addr;
    uint32_t tag;
       if (fp == NULL)
    {
        perror("File name is not valid!");
        exit(EXIT_FAILURE);
    }
    else
    {
        while (ch != EOF)
        {
            ch =fgetc(fp);
            char flag = ch;
           //ch = fgetc(fp);
            int i = 0;
            char address[8]={};
            ch = fgetc(fp);
            while (ch != '\n' && ch != EOF)
            {
                address[i] = ch;
                i = i + 1;
                ch = fgetc(fp);
               
            }
            bit_address = get_bit_address(address);
           cache_addr = bit_address >> offset_len;
            //cache_addr = cache_addr & 31;
           cache_addr = cache_addr << (32-cache_address_len);
           cache_addr = cache_addr >> (32-cache_address_len);
            tag = bit_address >> (32-tag_size);
       
            if (data_cache_flag[cache_addr] == cache_addr && data_cache[cache_addr] == tag)
            {
                hits += 1;
            }
            else
            {
                data_cache[cache_addr] = tag;
                data_cache_flag[cache_addr] = cache_addr;
            }
            total += 1;
        }
        fclose(fp);
        printf("hits ratio = %.6f",(float)hits);
    }
}

uint32_t get_bit_address(char address[])
{
    uint32_t bit_address;
    bit_address = strtol(address, 0, 16);
    return bit_address;
}