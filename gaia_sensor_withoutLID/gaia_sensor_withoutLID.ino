//#include <Servo.h>
#include <VarSpeedServo.h>
// https://github.com/netlabtoolkit/VarSpeedServo

#include <NewPing.h>
// http://playground.arduino.cc/Code/NewPing
//Methods
//sonar.ping(); - Send a ping, returns the echo time in microseconds or 0 (zero) if no ping echo within set distance limit
//sonar.ping_in(); - Send a ping, returns the distance in inches or 0 (zero) if no ping echo within set distance limit
//sonar.ping_cm(); - Send a ping, returns the distance in centimeters or 0 (zero) if no ping echo within set distance limit
//sonar.ping_median(iterations); - Do multiple pings (default=5), discard out of range pings and return median in microseconds
//sonar.convert_in(echoTime); - Converts microseconds to distance in inches = (microseconds / 74 / 2)
//sonar.convert_cm(echoTime); - Converts microseconds to distance in centimeters = (microseconds / 29 / 2)
//sonar.ping_timer(function); - Send a ping and call function to test if ping is complete.
//sonar.check_timer(); - Check if ping has returned within the set distance limit.
//timer_us(frequency, function); - Call function every frequency microseconds.
//timer_ms(frequency, function); - Call function every frequency milliseconds.
//timer_stop(); - Stop the timer.

#include <MedianFilter.h>
// https://github.com/daPhoosa/MedianFilter
//Minimum window size is 3
//Maximum window size is 255
//Only accepts arduino data type INT
//Input Data: filterObject.in(newValue); (this will return the median value after the new sample has been processed)
//Read Current Value: filterObject.out(); (this allows for reading the current median value without submitting a new sample)

MedianFilter test(5, 0);

#define SONAR_NUM     5 // 초음파 센서 개수
#define MAX_DISTANCE 500 // 최대 거리 (대기 시간)

int THRESHOLD = 40; // 초음파 센서 인식 거리

unsigned int cm[SONAR_NUM];         // 초음파 센서 결과 값 (거리)
int POT;
int  detected_num = 0; // 현재 가장 가까이 인식된 초음파 센서 번호
int detected_num_old = 0;
int  detected_value = 100;
int detected_level = 0;

unsigned long prev_time = 0;
unsigned long prev_time2 = 0;
unsigned long prev_time3 = 0;
unsigned long prev_time4 = 0;
unsigned long current_time = 0;

NewPing sonar1(4, 4, MAX_DISTANCE); // 초음파 센서 오른쪽 아래
NewPing sonar2(5, 5, MAX_DISTANCE); // 초음파 센서 왼쪽 아래
NewPing sonar3(6, 6, MAX_DISTANCE); // 초음파 센서 왼쪽 위
NewPing sonar4(7, 7, MAX_DISTANCE); // 초음파 센서 오른쪽 위
NewPing sonar5(8, 8, MAX_DISTANCE); // 초음파 센서 가운데 위

VarSpeedServo  right_eye_LID, right_eye_LR, right_eye_UP;
VarSpeedServo  left_eye_LID, left_eye_LR, left_eye_UP, jaw;

int val;

float target_R_LR, target_R_UP, target_L_LR, target_L_UP;
float current_R_LR, current_R_UP, current_L_LR, current_L_UP;
float step_R_LR, step_R_UP, step_L_LR, step_L_UP;
int stepped = 0;
int relax  = 0;

// 센서 위치 1(오른쪽/아래) 2(왼쪽/아래) 3(왼쪽/옆) 4(오른쪽/옆) 5(중간/위쪽)
// target_R_LR,
// target_R_UP,
// target_L_LR,
// target_L_UP

//int pos[20] = {
//  30, 90, 90, 40, 60,
//  25, 25, 60, 60, 80,
//  110, 40, 40, 110, 70,
//  70, 80, 50, 30, 20
//};

int pos[20] = {
  30, 90, 90, 40, 60,
  40, 40, 60, 60, 80,
  110, 40, 40, 110, 70,
  60, 60, 50, 30, 20
};


void setup() {
  Serial.begin(9600);
  Serial1.begin(9600); // arduino mega p19 (RX) p18 (TX) for Bluetooth

  prev_time = millis();
  prev_time2 = millis();
  prev_time3 = millis();
  prev_time4 = millis();

 // right_eye_LID.attach(9);  // 오.눈꺼풀 열기 25 감기 57
  right_eye_LR.attach(11);  // 오.눈알 오 40 가운데 60 왼 90
  right_eye_UP.attach(10);  // 오.눈알 위 100 가운데 60 아래 25
//  left_eye_LID.attach(12);  // 왼.눈꺼풀 열기 115 감기 75
  left_eye_LR.attach(13);  // 왼.눈알 오 40 가운데 70 왼 100
  left_eye_UP.attach(14);  // 왼.눈알 위 20 가운데 50 아래 80
  jaw.attach(16);  // 턱 닫기 90 열기 120

//  right_eye_LID.write(27);
//  left_eye_LID.write(115);

  right_eye_LR.write(60);
  right_eye_UP.write(70);
  left_eye_LR.write(70);
  left_eye_UP.write(50);
  jaw.write(100);

  stepped = 0;
  delay(100);
  //  right_eye_LR.detach();
  //  right_eye_UP.detach();
  //  left_eye_LR.detach();
  //  left_eye_UP.detach();
}

