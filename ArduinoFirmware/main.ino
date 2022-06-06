#define FARMDUINO_V30

#define USE_TIMER_1     false
#define USE_TIMER_2     true
#define USE_TIMER_3     false
#define USE_TIMER_4     false
#define USE_TIMER_5     false


// To be included only in main(), .ino with setup() to avoid `Multiple Definitions` Linker Error
#include "TimerInterrupt.h"           //https://github.com/khoih-prog/TimerInterrupt

// To be included only in main(), .ino with setup() to avoid `Multiple Definitions` Linker Error
#include "ISR_Timer.h"   

#include "pins.h"
#include "MovementEncoder.h"
#include "SerialTransfer.h"
#include <TMC2130Stepper.h>

#define DEFAULT_SPEED_RESOLUTION 32

SerialTransfer transferStream;

struct SendingDataFrame {
  float raw_encoder_x;
  float raw_encoder_y;
  float raw_encoder_z;
} realTimeData;

struct receivingDataFrame {
    float x_control = 0;        // stop=0, forward=1, backward=-1
    float x_speed = 5000;       // frequency
    float x_resolution = 16;    // 8,16,32,64,128,256
    float y_control = 0;
    float y_speed = 5000;
    float y_resolution = 16;
    float z_control = 0;
    float z_speed = 5000;
    float z_resolution = 16;
} receivedCommand; 

  // Use encoder (0 or 1)
const long ENCODER_INVERT_DEFAULT = 1;
 
  // Type of enocder.
  // 0 = non-differential encoder, channel A,B
  // 1 = differenttial encoder, channel A, A*, B, B*
const long ENCODER_TYPE_DEFAULT = 1;

  // Position = encoder position * scaling / 10000
const long ENCODER_SCALING_DEFAULT = 5556;

TMC2130Stepper X_Axis_Left = TMC2130Stepper(X_ENABLE_PIN, X_DIR_PIN, X_STEP_PIN, X_CHIP_SELECT);
TMC2130Stepper X_Axis_Right = TMC2130Stepper(E_ENABLE_PIN, E_DIR_PIN, E_STEP_PIN, E_CHIP_SELECT);

TMC2130Stepper Y_Axis = TMC2130Stepper(Y_ENABLE_PIN, Y_DIR_PIN, Y_STEP_PIN, Y_CHIP_SELECT);
TMC2130Stepper Z_Axis = TMC2130Stepper(Z_ENABLE_PIN, Z_DIR_PIN, Z_STEP_PIN, Z_CHIP_SELECT);

MovementEncoder  encoderX = MovementEncoder();
MovementEncoder  encoderY = MovementEncoder();
MovementEncoder  encoderZ = MovementEncoder();

int getSpeed(int inputResolution){

  if( inputResolution == 256 || 
      inputResolution == 128 ||
      inputResolution == 64  ||
      inputResolution == 32  ||
      inputResolution == 16  ||
      inputResolution == 8)
        {
          return inputResolution;
        }
   return DEFAULT_SPEED_RESOLUTION; 
}

void XAxisTimer() {
 
      digitalWrite(X_STEP_PIN, HIGH);
      digitalWrite(E_STEP_PIN, HIGH);
      digitalWrite(Y_STEP_PIN, HIGH);
      digitalWrite(Z_STEP_PIN, HIGH);
      delayMicroseconds(10);
 
      digitalWrite(X_STEP_PIN, LOW);
      digitalWrite(E_STEP_PIN, LOW);
      digitalWrite(Y_STEP_PIN, LOW);
      digitalWrite(Z_STEP_PIN, LOW);
      delayMicroseconds(2);
      //delayMicroseconds(inactive_dutyCycle); 
}

void InitializeSPI(){
  // SPI initialization
  //pinMode(READ_ENA_PIN, INPUT_PULLUP);
  pinMode(NSS_PIN, OUTPUT);
  digitalWrite(NSS_PIN, HIGH);
  
  SPI.setBitOrder(MSBFIRST);
    #if defined(FARMDUINO_V32)
  SPI.setDataMode(SPI_MODE3);
    #else
  SPI.setDataMode(SPI_MODE0);
    #endif
  SPI.setClockDivider(SPI_CLOCK_DIV4);
  SPI.begin();
}

