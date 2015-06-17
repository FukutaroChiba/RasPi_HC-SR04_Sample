#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>
#include <wiringPi.h>

#define ECHO 2
#define TRIG 3

double getDistance(int trig, int echo) {
  // GPIO settings
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  // triger
  digitalWrite(TRIG, 1);
  usleep(1);
  digitalWrite(TRIG, 0);
  // time
  struct timeval off, on;

  while (digitalRead(ECHO) == 0) {
    gettimeofday(&off, NULL);
  }
  while (digitalRead(ECHO) == 1) {
    gettimeofday(&on, NULL);
  }
  // 
  double behind = (on.tv_sec - off.tv_sec) + (on.tv_usec - off.tv_usec)*1.0E-6;
  return behind * 17000;
}

int main(void) {

  printf("%s\n", "start");
  if(wiringPiSetupGpio() == -1) return 1;


  while(1) {
    double distance = getDistance(TRIG, ECHO);
    printf("%fcm\n", distance);
    sleep(1);
  }
}
