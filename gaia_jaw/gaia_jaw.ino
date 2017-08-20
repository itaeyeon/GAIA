#include <VarSpeedServo.h>
// https://github.com/netlabtoolkit/VarSpeedServo
// write(value, speed, wait) - wait is a boolean that, if true, causes the function call to block until move is complete

VarSpeedServo  jaw;

#include <MedianFilter.h>
// https://github.com/daPhoosa/MedianFilter
//Minimum window size is 3
//Maximum window size is 255
//Only accepts arduino data type INT
//Input Data: filterObject.in(newValue); (this will return the median value after the new sample has been processed)
//Read Current Value: filterObject.out(); (this allows for reading the current median value without submitting a new sample)

MedianFilter test(5, 0);
#define THRESHOLD  20

int sound;

void setup() {
  Serial.begin(9600);
  jaw.attach(4);  // 턱 닫기 40 열기 10
  jaw.write(60);
  delay(50);
}

void loop() {
//  sound = analogRead(0) - 512;
    test.in(analogRead(0) - 512);
    sound = test.out();

  if ((sound >  THRESHOLD) || (sound <  -1 * THRESHOLD)) {
    jaw.write(10 + random(10));
    delay(100);
    jaw.write(30);
    delay(100);
  } else {
    jaw.write(40, 50, false);
    delay(50);
  }

  //  Serial.println(sound);
}
