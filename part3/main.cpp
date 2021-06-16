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
DigitalInOut ping(D10);
BufferedSerial xbee(D1, D0);

BBCar car(pin5, pin6, servo_ticker);

Thread t1;
Timer t;
EventQueue queue(64 * EVENTS_EVENT_SIZE);
char recvall[30], tmp[20];
int now;
int re = 1;

void Follow();
void RD();
//parallax_ping  ping1(pin10);
int main(){
   xbee.set_baud(9600);
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
            else if (recv[0] != ',') {
                tmp[now++] = recv[0];
            }
      } 
   }
}

void Follow(){
    char n[2][10];
    int tx, ry;
    int i = 0;
    int j = 0;
    int count = 0;
    int dx, dy;
    int deg;
    int len;
    float a = 2.3;
    int turn = 0;
    float r;
    bool stop = false;
    char buff[25];
    float dis, val;
    while(1){
        re = 0;
        //printf("%s\n", recvall);
        i = 0;
        j = 0;
        count = 0;
        while(recvall[i] != ')'){
            if (recvall[i] == ' ') {
                j++;
                count = 0;
                i++;
            }
            n[j][count] = recvall[i];
            count++;
            i++;
        }
        tx = atoi(n[0]);
        ry = atoi(n[1]);
        len = strlen(recvall);
        printf("%d %d %d\n", strlen(recvall), tx, ry);
        re = 1;
        for (i = 0; i < 2; i++) {
            for (j = 0; j < 10; j++) {
                n[i][j] = '\0';
            }
        }
        for (i = 0; i < 30; i++) {
            recvall[i] = '\0';
        
        }

        if (ry > 3 && ry < 20) {
            printf("L and go");
            turn = 1;
            car.goStraight(30); 
        } else if (ry < 357 && ry > 340) {
            printf("R and go");
            turn = 2;
            car.goStraight(30);  
        } else {
            if (len == 0) {
                if (turn == 2){
                    car.turn(30,1);
                    ThisThread::sleep_for(75ms);
                    car.stop();
                } else if (turn == 1) {
                    car.turn(-30,1);
                    ThisThread::sleep_for(75ms);
                    car.stop();
                }
            } else {
                if (tx <= -2){
                    printf("RIGHT\n");
                    car.turn(30,1); 
                    turn = 0;
                } else if (tx > 2) {
                    printf("LEFT\n");
                    car.turn(-30,1);
                    turn = 0;
                } else {
                    car.stop();
                    strcpy(buff, "");
                    sprintf(buff, "angle = %d\r\n", ry);
                    xbee.write(buff, sizeof(buff));
                    ThisThread::sleep_for(75ms);
                }
            }
        }
        ThisThread::sleep_for(100ms);
    }

}