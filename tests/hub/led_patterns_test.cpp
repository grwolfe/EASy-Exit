#include "mbed.h"

DigitalOut outputs[20] =
{
    p11, p12, p13, p14, p15, p16, p17, p18, p19, p20,
    p21, p22, p23, p24, p25, p26, p27, p28, p29, p30
};

void pattern(uint8_t pat)
{
    switch (pat)
    {
    case 0:
        for (size_t i = 0; i < 20; i++)
        {
            outputs[i] = !outputs[i];
            wait(0.05);
        }
        break;
        
    case 1:
        outputs[0] = 1;
        for (size_t i = 1; i < 20; i++)
        {
            wait(0.05);
            outputs[i - 1] = !outputs[i - 1];
            outputs[i] = !outputs[i];
        }
        wait(0.05);
        outputs[19] = 0;
        break;

    case 2:
        for (size_t i = 0; i < 10; i++) {
            outputs[i] = !outputs[i];
            outputs[i + 10] = !outputs[i + 10];
            wait(0.05);
        }

        for (size_t i = 19; i > 9; i--) {
            outputs[i] = !outputs[i];
            outputs[i - 10] = !outputs[i - 10];
            wait(0.05);
        }
        break;
        
    case 3:
        for (size_t i = 0; i < 2; i++) {
            for (size_t i = 0; i < 10; i++) {
                outputs[i] = !outputs[i];
                outputs[19 - i] = !outputs[19 - i];
                wait(0.05);
            }
        }

        for (size_t i = 0; i < 2; i++) {
            for (size_t i = 0; i < 10; i++) {
                outputs[10 + i] = !outputs[10 + i];
                outputs[10 - i - 1] = !outputs[10 - i - 1];
                wait(0.05);
            }
        }
        break;
    }
    
}

int main(void)
{
    while(1)
    {
        for (size_t i = 0; i < 2; i++) {
            pattern(0);
        }

        wait(0.1);
        
        for (size_t i = 0; i < 2; i++) {
            pattern(1);            
        }

        wait(0.1);

        pattern(3);
        
        wait(0.1);
    }
}