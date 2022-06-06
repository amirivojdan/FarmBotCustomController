  #define NSS_PIN 22

    // X1-AXIS
  #define X_STEP_PIN 26     // X1_STEP_PIN
  #define X_DIR_PIN 27      // X1_DIR_PIN
  #define X_ENABLE_PIN 25   // X1_ENABLE_PIN
  #define X_CHIP_SELECT 24  // X1_CHIP_SELECT
  #define X_MIN_PIN 69
  #define X_MAX_PIN 68
  #define X_ENCDR_A -1      // N/A
  #define X_ENCDR_B -1      // N/A
  #define X_ENCDR_A_Q -1    // N/A
  #define X_ENCDR_B_Q -1    // N/A

    // X2-AXIS
  #define E_STEP_PIN 15     // X2_STEP_PIN
  #define E_DIR_PIN 30      // X2_DIR_PIN
  #define E_ENABLE_PIN 14   // X2_ENABLE_PIN
  #define E_CHIP_SELECT 29  // X2_CHIP_SELECT
  #define X2_ENCDR_A -1     // N/A
  #define X2_ENCDR_B -1     // N/A

    // Y-AXIS
  #define Y_STEP_PIN 32
  #define Y_DIR_PIN 33
  #define Y_ENABLE_PIN 31
  #define Y_CHIP_SELECT 28  // Y_CHIP_SELECT
  #define Y_MIN_PIN 67
  #define Y_MAX_PIN 66
  #define Y_ENCDR_A -1      // N/A
  #define Y_ENCDR_B -1      // N/A
  #define Y_ENCDR_A_Q -1    // N/A
  #define Y_ENCDR_B_Q -1    // N/A

    // Z-AXIS
  #define Z_STEP_PIN 35
  #define Z_DIR_PIN 36
  #define Z_ENABLE_PIN 34
  #define Z_CHIP_SELECT 23  // Z_CHIP_SELECT
  #define Z_MIN_PIN 65
  #define Z_MAX_PIN 64
  #define Z_ENCDR_A -1      // N/A
  #define Z_ENCDR_B -1      // N/A
  #define Z_ENCDR_A_Q -1    // N/A
  #define Z_ENCDR_B_Q -1    // N/A

    // UTM
  #define UTM_C 63 // TOOL VERIFICATION
  #define UTM_D 59 // SOIL SENSOR
  #define UTM_E -1
  #define UTM_F -1
  #define UTM_G -1
  #define UTM_H -1
  #define UTM_I -1
  #define UTM_J -1
  #define UTM_K -1
  #define UTM_L -1
    // Available digital pins: 2,3,16,17,18,19,22,38,39,42,43,44,45,46,47,48
    // Available analog pins: 6,7,8

  #define LED_PIN 13

    // Peripherals
  #define LIGHTING_PIN 7
  #define WATER_PIN 8
  #define VACUUM_PIN 9
  #define PERIPHERAL_4_PIN 10
  #define PERIPHERAL_5_PIN 12

    // Peripheral current sensors
  #define LIGHTING_CURRENT_PIN 54 // A0
  #define WATER_CURRENT_PIN 55 // A1
  #define VACUUM_CURRENT_PIN 58  // A4
  #define PERIPHERAL_4_CURRENT_PIN 57 // A3
  #define PERIPHERAL_5_CURRENT_PIN 56 // A2

    // Auxiliary motors
  #define AUX_STEP_PIN 40
  #define AUX_DIR_PIN 41
  #define AUX_ENABLE_PIN 37

  #define SERVO_0_PIN 4
  #define SERVO_1_PIN 5
  #define SERVO_2_PIN 6
  #define SERVO_3_PIN 11

    // Encoder X channel A (disabled, use LED pin): pin 13, port B7
  #define ENC_X_A_PORT PINB
  #define ENC_X_A_BYTE 0x80

    // Encoder X channel B (disabled, use LED pin): pin 13, port B7
  #define ENC_X_B_PORT PINB
  #define ENC_X_B_BYTE 0x80

    // Encoder X channel A Q (disabled, use LED pin): pin 13, port B7
  #define ENC_X_A_Q_PORT PINB
  #define ENC_X_A_Q_BYTE 0x80

    // Encoder X channel B Q (disabled, use LED pin): pin 13, port B7
  #define ENC_X_B_Q_PORT PINB
  #define ENC_X_B_Q_BYTE 0x80

    // Encoder Y channel A (disabled, use LED pin): pin 13, port B7
  #define ENC_Y_A_PORT PINB
  #define ENC_Y_A_BYTE 0x80

    // Encoder Y channel B (disabled, use LED pin): pin 13, port B7
  #define ENC_Y_B_PORT PINB
  #define ENC_Y_B_BYTE 0x80

    // Encoder Y channel A Q (disabled, use LED pin): pin 13, port B7
  #define ENC_Y_A_Q_PORT PINB
  #define ENC_Y_A_Q_BYTE 0x80

    // Encoder Y channel B Q (disabled, use LED pin): pin 13, port B7
  #define ENC_Y_B_Q_PORT PINB
  #define ENC_Y_B_Q_BYTE 0x80

    // Encoder Z channel A (disabled, use LED pin): pin 13, port B7
  #define ENC_Z_A_PORT PINB
  #define ENC_Z_A_BYTE 0x80

    // Encoder Z channel B (disabled, use LED pin): pin 13, port B7
  #define ENC_Z_B_PORT PINB
  #define ENC_Z_B_BYTE 0x80

    // Encoder Z channel A Q (disabled, use LED pin): pin 13, port B7
  #define ENC_Z_A_Q_PORT PINB
  #define ENC_Z_A_Q_BYTE 0x80

    // Encoder Z channel B Q (disabled, use LED pin): pin 13, port B7
  #define ENC_Z_B_Q_PORT PINB
  #define ENC_Z_B_Q_BYTE 0x80