void InitializePins(){
  
  pinMode(X_ENABLE_PIN, OUTPUT); 
  pinMode(X_DIR_PIN, OUTPUT);
  pinMode(X_STEP_PIN, OUTPUT);
  pinMode(X_CHIP_SELECT, OUTPUT);

  pinMode(E_ENABLE_PIN, OUTPUT); 
  pinMode(E_DIR_PIN, OUTPUT);
  pinMode(E_STEP_PIN, OUTPUT);
  pinMode(E_CHIP_SELECT, OUTPUT);

  pinMode(Y_ENABLE_PIN, OUTPUT); 
  pinMode(Y_DIR_PIN, OUTPUT);
  pinMode(Y_STEP_PIN, OUTPUT);
  pinMode(Y_CHIP_SELECT, OUTPUT);

  pinMode(Z_ENABLE_PIN, OUTPUT); 
  pinMode(Z_DIR_PIN, OUTPUT);
  pinMode(Z_STEP_PIN, OUTPUT);
  //pinMode(Z_CHIP_SELECT, OUTPUT); // it disables the X axis !
}

void setUp_X_Driver(int resolution){
  X_Axis_Left.begin(); // Initiate pins and registeries
  X_Axis_Left.stealthChop(1); // Enable extremely quiet stepping
  X_Axis_Left.setCurrent(2000,0.9,0.7); //current , sense resistor, hold current
  X_Axis_Left.microsteps(resolution);
  X_Axis_Left.double_edge_step(1);
  X_Axis_Left.hold_current(31);  //stand-still current value: 0-31
  X_Axis_Left.run_current(31);   //runing current value: 0-31
  
  
  X_Axis_Right.begin(); // Initiate pins and registeries
  X_Axis_Right.stealthChop(1); // Enable extremely quiet stepping
  X_Axis_Right.setCurrent(2000,0.9,0.7); //current , sense resistor, hold current
  X_Axis_Right.microsteps(resolution);
  X_Axis_Right.double_edge_step(1);
  X_Axis_Right.hold_current(31);  //stand-still current value: 0-31
  X_Axis_Right.run_current(31);   //runing current value: 0-31
}

void setUp_Y_Driver(int resolution){
  Y_Axis.begin(); // Initiate pins and registeries
  Y_Axis.stealthChop(1); // Enable extremely quiet stepping
  Y_Axis.setCurrent(2000,0.9,0.7);
  Y_Axis.microsteps(resolution);
  Y_Axis.double_edge_step(1);
  Y_Axis.hold_current(31);  //stand-still current value: 0-31
  Y_Axis.run_current(31);   //runing current value: 0-31
}

void setUp_Z_Driver(int resolution){
  Z_Axis.begin(); // Initiate pins and registeries
  Z_Axis.stealthChop(1); // Enable extremely quiet stepping
  Z_Axis.setCurrent(2000,0.9,0.7);
  Z_Axis.microsteps(resolution);
  Z_Axis.double_edge_step(1);
  Z_Axis.hold_current(31);  //stand-still current value: 0-31
  Z_Axis.run_current(31);   //runing current value: 0-31
}

 
void setup() {

  InitializeSPI();
  InitializePins();  
  
  setUp_X_Driver(DEFAULT_SPEED_RESOLUTION);
  setUp_Y_Driver(DEFAULT_SPEED_RESOLUTION);
  setUp_Z_Driver(DEFAULT_SPEED_RESOLUTION);
  
  Serial.begin(115200);
  while (!Serial);
  transferStream.begin(Serial);

  encoderX.loadMdlEncoderId(_MDL_X1);
  encoderY.loadMdlEncoderId(_MDL_Y);
  encoderZ.loadMdlEncoderId(_MDL_Z);

  encoderX.loadPinNumbers(X_ENCDR_A, X_ENCDR_B, X_ENCDR_A_Q, X_ENCDR_B_Q);
  encoderY.loadPinNumbers(Y_ENCDR_A, Y_ENCDR_B, Y_ENCDR_A_Q, Y_ENCDR_B_Q);
  encoderZ.loadPinNumbers(Z_ENCDR_A, Z_ENCDR_B, Z_ENCDR_A_Q, Z_ENCDR_B_Q);

  // type, scaling, invert 
  encoderX.loadSettings(ENCODER_TYPE_DEFAULT, ENCODER_SCALING_DEFAULT, ENCODER_INVERT_DEFAULT);
  encoderY.loadSettings(ENCODER_TYPE_DEFAULT, ENCODER_SCALING_DEFAULT, ENCODER_INVERT_DEFAULT);
  encoderZ.loadSettings(ENCODER_TYPE_DEFAULT, ENCODER_SCALING_DEFAULT, ENCODER_INVERT_DEFAULT);

  encoderX.setPosition(0);
  encoderY.setPosition(0);
  encoderZ.setPosition(0);
  
  ITimer2.init();
   
  //Frequency in float Hz
  float frequency = 5000.00; 
  if (ITimer2.attachInterrupt(frequency, XAxisTimer))
    //Serial.println("Successfully Initialized!");
    int i=0;
  else
    int b=0;
    //Serial.println("Can't set ITimer. Select another freq. or timer");
}
 int last_res_X = DEFAULT_SPEED_RESOLUTION;
 int last_res_Y = DEFAULT_SPEED_RESOLUTION;
 int last_res_Z = DEFAULT_SPEED_RESOLUTION;
 
