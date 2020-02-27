#include<stdio.h>
#include<stdint.h>
#include<fcntl.h>
#include<unistd.h>

/*Macros*/
#define packet_buffer_size  (uint8_t)24
#define One_byte             8

typedef struct Packet_t
{
    uint64_t SRC_ADDR ;
    uint64_t DST_ADDR ;
    uint32_t KEY ;
    int VALUE ;

}Packet_t;

/*Serialize function*/
uint8_t* serialize(Packet_t Packet)
{
    uint8_t buff_Index=0;
    static uint8_t packet_buff[packet_buffer_size];
    /* SRC_ADDR serialize */
    buff_Index=7;
    uint64_t SRC_ADDR = Packet.SRC_ADDR;
    for(int i=0; i< 8 ;i++) // loop for shifting  8byte SRC_ADDR to extract One_Byte at a time
    {
        if(i!=0)
        {
            SRC_ADDR=(SRC_ADDR >> One_byte);
        }
        uint8_t temp= (uint8_t)(SRC_ADDR); //Extract One Byte and store in buffer
        packet_buff[buff_Index]=temp;
        buff_Index--;
    }

    /* DST_ADDR serialize */
    buff_Index=15;
    uint64_t DST_ADDR = Packet.DST_ADDR;
    for(int i=0; i< 8 ;i++)
    {
        if(i!=0)
        {
            DST_ADDR=(DST_ADDR >> One_byte);
        }
        uint8_t temp= (uint8_t)(DST_ADDR);
        packet_buff[buff_Index]=temp;
        buff_Index--;
    }

    /* KEY serialize */
    buff_Index=19;
    uint64_t KEY = Packet.KEY;
    for(int i=0; i< 4 ;i++)
    {
        if(i!=0)
        {
            KEY=(KEY >> One_byte);
        }
        uint8_t temp= (uint8_t)(KEY);
        packet_buff[buff_Index]=temp;
        buff_Index--;
    }
    /* VALUE serialize */
    buff_Index=23;
    uint64_t VALUE = Packet.VALUE;
    for(int i=0; i< 4 ;i++)
    {
        if(i!=0)
        {
            VALUE=(VALUE >> One_byte);
        }
        uint8_t temp= (uint8_t)(VALUE);
        packet_buff[buff_Index]=temp;
        buff_Index--;
    }

    return packet_buff;
}

/*De-serialize function*/
Packet_t deserialize(uint8_t* packet_buff)
{

    Packet_t Packet;
    int temp;
    uint32_t tempKey;
    uint64_t tempAddr;
    uint8_t buff_Index=0;
    /*De-serialize Value*/
    int value= 0;
    buff_Index=23;
    for(int i=0; i< 4 ;i++)
    {
        temp=((int)((packet_buff[buff_Index])) << One_byte*i); //shift the buffer value by One_Byte*i  positions to the left
        value |= temp; //stitch the shifted value to the value variable
        temp=0;
        buff_Index--;
    }
    Packet.VALUE=value;

    /*De-serialize Key*/
    uint32_t Key= 0;
    buff_Index=19;
    for(int i=0; i< 4 ;i++)
    {
        tempKey=((uint32_t)((packet_buff[buff_Index])) << One_byte*i);
        Key |= tempKey;
        tempKey=0;
        buff_Index--;
    }
    Packet.KEY=Key;

    /*De-serialize DST_ADDR*/
    uint64_t DST_ADDR= 0;
    buff_Index=15;
    for(int i=0; i< 8 ;i++)
    {
        tempAddr=((uint64_t)((packet_buff[buff_Index])) << One_byte*i);
        DST_ADDR |= tempAddr;
        tempAddr=0;
        buff_Index--;
    }
    Packet.DST_ADDR=DST_ADDR;

    /*De-serialize C*/
    uint64_t SRC_ADDR= 0;
    buff_Index=7;
    for(int i=0; i< 8 ;i++)
    {
        tempAddr=((uint64_t)((packet_buff[buff_Index])) << One_byte*i);
        SRC_ADDR |= tempAddr;
        tempAddr=0;
        buff_Index--;
    }
    Packet.SRC_ADDR=SRC_ADDR;

return Packet;
}

int main(int argc, char *argv[])
{
    int check=0;
    uint8_t input_pckt_buff[100];
    if(argc==1)
    {
        printf("No file found");
        return 0;
    }
    FILE* file=fopen(argv[1],"r");
    if(file == NULL)
    {
        printf("Error opening file\n");
        return 1;
    }
    int i=0;
    while(fscanf(file, "%d,", &input_pckt_buff[i])!=EOF)
    {

        printf("%d,",input_pckt_buff[i]);
        if(i == (packet_buffer_size-1))
        {
            Packet_t Packet_out=deserialize(input_pckt_buff);
            printf("\n");
            printf("SRC_ADDR=%llu,DST_ADDR=%llu,KEY=%lu,VALUE=%d\n",Packet_out.SRC_ADDR,Packet_out.DST_ADDR,Packet_out.KEY,Packet_out.VALUE);
            /*Clear buffer*/
            for(int k=0;k < 100; k++ )
            {
                input_pckt_buff[k]=0;
            }
            i=0;
        }
        else
        {
          i++;
        }
    }

    fclose(file);


    return 0;
}
