# NMEA2000 SeatalkNG AlarmBuzzer
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

# Remove the 120 ohm resistor from the transceiver
For unknown reasons, many CAN bus transceivers for the ESP32 have a 120 Ohm resistor built into them. The resistor does not belong to the devices at the ends of the stub lines, but to the ends of the backbone cable.

Whether the transceiver contains a 120 ohm resistor can be determined either by looking at the [circuit diagram](https://github.com/AK-Homberger/NMEA2000-Workshop/blob/main/Docs/SN65HVD230%20CAN%20Board_SCH.pdf) or by measuring with the multimeter.

A knife tip is suitable for removing the SMD resistor. Use it to scratch or pry off the resistance. With the transceiver shown here, place the tip of the knife in front of the resistor (between the chip and the resistor) and support the knife against the chip. Then lever the tip against the resistance and use it to scratch / loosen / break the resistance. Whichever happens first.

![Transceiver](https://github.com/AK-Homberger/NMEA2000WifiGateway-with-ESP32/blob/master/CAN-Transceiver.jpg)

It then looks like the picture. Then measure to be on the safe side. Without a 120 ohm resistor, the multimeter shows approx. 75 kOhm.

# Partlist:

- ESP32 [Link](https://www.amazon.de/AZDelivery-NodeMCU-Development-Nachfolgermodell-ESP8266/dp/B071P98VTG/ref=sxts_sxwds-bia-wc-drs3_0?__mk_de_DE=%C3%85M%C3%85%C5%BD%C3%95%C3%91&cv_ct_cx=ESP32&dchild=1&keywords=ESP32) 
- Buzzer  [Link](https://www.makershop.de/en/module/audio/aktiver-summer/) or [Link2](https://www.conrad.de/de/p/makerfactory-mf-6402168-aktiver-summer-2134056.html)
- SN65HVD230 [Link](https://www.amazon.de/SN65HVD230-Board-Connecting-Communication-Development/dp/B00KM6XMXO/ref=sxts_sxwds-bia-wc-drs1_0?__mk_de_DE=%C3%85M%C3%85%C5%BD%C3%95%C3%91&cv_ct_cx=SN65HVD230&dchild=1&keywords=SN65HVD230&pd_rd_i=B00KM6XMXO&pd_rd_r=0000ea9b-16c8-4bfc-bb40-b71623633214&pd_rd_w=VecN7&pd_rd_wg=VRb2Q&pf_rd_p=578deb70-f9b7-4aa5-9f96-98765f2717c8&pf_rd_r=H8X4ND0GD8MN6WH9H17A&psc=1&qid=1601309172&s=industrial&sr=1-1-5a42e879-3844-4142-9c14-e77fe027c877)
- D24V10F5 [Link](https://eckstein-shop.de/Pololu-5V-1A-Step-Down-Spannungsregler-D24V10F5)
- Resistor 3,3KOhm [Link](https://www.reichelt.de/widerstand-kohleschicht-3-3-kohm-0207-250-mw-5--1-4w-3-3k-p1397.html?search=widerstand+250+mw+3k3) Other resistors are the same type!
- Transistor BC547 [Link](https://www.reichelt.de/bipolartransistor-npn-45v-0-1a-0-5w-to-92-bc-547b-dio-p219082.html?search=BC547)
- PCB universal set [Link](https://www.amazon.de/70Stk-Doppelseitig-Lochrasterplatte-Kit-Lochrasterplatine/dp/B07BDKG68Q/ref=sr_1_6?adgrpid=70589021505&dchild=1&gclid=EAIaIQobChMI07qXtuaN7AIVjentCh3xPg80EAAYASAAEgK_-_D_BwE&hvadid=352809599274&hvdev=c&hvlocphy=9043858&hvnetw=g&hvqmt=e&hvrand=11402952735368332074&hvtargid=kwd-300896600841&hydadcr=26892_1772693&keywords=lochrasterplatine&qid=1601363175&sr=8-6&tag=googhydr08-21)


# Update
Verion 0.3 - 04.08.20: Changed TX pin from 2 to 5 and buzzer pin from 5 to 15 to avoid CAN bus problems during ESP32 reboot/programming.

Verion 0.1 - 28.07.20: Initial version.