void loop() {
  test.in(map(analogRead(0), 0, 1023, 0, 200));
  THRESHOLD = test.out();

  for (int i = 0; i < SONAR_NUM; i++) {
    // 초음파 센서 읽기
    //    if (i == 0)
    //      cm[0] = sonar1.ping_cm();
    //    else if (i == 1)
    //      cm[1] = sonar2.ping_cm();
    //    else if (i == 2)
    //      cm[2] = sonar3.ping_cm();
    //    else if (i == 3)
    //      cm[3] = sonar4.ping_cm();
    //    else if (i == 4)
    //      cm[4] = sonar5.ping_cm();

#define ITERATION 5
    if (i == 0)
      cm[0] = sonar1.ping_median(ITERATION) / 58;
    else if (i == 1)
      cm[1] = sonar2.ping_median(ITERATION) / 58;
    else if (i == 2)
      cm[2] = sonar3.ping_median(ITERATION) / 58;
    else if (i == 3)
      cm[3] = sonar4.ping_median(ITERATION) / 58;
    else if (i == 4)
      cm[4] = sonar5.ping_median(ITERATION) / 58;

    delay(50);
    
    // 인식 거리 내에 물체가 들어오면 목표 위치 설정
    detected_value = THRESHOLD;

    for (int j = 0; j < SONAR_NUM; j++) {
      if (cm[j] == 0) {
        cm[j] = MAX_DISTANCE;
      }
      if (cm[j] < detected_value) {
        detected_value = cm[j];
        detected_num = j + 1;
      }
    }

   if (relax == 0) { // 눈 뜨기
    /*  right_eye_LID.write(27);
      left_eye_LID.write(115);
      delay(50);*/

      // 새로운 목표 위치일 경우 눈 좌우상하 설정
      if (detected_num != detected_num_old && detected_num > 0 ) {

        right_eye_LR.attach(11);
        right_eye_UP.attach(10);
        left_eye_LR.attach(13);
        left_eye_UP.attach(14);

        // 눈알 목표 지점으로 움직이기
        int speed = 20;
        left_eye_LR.write(pos[10 + detected_num - 1], speed, false);
        left_eye_UP.write(pos[15 + detected_num - 1], speed, false);
        right_eye_LR.write(pos[detected_num - 1], speed, false);
        right_eye_UP.write(pos[5 + detected_num - 1], speed, false);

        stepped = 50;
        detected_num_old = detected_num;
        prev_time4 = current_time;

      } else if (detected_num == detected_num_old && current_time > prev_time4 + 20000) {

        // 센서 인식 없이 5초 지나면 새 목표 설정
        while (detected_num == detected_num_old)
          detected_num = random(1, 5);

        right_eye_LR.attach(11);
        right_eye_UP.attach(10);
        left_eye_LR.attach(13);
        left_eye_UP.attach(14);

        // 눈알 목표 지점으로 움직이기
        int speed = 20;
        left_eye_LR.write(pos[10 + detected_num - 1], speed, false);
        left_eye_UP.write(pos[15 + detected_num - 1], speed, false);
        right_eye_LR.write(pos[detected_num - 1], speed, false);
        right_eye_UP.write(pos[5 + detected_num - 1], speed, false);

        stepped = 50;
        detected_num_old = detected_num;
        prev_time4 = current_time;

      }
/*
      // 랜덤으로 눈 깜빡이기
      if (current_time > prev_time3 + 6000 && random(30) < 1) {
        right_eye_LID.write(57);
        left_eye_LID.write(75);
        delay(100);
        right_eye_LID.write(27);
        left_eye_LID.write(115);
        prev_time3 = current_time;
      }*/
    }

    // 눈알 다 움직이면 끄기
    if (left_eye_LR.read() == pos[10 + detected_num - 1])
      left_eye_LR.detach();
    if (left_eye_UP.read() == pos[15 + detected_num - 1])
      left_eye_UP.detach();
    if (right_eye_LR.read() == pos[detected_num - 1])
      right_eye_LR.detach();
    if (right_eye_UP.read() == pos[5 + detected_num - 1])
      right_eye_UP.detach();


    // 시리얼로 센서값 및 목표 위치 전송
    Serial.print(THRESHOLD);
    Serial.print("\t");

    for (int j = 0; j < SONAR_NUM; j++) {
      Serial.print(cm[j]);
      Serial.print("\t");
    }
    Serial.print(detected_num);
    Serial.print("\t");
    Serial.print(stepped);
    Serial.print("\t");
    Serial.print(detected_level);
    Serial.print("\t");
    Serial.print(relax);
    Serial.println();


    // 현재 시간 측정하기
    current_time = millis();

    // 1초 단위로 블루투스로 센서 값 및 목표 위치 보내기
    if (current_time - prev_time > 1000) {

      detected_level = 0;

      for (int i = 0; i < SONAR_NUM; i++) {
        Serial1.print(cm[i]);
        Serial1.print(",");
        if (cm[i] < THRESHOLD)
          detected_level = 1;
      }
      Serial1.print(detected_num);
      Serial1.print(",");
      Serial1.print(detected_level);
      Serial1.println();

      prev_time = current_time;

      // 센서 인식 없이 100초가 지난 경우, 눈감고 모터 끄기
      if ((current_time - prev_time2 > 100000) && (detected_level == 0)) {
        relax = 1;

        int speed = 20;
    /*    right_eye_LID.write(53, speed, false);
        left_eye_LID.write(78, speed, false);
        delay(100); */
        right_eye_LR.detach();
        right_eye_UP.detach();
        left_eye_LR.detach();
        left_eye_UP.detach();

      } else if (detected_level == 1) {
        relax = 0;
        prev_time2 = current_time;
      }
    }

  }
}


