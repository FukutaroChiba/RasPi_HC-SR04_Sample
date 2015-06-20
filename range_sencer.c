#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>
#include <wiringPi.h>
#include "ibeacon.h"

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

// カメラで撮影
void capture(void) {
  struct tm *strtim;
  time_t timer;
  time(&timer); // 現在時刻取得
  strtim = localtime(&timer); // 構造体に詰める
  char cmd[200];
  sprintf(cmd, "fswebcam --line-colour '#FF000000' --banner-colour '#FF000000'  -d /dev/video0 -S 25 -r 640x480 --png 0 /home/pi/Desktop/img_%d:%d:%d.png", strtim->tm_hour, strtim->tm_min, strtim->tm_sec);
  system(cmd);
}

int main(void) {

  int advInterval = 2;
  char uuid[] = "FC1FC7ED76874C08B15A1004F0728427";
  int major = 0;
  int minor = 0;
  int rssi = -58;

  int rc = enable_advertising(advInterval, uuid, major, minor, rssi); 
  return 1;
  printf("%s\n", "start");
  if(wiringPiSetupGpio() == -1) return 1;

  while(1) {
    double distance = getDistance(TRIG, ECHO);
    printf("%fcm\n", distance);
    sleep(1);

    if(distance < 20) {
      capture();
      sleep(1);
      return 0;
    }
  }
}
