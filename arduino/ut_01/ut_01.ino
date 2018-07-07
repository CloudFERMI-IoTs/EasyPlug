

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

// SPI Lora
#define LORA_RST      23
#define LORA_SS       22
#define LORA_MOSI     5
#define LORA_MISO     17
#define LORA_SCK      18
#define LORA_DIO012   15



int freq = 2400;
int channel = 0;
int resolution = 8;


void IRAM_ATTR handleInterrupt() {
  Serial.printf("External Active\n");  
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

    attachInterrupt(digitalPinToInterrupt(X_5VEXT), handleInterrupt, FALLING);

    // speaker
    ledcSetup(channel, freq, resolution);
    ledcAttachPin(SPK, channel);
    

    Serial.printf("Start testing...\n");
    
    // flash leds in 10s
    for(uint8_t i=0; i<10; i++) {
      if(i%2) {
        digitalWrite(LED1, !digitalRead(LED1));
      } else {
        digitalWrite(LED2, !digitalRead(LED2));
      }
      delay(1000);
    }

    // test RL1
    digitalWrite(RL1, HIGH);
    delay(3000);
    digitalWrite(RL1, LOW);
    
    delay(2000);
    digitalWrite(RL2, HIGH);
    delay(3000);
    digitalWrite(RL2, LOW);

    delay(2000);
    digitalWrite(RL3, HIGH);
    delay(3000);
    digitalWrite(RL3, LOW);

    ledcWriteTone(channel, 2400);
    delay(2000);
    ledcWrite(channel, 0);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(digitalRead(BTN) == LOW) {
    digitalWrite(LED1, !digitalRead(LED1));
    ledcWrite(channel, 127);
    delay(50);
    ledcWrite(channel, 0);
  }
  

}