void loop() {
  
     if(transferStream.available())
     {
      // use this variable to keep track of how many
      // bytes we've processed from the receive buffer
      uint16_t recSize = 0;

      recSize = transferStream.rxObj(receivedCommand, recSize);

      int x_res = getSpeed(receivedCommand.x_resolution);
      if(last_res_X!=x_res){
          setUp_X_Driver(x_res);
          last_res_X = x_res;
      }

      int y_res = getSpeed(receivedCommand.y_resolution);
      if(last_res_Y!=y_res){
          setUp_Y_Driver(y_res);
          last_res_Y = y_res;
      }
      
      int z_res = getSpeed(receivedCommand.z_resolution);
      if(last_res_Z!=z_res){
          setUp_Z_Driver(z_res); 
          last_res_Z = z_res;
      }
      
                if(receivedCommand.x_control>0){
                     digitalWrite(E_ENABLE_PIN, LOW);
                     digitalWrite(X_ENABLE_PIN, LOW);
                     
                     digitalWrite(X_DIR_PIN, HIGH);
                     digitalWrite(E_DIR_PIN, LOW);
                     
                  }
                else if (receivedCommand.x_control<0){
                     //Xenabled
                    digitalWrite(E_ENABLE_PIN, LOW);
                    digitalWrite(X_ENABLE_PIN, LOW);
                          
                    digitalWrite(X_DIR_PIN, LOW);
                    digitalWrite(E_DIR_PIN, HIGH);
                    
                  }
                 else if (receivedCommand.x_control==0){
                    digitalWrite(E_ENABLE_PIN, HIGH);
                    digitalWrite(X_ENABLE_PIN, HIGH);               
                  }

                if(receivedCommand.y_control>0){
                    //Y enabled
                     digitalWrite(Y_ENABLE_PIN, LOW);
                     digitalWrite(Y_DIR_PIN, HIGH);
                  }
                else if (receivedCommand.y_control<0){
                     digitalWrite(Y_ENABLE_PIN, LOW);  
                     digitalWrite(Y_DIR_PIN, LOW);
                  }

               else if (receivedCommand.y_control==0){
                    digitalWrite(Y_ENABLE_PIN, HIGH);            
                  }

                if(receivedCommand.z_control>0){
                     digitalWrite(Z_ENABLE_PIN, LOW);
                     digitalWrite(Z_DIR_PIN, HIGH);
                  }
                else if (receivedCommand.z_control<0){
                     digitalWrite(Z_ENABLE_PIN, LOW);  
                     digitalWrite(Z_DIR_PIN, LOW);
                  }

               else if (receivedCommand.z_control==0){
                    digitalWrite(Z_ENABLE_PIN, HIGH);                 
                  }
     } 
     
     encoderX.processEncoder(); 
     realTimeData.raw_encoder_x = encoderX.currentPosition();

     encoderY.processEncoder(); 
     realTimeData.raw_encoder_y = encoderY.currentPosition();  
 
     encoderZ.processEncoder(); 
     realTimeData.raw_encoder_z = encoderZ.currentPosition();


     // use this variable to keep track of how many
     // bytes we're stuffing in the transmit buffer
     uint16_t sendSize = 0;
     sendSize = transferStream.txObj(realTimeData, sendSize);
     transferStream.sendData(sendSize);
     delay(20);     
}
