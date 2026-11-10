// config.h - all pin numbers and settings in one place

#ifndef CONFIG_H
#define CONFIG_H

// ---------- Serial ----------
#define SERIAL_BAUD         115200

// ---------- TFT display (SPI) ----------
#define PIN_TFT_CS          4
#define PIN_TFT_DC          5
#define PIN_TFT_RST         6
#define PIN_TFT_MOSI        11      // labelled SDA on the module
#define PIN_TFT_SCLK        12      // labelled SCL on the module

#define TFT_WIDTH           240     // native panel size
#define TFT_HEIGHT          320
#define TFT_ROTATION        1       // 1 = landscape
#define TFT_SPI_MODE        SPI_MODE0   
#define SCREEN_W            320     // size after rotation
#define SCREEN_H            240

// ---------- BMP280 sensor (I2C) ----------
#define PIN_I2C_SDA         8
#define PIN_I2C_SCL         9
#define BMP280_ADDRESS_1    0x76    // SDO connected to GND
#define BMP280_ADDRESS_2    0x77    // SDO connected to 3V3
#define SENSOR_PERIOD_MS    500     // time between sensor reads

// ---------- Joystick (KY023) ----------
#define PIN_JOY_X           1       // VRX
#define PIN_JOY_Y           2       // VRY
#define PIN_JOY_SW          7       // push button (active LOW)

#define JOY_LOW             1000    // ADC thresholds (12-bit, centre is about 2048)
#define JOY_HIGH            3000
#define JOY_INVERT_X        0       // set to 1 if left/right feel backwards
#define JOY_INVERT_Y        0       // set to 1 if up/down feel backwards
#define JOY_REPEAT_MS       200     // time between repeated moves
#define BUTTON_DEBOUNCE_MS  40

// ---------- Sea level pressure reference ----------
#define SEA_LEVEL_DEFAULT   1013.25f    // hPa
#define SEA_LEVEL_MIN       900.0f
#define SEA_LEVEL_MAX       1100.0f
#define SEA_LEVEL_STEP_FINE   0.1f      // joystick up/down
#define SEA_LEVEL_STEP_COARSE 1.0f      // joystick left/right

#endif
