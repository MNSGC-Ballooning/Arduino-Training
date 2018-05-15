# Introduction to Arduino

This repository contains code for the exercises in the MNSGC Introduction to Arduino tutorial, which can be found here:
https://docs.google.com/presentation/d/1eIsCDgpDu-nCz0vGucmX006oPLuZxcmLnavc_X0pgl8/edit?usp=sharing

## Exercises currently included in this tutorial:
1. BasicBlink - a simple program to blink an led repeatedly, introducing basic sketch layout
2. Sweep - introduces objects and built-in libraries by driving a servo
3. AnalogPressure - introduces reading sensors, starting with a raw analog pressure sensor
4. AnalogHumidity - more practice with reading analog
5. DigitalAccel - introduces more complicated digital sensors, and the libraries that drive them
6. DigitalPressure - introduces multi-sensor ICs with a combined pressure/temp sensor
7. TempMagLogger - combines digital and analog sensors, and introduces SD datalogging
8. SDTempBus - introduces bus communication using OneWire protocol
9. GPSLogger - switch from computer tether to battery power, logs position of arduino
10. XBeeBlink - led blinking sketch that accepts input over xBee, allowing remote control of arduino

## Libraries required:
- [ADXL345](https://github.com/sparkfun/SparkFun_ADXL345_Arduino_Library)
- [MPL3115A](https://github.com/sparkfun/SparkFun_MPL3115A2_Breakout_Arduino_Library)
- [MAG3110](https://github.com/sparkfun/SparkFun_MAG3110_Breakout_Board_Arduino_Library)
- [OneWire](https://github.com/PaulStoffregen/OneWire)
- [DallasTemperature](https://github.com/milesburton/Arduino-Temperature-Control-Library)
- [TinyGPS](https://github.com/mikalhart/TinyGPS)
- [FlightGPS](https://github.com/MNSGC-Ballooning/FlightGPS)
- [Relay_XBee](https://github.com/MNSGC-Ballooning/Relay_XBee)