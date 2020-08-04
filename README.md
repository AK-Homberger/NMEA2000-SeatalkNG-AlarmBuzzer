# NMEA2000-SeatalkNG-AlarmBuzzer
This is external buzzer for NMEA2000 / SeaTalk-NG Alarms.

See Canboat project for PGN details and alarm types (LOOKUP_SEATALK_ALARM_ID and PGN 65288): https://github.com/canboat/canboat/blob/master/analyzer/pgn.h

Currently PGN 65288 alarms are supported. Other NMEA2000 alarms can be integrated if required (e.g engine related).

An external Buzzer have to be connected.

![Schematics](https://github.com/AK-Homberger/NMEA2000-SeatalkNG-AlarmBuzzer/blob/master/NMEA2000-AlarmBuzzer.png)

The wanted alarms have to be defined in the table within the code:

struct Seatalk_Alarm Seatalk_Alarm_table[] = {

  { 1,  0 },  // Shallow depth
  
  { 5,  0 },  // Off Course
  
  { 14, 0 },  // WP Arrival
  
  { 20, 0 },  // Pilot Off Course
  
  { 21, 0 },  // Pilot Wind Shift
  
  { 22, 0 },  // Pilot Low Battery
  
  { 29, 0 },  // Pilot Way Point Advance
  
  { 37, 0 },  // GPS Failure
  
  { 38, 0 },  // MOB
  
  { 104, 0 }, // AIS Dangerous Target
  
};

Just change/add/delete alarms as wanted with the Alarm ID from LOOKUP_SEATALK_ALARM_ID

The code is using the NMEA 2000 libraries from Timo Lappalainen (https://github.com/ttlappalainen).

Download and install the following libraries from GitHub link above:

- NMEA2000
- NMEA2000_esp32


# Update
Verion 0.3 - 04.08.20: Changed TX pin from 2 to 5 and buzzer pin from 5 to 15 to avoid CAN bus problems during ESP32 reboot/programming.

Verion 0.1 - 28.07.20: Initial version.
