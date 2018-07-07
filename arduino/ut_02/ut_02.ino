
#include <lmic.h>
#include <hal/hal.h>
#include <SPI.h>
/*
 * IO assignment on board EasyPlug 201806_r1.0
 */
// interrupt pin from power supply
#define X_5V220   35    // GPIO35, input only
#define X_5VEXT   34    // GPIO34, input only

// Led
#define LED1      26    // GPIO26
#define LED2      25    // GPIO25

// Button
#define BTN           39    // SENSOR_VN / GPIO39 / Input only

// Relay
#define RL1       12    // GPIO12
#define RL2       14    // GPIO14
#define RL3       27    // GPIO27

// Speaker
#define SPK       13    // GPIO13

// OSC 32768
#define XTAL_32K_P    32
#define XTAL_32K_N    33

// SPI Lora SX1278
#define LORA_RST      23
#define LORA_SS       22
#define LORA_MOSI     5
#define LORA_MISO     17
#define LORA_SCK      18
#define LORA_DIO012   15


// SPK settings
int freq = 2400;
int channel = 0;
int resolution = 8;


#if !defined(DISABLE_INVERT_IQ_ON_RX)
#error This example requires DISABLE_INVERT_IQ_ON_RX to be set. Update \
       config.h in the lmic library to set it.
#endif

#define TX_INTERVAL 2000
// Pin mapping
const lmic_pinmap lmic_pins = {
    .nss = LORA_SS,
    .rxtx = LMIC_UNUSED_PIN,
    .rst = LORA_RST,
    .dio = {LORA_DIO012, LORA_DIO012, LORA_DIO012},
};

// These callbacks are only used in over-the-air activation, so they are
// left empty here (we cannot leave them out completely unless
// DISABLE_JOIN is set in config.h, otherwise the linker will complain).
void os_getArtEui (uint8_t* buf) { }
void os_getDevEui (uint8_t* buf) { }
void os_getDevKey (uint8_t* buf) { }

void onEvent (ev_t ev) {
}

osjob_t txjob;
osjob_t timeoutjob;
static void tx_func (osjob_t* job);

// Transmit the given string and call the given function afterwards
void tx(const char *str, osjobcb_t func) {
  os_radio(RADIO_RST); // Stop RX first
  delay(1); // Wait a bit, without this os_radio below asserts, apparently because the state hasn't changed yet
  LMIC.dataLen = 0;
  while (*str)
    LMIC.frame[LMIC.dataLen++] = *str++;
  LMIC.osjob.func = func;
  os_radio(RADIO_TX);
  Serial.println("TX");
}

// Enable rx mode and call func when a packet is received
void rx(osjobcb_t func) {
  LMIC.osjob.func = func;
  LMIC.rxtime = os_getTime(); // RX _now_
  // Enable "continuous" RX (e.g. without a timeout, still stops after
  // receiving a packet)
  os_radio(RADIO_RXON);
  Serial.println("RX");
}

static void rxtimeout_func(osjob_t *job) {
  digitalWrite(LED2, HIGH);    // OFF
}

static void rx_func (osjob_t* job) {
  // Blink once to confirm reception and then keep the led on
  digitalWrite(LED2, HIGH);   // OFF
  ledcWrite(channel, 127);
  delay(200);
  digitalWrite(LED2, LOW);    // ON
  ledcWrite(channel, 0);

  // Timeout RX (i.e. update led status) after 3 periods without RX
  os_setTimedCallback(&timeoutjob, os_getTime() + ms2osticks(3*TX_INTERVAL), rxtimeout_func);

  // Reschedule TX so that it should not collide with the other side's
  // next TX
  os_setTimedCallback(&txjob, os_getTime() + ms2osticks(TX_INTERVAL/2), tx_func);

  Serial.print("Got ");
  Serial.print(LMIC.dataLen);
  Serial.println(" bytes");
  Serial.write(LMIC.frame, LMIC.dataLen);
  Serial.println();

  // Restart RX
  rx(rx_func);
}


static void txdone_func (osjob_t* job) {
  rx(rx_func);
}

// log text to USART and toggle LED
static void tx_func (osjob_t* job) {
  // say hello
  tx("Hello, world from EasyPlug 1!", txdone_func);
  // reschedule job every TX_INTERVAL (plus a bit of random to prevent
  // systematic collisions), unless packets are received, then rx_func
  // will reschedule at half this time.
  os_setTimedCallback(job, os_getTime() + ms2osticks(TX_INTERVAL + random(500)), tx_func);
}

void setup() {
    Serial.begin(115200);

    pinMode(LED1, OUTPUT);
    pinMode(LED2, OUTPUT);
    pinMode(RL1, OUTPUT);
    pinMode(RL2, OUTPUT);
    pinMode(RL3, OUTPUT);

    pinMode(BTN, INPUT);
    pinMode(X_5V220, INPUT);
    pinMode(X_5VEXT, INPUT);

    pinMode(LORA_RST, OUTPUT);
    digitalWrite(LORA_RST, LOW);
    delay(10);
    digitalWrite(LORA_RST, HIGH);

  
    // initialize runtime env
    os_init();
    // Set up these settings once, and use them for both TX and RX
    LMIC.freq = 433000000;

    // Maximum TX power
    LMIC.txpow = 27;
    // Use a medium spread factor. This can be increased up to SF12 for
    // better range, but then the interval should be (significantly)
    // lowered to comply with duty cycle limits as well.
    LMIC.datarate = DR_SF9;
    // This sets CR 4/5, BW125 (except for DR_SF7B, which uses BW250)
    LMIC.rps = updr2rps(LMIC.datarate);

    // setup initial job
    os_setCallback(&txjob, tx_func);

    // speaker
    ledcSetup(channel, freq, resolution);
    ledcAttachPin(SPK, channel);
    

    Serial.printf("Start testing...\n");
    
    // test Leds, flash leds in 10s
    for(uint8_t i=0; i<10; i++) {
      if(i%2) {
        digitalWrite(LED1, !digitalRead(LED1));
      } else {
        digitalWrite(LED2, !digitalRead(LED2));
      }
      delay(300);
    }

    // test SPK
    ledcWriteTone(channel, 2400);
    delay(2000);
    ledcWrite(channel, 0);
}

void loop() {
  // process Lora  
  os_runloop_once();
  
  if(digitalRead(BTN) == LOW) {
    digitalWrite(LED1, !digitalRead(LED1));
    ledcWrite(channel, 127);
    delay(50);
    ledcWrite(channel, 0);
  }
  

}

