/*
  This code is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.
  This code is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.
  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

// NMEA2000 Alarm Buzzer
//   - Reads SeaTalk-NG Alarm Messages from NMEA2000 bus (PGN 65288)
//   - Generate external alarm with relay connected to GPIO pin 5

// Version 0.1, 28.07.2020, AK-Homberger

#define ESP32_CAN_TX_PIN GPIO_NUM_2  // Set CAN TX port to 2 
#define ESP32_CAN_RX_PIN GPIO_NUM_4  // Set CAN RX port to 4

#define ALARM_PIN 5 // Alarm buzzer relay pin

#include <Arduino.h>
#include <N2kMsg.h>
#include <NMEA2000.h>
#include <NMEA2000_CAN.h>


// See Canboat project for types (LOOKUP_SEATALK_ALARM_ID / LOOKUP_SEATALK_ALARM_GROUP): https://github.com/canboat/canboat/blob/master/analyzer/pgn.h

#define LOOKUP_SEATALK_ALARM_STATUS          \
  (",0=Alarm condition not met"              \
   ",1=Alarm condition met and not silenced" \
   ",2=Alarm condition met and silenced")

#define LOOKUP_SEATALK_ALARM_ID                                   \
  (",0=No Alarm"                                                  \
   ",1=Shallow Depth"                                             \
   ",2=Deep Depth"                                                \
   ",3=Shallow Anchor"                                            \
   ",4=Deep Anchor"                                               \
   ",5=Off Course"                                                \
   ",6=AWA High"                                                  \
   ",7=AWA Low"                                                   \
   ",8=AWS High"                                                  \
   ",9=AWS Low"                                                   \
   ",10=TWA High"                                                 \
   ",11=TWA Low"                                                  \
   ",12=TWS High"                                                 \
   ",13=TWS Low"                                                  \
   ",14=WP Arrival"                                               \
   ",15=Boat Speed High"                                          \
   ",16=Boat Speed Low"                                           \
   ",17=Sea Temp High"                                            \
   ",18=Sea Temp Low"                                             \
   ",19=Pilot Watch"                                              \
   ",20=Pilot Off Course"                                         \
   ",21=Pilot Wind Shift"                                         \
   ",22=Pilot Low Battery"                                        \
   ",23=Pilot Last Minute Of Watch"                               \
   ",24=Pilot No NMEA Data"                                       \
   ",25=Pilot Large XTE"                                          \
   ",26=Pilot NMEA DataError"                                     \
   ",27=Pilot CU Disconnected"                                    \
   ",28=Pilot Auto Release"                                       \
   ",29=Pilot Way Point Advance"                                  \
   ",30=Pilot Drive Stopped"                                      \
   ",31=Pilot Type Unspecified"                                   \
   ",32=Pilot Calibration Required"                               \
   ",33=Pilot Last Heading"                                       \
   ",34=Pilot No Pilot"                                           \
   ",35=Pilot Route Complete"                                     \
   ",36=Pilot Variable Text"                                      \
   ",37=GPS Failure"                                              \
   ",38=MOB"                                                      \
   ",39=Seatalk1 Anchor"                                          \
   ",40=Pilot Swapped Motor Power"                                \
   ",41=Pilot Standby Too Fast To Fish"                           \
   ",42=Pilot No GPS Fix"                                         \
   ",43=Pilot No GPS COG"                                         \
   ",44=Pilot Start Up"                                           \
   ",45=Pilot Too Slow"                                           \
   ",46=Pilot No Compass"                                         \
   ",47=Pilot Rate Gyro Fault"                                    \
   ",48=Pilot Current Limit"                                      \
   ",49=Pilot Way Point Advance Port"                             \
   ",50=Pilot Way Point Advance Stbd"                             \
   ",51=Pilot No Wind Data"                                       \
   ",52=Pilot No Speed Data"                                      \
   ",53=Pilot Seatalk Fail1"                                      \
   ",54=Pilot Seatalk Fail2"                                      \
   ",55=Pilot Warning Too Fast To Fish"                           \
   ",56=Pilot Auto Dockside Fail"                                 \
   ",57=Pilot Turn Too Fast"                                      \
   ",58=Pilot No Nav Data"                                        \
   ",59=Pilot Lost Waypoint Data"                                 \
   ",60=Pilot EEPROM Corrupt"                                     \
   ",61=Pilot Rudder Feedback Fail"                               \
   ",62=Pilot Autolearn Fail1"                                    \
   ",63=Pilot Autolearn Fail2"                                    \
   ",64=Pilot Autolearn Fail3"                                    \
   ",65=Pilot Autolearn Fail4"                                    \
   ",66=Pilot Autolearn Fail5"                                    \
   ",67=Pilot Autolearn Fail6"                                    \
   ",68=Pilot Warning Cal Required"                               \
   ",69=Pilot Warning OffCourse"                                  \
   ",70=Pilot Warning XTE"                                        \
   ",71=Pilot Warning Wind Shift"                                 \
   ",72=Pilot Warning Drive Short"                                \
   ",73=Pilot Warning Clutch Short"                               \
   ",74=Pilot Warning Solenoid Short"                             \
   ",75=Pilot Joystick Fault"                                     \
   ",76=Pilot No Joystick Data"                                   \
   ",77=not assigned"                                             \
   ",78=not assigned"                                             \
   ",79=not assigned"                                             \
   ",80=Pilot Invalid Command"                                    \
   ",81=AIS TX Malfunction"                                       \
   ",82=AIS Antenna VSWR fault"                                   \
   ",83=AIS Rx channel 1 malfunction"                             \
   ",84=AIS Rx channel 2 malfunction"                             \
   ",85=AIS No sensor position in use"                            \
   ",86=AIS No valid SOG information"                             \
   ",87=AIS No valid COG information"                             \
   ",88=AIS 12V alarm"                                            \
   ",89=AIS 6V alarm"                                             \
   ",90=AIS Noise threshold exceeded channel A"                   \
   ",91=AIS Noise threshold exceeded channel B"                   \
   ",92=AIS Transmitter PA fault"                                 \
   ",93=AIS 3V3 alarm"                                            \
   ",94=AIS Rx channel 70 malfunction"                            \
   ",95=AIS Heading lost/invalid"                                 \
   ",96=AIS internal GPS lost"                                    \
   ",97=AIS No sensor position"                                   \
   ",98=AIS Lock failure"                                         \
   ",99=AIS Internal GGA timeout"                                 \
   ",100=AIS Protocol stack restart"                              \
   ",101=Pilot No IPS communications"                             \
   ",102=Pilot Power-On or Sleep-Switch Reset While Engaged"      \
   ",103=Pilot Unexpected Reset While Engaged"                    \
   ",104=AIS Dangerous Target"                                    \
   ",105=AIS Lost Target"                                         \
   ",106=AIS Safety Related Message (used to silence)"            \
   ",107=AIS Connection Lost"                                     \
   ",108=No Fix")

#define LOOKUP_SEATALK_ALARM_GROUP \
  (",0=Instrument"                 \
   ",1=Autopilot"                  \
   ",2=Radar"                      \
   ",3=Chart Plotter"              \
   ",4=AIS")


// Struct to hold alarm state for any defined alarm
struct Seatalk_Alarm {
  unsigned char ID;
  unsigned char AlarmState;
};


// Just add a row for any required alarm to the following table with data from LOOKUP_SEATALK_ALARM_ID

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
int Alarms = sizeof(Seatalk_Alarm_table) / sizeof(Seatalk_Alarm); // Number of alarms is calculated atomatically


const unsigned long TransmitMessages[] PROGMEM = { 0
                                                 };

const unsigned long ReceiveMessages[] PROGMEM = { 65288UL,    // Read Seatalk Alarm State
                                                  0
                                                };


void setup() {
  uint8_t chipid[6];
  uint32_t id = 0;
  int i = 0;

  pinMode(9, ALARM_PIN);
  digitalWrite(ALARM_PIN, 0);

  esp_efuse_read_mac(chipid);
  for (i = 0; i < 6; i++) id += (chipid[i] << (7 * i));

  // Reserve enough buffer for sending all messages. This does not work on small memory devices like Uno or Mega
  NMEA2000.SetN2kCANReceiveFrameBufSize(150);
  NMEA2000.SetN2kCANMsgBufSize(8);
  // Set Product information
  NMEA2000.SetProductInformation("00000001", // Manufacturer's Model serial code
                                 100, // Manufacturer's product code
                                 "Alarm Buzzer",  // Manufacturer's Model ID
                                 "1.0.0.0",  // Manufacturer's Software version code
                                 "1.0.0.0" // Manufacturer's Model version
                                );
  // Set device information
  NMEA2000.SetDeviceInformation(id, // Unique number. Use e.g. Serial number.
                                140, // Device function=Analog to NMEA 2000 Gateway. See codes on http://www.nmea.org/Assets/20120726%20nmea%202000%20class%20&%20function%20codes%20v%202.00.pdf
                                120, // Device class=Inter/Intranetwork Device. See codes on  http://www.nmea.org/Assets/20120726%20nmea%202000%20class%20&%20function%20codes%20v%202.00.pdf
                                2046 // Just choosen free from code list on http://www.nmea.org/Assets/20121020%20nmea%202000%20registration%20list.pdf
                               );

  Serial.begin(115200);
  delay(100);


  // Uncomment 3 rows below to see, what device will send to bus
  //NMEA2000.SetForwardStream(&Serial);  // PC output on due programming port
  //NMEA2000.SetForwardType(tNMEA2000::fwdt_Text); // Show in clear text. Leave uncommented for default Actisense format.
  //NMEA2000.SetForwardOwnMessages();

  // If you also want to see all traffic on the bus use N2km_ListenAndNode instead of N2km_NodeOnly below
  NMEA2000.SetMode(tNMEA2000::N2km_NodeOnly, 52); //N2km_NodeOnly N2km_ListenAndNode
  NMEA2000.ExtendTransmitMessages(TransmitMessages);
  NMEA2000.ExtendReceiveMessages(ReceiveMessages);

  NMEA2000.SetMsgHandler(MyHandleNMEA2000Msg);

  //NMEA2000.SetDebugMode(tNMEA2000::dm_ClearText); // Uncomment this, so you can test code without CAN bus chips on Arduino Mega
  NMEA2000.EnableForward(false); // Disable all msg forwarding to USB (=Serial)
  NMEA2000.Open();

  Serial.println((String) "NMEA2000 Open");
}


// Set AlarmState for AlarmID

void SetAlarm(unsigned char AlarmID, unsigned char State) {
  unsigned char i;

  for (i = 0; i < Alarms; i++) {
    if (Seatalk_Alarm_table[i].ID == AlarmID) Seatalk_Alarm_table[i].AlarmState = State;
  }

}

// Check for any active AlarmState

bool IsAlarm(void) {
  unsigned char i;

  for (i = 0; i < Alarms; i++) {
    if (Seatalk_Alarm_table[i].AlarmState == 1) return true;
  }
  return false;
}


// Handle NMEA2000 messages

void MyHandleNMEA2000Msg(const tN2kMsg &N2kMsg) {

  if (N2kMsg.PGN == 65288L) { // SeaTalk Alarm
    unsigned char AlarmState;
    unsigned char AlarmCode;
    unsigned char AlarmGroup;

    int Index = 3;

    AlarmState = N2kMsg.GetByte(Index);
    AlarmCode  = N2kMsg.GetByte(Index);
    AlarmGroup = N2kMsg.GetByte(Index);

    Serial.printf("Alarm: State=%d, Code=%d Group=%d \n", AlarmState, AlarmCode, AlarmGroup);

    SetAlarm(AlarmCode, AlarmState);

  }
}


void loop() {

  NMEA2000.ParseMessages();

  if (IsAlarm()) {
    digitalWrite(ALARM_PIN, true);
  }
  else {
    digitalWrite(ALARM_PIN, false);
  }

}
