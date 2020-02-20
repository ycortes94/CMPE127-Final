#include "msp.h"
#include "..\inc\bump.h"
#include "..\inc\Clock.h"
#include "..\inc\SysTick.h"
#include "..\inc\CortexM.h"
#include "..\inc\LaunchPad.h"
#include "..\inc\Motor.h"
#include "..\inc\TimerA1.h"
#include "..\inc\TExaS.h"
#include "..\inc\Reflectance.h"

uint8_t Data; // QTR-8RC
int32_t pos;
void Move_Back(void){
    uint8_t bump=Bump_Read();
    if (bump != 0x3F) {
        Motor_Stop();
        Clock_Delay1ms(1000);
        Motor_Backward(2400, 2400);
        Clock_Delay1ms(1000);
        Motor_Right(1600, 1600);
        Clock_Delay1ms(1000);
        Motor_Stop();
        Clock_Delay1ms(1000);
        Motor_Forward(1400, 1400);
        Clock_Delay1ms(1000);

        for(int i = 0; i < 4; i++) {
            Data = Reflectance_Read(1000);
            pos += Reflectance_Position(Data);
        }
        pos = pos/4;

    }

}
uint8_t cnt = 0;
int32_t buff[4];
int main(void){

    Clock_Init48MHz();
    LaunchPad_Init(); // built-in switches and LEDs
    Bump_Init();      // bump switches
    Motor_Init();     // your function
    while(LaunchPad_Input()==0);  // wait for touch
    while(LaunchPad_Input());     // wait for release
    TimerA1_Init(&Move_Back, 50000);
    EnableInterrupts();


    while(1)
    {
        pos = 0;
        Motor_Forward(5600, 5600);
        Clock_Delay1ms(500);
        for(int i = 0; i < 4; i++) {
            Data = Reflectance_Read(1000);
            pos += Reflectance_Position(Data);
        }
        pos = pos/4;

        if(pos < -6)
        {
            if(pos < -140) {
                Motor_Left(2000,2000);
                Clock_Delay1ms(1000);
            }
            else
            {
                Motor_Left(1600,1600);
                Clock_Delay1ms(1000);
            }

        }
        else if(pos > 6)
        {
            if(pos > 140) {
                Motor_Right(2000,2000);
                Clock_Delay1ms(1000);
            }
            else
            {
                Motor_Right(1600,1600);
                Clock_Delay1ms(1000);
            }

        }
        else
        {
            Motor_Forward(5600, 5600);
            Clock_Delay1ms(500);
        }

        if (Data == 0) {
            cnt += 1;
            Motor_Stop();
            Clock_Delay1ms(1000);
            Motor_Backward(3800, 3800);
            Clock_Delay1ms(1000);
            if(cnt == 10)
            {
                Motor_Stop();
                Clock_Delay1ms(1000);
                Motor_Left(1600,1600);
                Clock_Delay1ms(1000);
                Motor_Forward(1200, 1200);
                Clock_Delay1ms(1000);
                cnt = 0;
            }
        }
        else if (Data == 255)
        {
            while(1)
            {
                Motor_Stop();
            }
        }
    }
}



