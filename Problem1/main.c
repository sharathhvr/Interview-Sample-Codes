#include <stdio.h>
#include <stdlib.h>
#include<stdint.h>
#include<string.h>

/*Macros */
#define OneByte    8


typedef struct MotorMsg_t
{
    float forward_back;
    float left_right;

}MotorMsg_t;


void display_message(char* disply_msg )
{
    printf("Message:");
    for(int i=0; i< strlen(disply_msg);i++)
    {
        printf("%c", disply_msg[i]);
    }
    printf("\n");
}

void update_motor( MotorMsg_t motor_message )
{

    printf("Forward_back: 0x%X\nRight_Left: 0x%X\n",(int)motor_message.forward_back,(int)motor_message.left_right);//typecasting into integer to display hexa-decimal value

}

void messageDispatch(uint8_t* packet)
{
    int j=0;
    uint32_t forward_back=0;
    uint32_t left_right=0;
    uint16_t Message_Length=0;
    static MotorMsg_t motor_msg;

    /*Extract message length from packet*/
    Message_Length |= packet[2];
    Message_Length |= (packet[3]<< OneByte);
    printf("Message Length:%d bytes\n",Message_Length);

    static char display_msg[100];

    if(packet[1]== 0x34)// Display Packet
    {
        printf("Its a Display packet\n");
        /*Extract Message from packet*/
        j=0;
        for(int i=4; i < Message_Length+4; i++)
        {
            display_msg[j]=(char)packet[i];
            j++;
        }
        j=0;
    /* call Display Message function */
    display_message(display_msg);

    }
    else if(packet[1]== 0x80) //Motor packet
    {
        printf("Its a Motor packet\n");
        /*Extract Message forward_back from packet*/
        j=0;
        for(int i=4; i < (Message_Length/2)+4; i++)
        {
            forward_back |= (packet[i] << OneByte*j);
            j++;
        }

        /*Extract Message left_right from packet*/
        j=0;
        for(int i=8; i < (Message_Length/2)+8; i++)
        {
            left_right |= (packet[i] << OneByte*j);
            j++;
        }
        j=0;

        motor_msg.forward_back=(float)forward_back;
        motor_msg.left_right=(float)left_right;
    /* Call Update Motor function */
     update_motor(motor_msg);

    }

}

int main()
{
    //Send display packet
    printf("Test packet1:\n");
    uint8_t display_packet[] = {0x1, 0x34, 0x05, 0x0, 0x48, 0x65, 0x6c, 0x6c, 0x6f};
    messageDispatch(display_packet);
    //Send Motor packet
    printf("\nTest packet2:\n");
    uint8_t motor_packet[] = {0x2, 0x80, 0x08, 0x0, 0x0, 0x0, 0x80, 0x3f, 0x0, 0x0, 0x0, 0xbf};
    messageDispatch(motor_packet);
    return 0;
}
