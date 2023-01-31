
#include "mbed.h"

#define MAXIMUM_BUFFER_SIZE
using namespace std;

Thread thread1;
Thread thread2;

UnbufferedSerial pc(USBTX, USBRX);
UnbufferedSerial  dev(D8, D2);

int dev_RxLen=0;
int pc_RxLen=0;
  
 char dev_RxBuf[512] = {0};
char pc_RxBuf[512] = {0};
  char buf4[64] = {0};
  
// обработчики прерываний по приему байта с устройства и с компа- просто заполняют буферы свои- при достижении 64 байта в буфере -начинают заполнять снова с нуля

void dev_recv()
{
    dev.read(&dev_RxBuf[dev_RxLen], sizeof(dev_RxBuf[dev_RxLen]));    //  Got 1 char    
    dev_RxLen++;
}


 
void pc_recv()
{
    pc.read(&pc_RxBuf[pc_RxLen], sizeof(pc_RxBuf[pc_RxLen]));  //  Got 1 char
    pc_RxLen++;
}

void MOST1()
{
    while(1)
    {
     //послать на комп содержимое буфера -принятое от вайфаймодуля  по прерыванию
    //  (распечатать на консоль то что пришло  с wifi модуля)
if (dev_RxLen>0)
{
for (uint8_t i = 0; i < dev_RxLen; i++) 
{
pc.write(&dev_RxBuf[i], sizeof(dev_RxBuf[i]));
}
dev_RxLen=0;
}

ThisThread::sleep_for(1s);
}
}
void MOST2()
{
    while(1)
    {
     
// послать на wifi модуль содержимое буфера -принятое от компа по прерыванию
if (pc_RxLen>0)
{
for (uint8_t i = 0; i < pc_RxLen; i++) 
{
dev.write(&pc_RxBuf[i], sizeof(pc_RxBuf[i]));
}
pc_RxLen=0;
}
ThisThread::sleep_for(1s);
}
}

int main()
{
    pc.attach(&pc_recv, UnbufferedSerial::RxIrq);
    dev.attach(&dev_recv, UnbufferedSerial::RxIrq);
        
    pc.baud(115200);
    dev.baud(115200);

    sprintf(buf4,"AT+GMR\r\n"); 
  for (uint8_t i = 0; i < 8; i++) {  
    dev.write(&buf4[i], sizeof(buf4[i]));
//распечатать на консоль то что послано на  модуль
    pc.write(&buf4[i], sizeof(buf4[i]));
        }  

   // запуск 2 потоков пересылающего содержимое обоих приемных буферов и сон 1 сек
    thread1.start(MOST1);
    thread2.start(MOST2);

//фориат передачи -по умолчанию - поэтому не нужно
  //     pc.format(8, BufferedSerial::None,  1    );
   //     dev.format(8, BufferedSerial::None, 1    );

    while(1) { 
       sleep(); 
            }      
    }
    



     