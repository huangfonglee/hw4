#include"mbed.h"
#include "bbcar.h"
#include "bbcar_rpc.h"
#include "stdlib.h"
#include "string.h"
#include <math.h>
PwmOut pin5(D5), pin6(D6);
Ticker servo_ticker;
BufferedSerial pc(USBTX,USBRX); //tx,rx
BufferedSerial uart(A1,A0); //tx,rx

BBCar car(pin5, pin6, servo_ticker);

Thread t1;
EventQueue queue(32 * EVENTS_EVENT_SIZE);

char recvall[20], tmp[20];
int now;
int re = 1;

void Follow();

int main(){
   char recv[1];
   uart.set_baud(9600);
   t1.start(callback(&queue, &EventQueue::dispatch_forever));
   queue.call(Follow);
   while(1){
      if(uart.readable()){
            uart.read(recv, sizeof(recv));
            //pc.write(recv, sizeof(recv));
            if(recv[0] == '(') now = 0;
            else if(recv[0] == ')') {
                tmp[now] = recv[0];
                if(re) strcpy(recvall, tmp);
                strcpy(tmp, "");
            }
            else if (recv[0] != ','){
                tmp[now++] = recv[0];
            }
      } 
   }
}

void Follow(){
    char nx1[4], nx2[4], ny1[4], ny2[4];
    int x1, x2, y1, y2;
    int i = 0;
    int j = 0;
    int count = 0;
    int dx, dy;

    while(1) {
        re = 0;
        printf("%s\n", recvall);
        i = 0;
        count = 0;
        while(recvall[count] != ' '){
            nx1[i] = recvall[count];
            i++;
            count++;
        }
        i = 0;
        count++;
        while(recvall[count] != ' '){
            ny1[i] = recvall[count];
            i++;
            count++;
        }
        i = 0;
        count++;
        while(recvall[count] != ' '){
            nx2[i] = recvall[count];
            i++;
            count++;
        }
        i = 0;
        count++;
        while(recvall[count] != ')'){
            ny2[i] = recvall[count];
            i++;
            count++;
        }
        i = 0;
        
        x1 = atoi(nx1);
        y1 = atoi(ny1);
        x2 = atoi(nx2);
        y2 = atoi(ny2);
        printf("%d, %d, %d. %d\n", x1, y1, x2, y2);
        if (y1 > y2) {
            int temp;
            temp = x1;
            x1 = x2;
            x2 = temp;
            temp = y1;
            y1 = y2;
            y2 = temp;
        }
        //printf("%d %d %d %d\n", x1, y1, x2, y2);
        
        dx = x1 - x2;
        dy = y1 - y2;
        
        if (abs(dx) + abs(dy) > 0) {
            if (x1 < 70) {
                car.turn(30, 1);
                printf("Right\n");
                ThisThread::sleep_for(50ms);
                car.stop();
            }
            else if (x1 > 90) {
                car.turn(-30, 1);
                printf("LEFT\n");
                ThisThread::sleep_for(50ms);
                car.stop();
            }
            else {
                car.goStraight(35);
            }
        }
        else{
            car.stop();
        }

        for (i = 0; i < 4; i++) {
            nx1[i] = '\0';
            ny1[i] = '\0';
            nx2[i] = '\0';
            ny2[i] = '\0';
        }
        re = 1;
        ThisThread::sleep_for(50ms);
    }

}