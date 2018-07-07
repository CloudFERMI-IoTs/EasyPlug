# EasyPlug
Sample codes for EasyPlug Lora Actuator Nodes

Actuator Node with Lora / Wifi / Bluetooth. This is pure hardware platform, sold to public domain for developing commercial IoTs products by our partners.

==========
*** TECHNICAL SPECS

1. High quality, luxury Cover / Power Socket from Panasonic

2. Based on ESP32 chip

3. Support 03 power sockets with 03 relays 250V/10A (max). When using, should use half for long-term operation. Total max input current must be about 20A/220VAC.

4. Support 02 LOAD power supplies: 220VAC system and DC. You could use this to control 24VDC loads.

5. Support 02 power supply inputs for LOGIC CIRCUIT operation: 5V from 220VAC system, and 5V from external DC power supply with micro-USB connector.

6. Support Wifi b/g/n, Bluetooth / BLE 4.2

7. Support Lora SX1278 433MHz with antenna inside its cover

8. Support 01 piezo speaker 

9. Support 01 button

10. Support 02 user leds, and 1 power led

11. Interface debug/auto programming port (TXD, RXD, DTR, RTS) (header 2.54mm)

12. Support 04 GPIOs with built-in pull-up resistor (10k) for 2 GPIO (header 2.54mm)

13. Support RTC crystal 32.768KHz

14. Support 02 interrupt sources from 2 logic power supplies to detect presence or not

15. There's no internal fuse protection for loads. So please use CB to protect your systems.

16. Should use Temp sensor inside ESP32 to protect over thermal condition.
