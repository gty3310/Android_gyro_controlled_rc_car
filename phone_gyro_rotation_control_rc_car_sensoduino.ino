
// Replace sensor numbers with Names
#include <SoftwareSerial.h>
#include <AFMotor.h>

SoftwareSerial bt(A5, A4);
AF_DCMotor motor(2, MOTOR12_64KHZ); // create motor #2, 64KHz pwm
AF_DCMotor motor1(1, MOTOR12_64KHZ);

#define START_CMD_CHAR '>'
#define END_CMD_CHAR '\n'
#define DIV_CMD_CHAR ','
#define LEFT_WHEEL 3
#define RIGHT_WHEEL 5
#define DEBUG 1 // Set to 0 if you don't want serial output of sensor data
String inText;
float value0, value1, value2;
int leftWheelPower = 100;
int rightWheelPower = 100;
void setup() {
  Serial.begin(115200);
  motor.setSpeed(leftWheelPower);
  motor1.setSpeed(rightWheelPower);
  bt.begin(9600);
}
void loop()
{
  motor.run(FORWARD);
  motor1.run(FORWARD);
  Serial.flush();
  int inCommand = 0;
  int sensorType = 0;
  unsigned long logCount = 0L;
  char getChar = ' ';  //read serial
  // wait for incoming data
  if (bt.available() >= 1) {
    // parse incoming command start flag
    getChar = bt.read();
    if (getChar != START_CMD_CHAR) return; // if no command start flag, return to loop().
    // parse incoming pin# and value
    sensorType = bt.parseInt(); // read sensor typr
    logCount = bt.parseInt();  // read total logged sensor readings
    value0 = bt.parseFloat();  // 1st sensor value
    value1 = bt.parseFloat();  // 2rd sensor value if exists
    value2 = bt.parseFloat();  // 3rd sensor value if exists
    if (sensorType == 3) { //Orientation
      int pitch = (int) value1;
      if (pitch > 0) {
        leftWheelPower -= pitch;
        rightWheelPower += pitch;
      } else if (pitch < 0) {
        leftWheelPower += pitch;
        rightWheelPower -= pitch;
      }
    }
    if (DEBUG) {
      Serial.print("Sensor type: ");
      Serial.println(sensorType);
      Serial.print("Sensor log#: ");
      Serial.println(logCount);
      Serial.print("Val[0]: ");
      Serial.println(value0);
      Serial.print("Val[1]: ");
      Serial.println(value1);
      Serial.print("Val[2]: ");
      Serial.println(value2);
      Serial.println("-----------------------");
      delay(10);
    }
  }
 
  motor.setSpeed(leftWheelPower);
  motor1.setSpeed(rightWheelPower);
}
