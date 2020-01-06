// The native serial support happens via UART
#include <avr/io.h>
#include <compat/twi.h>
#include <Arduino.h>

#define SLA_W 0x18
#define SCL_CLOCK 100000

void I2C_init() {
    
    //TWCR = (1<<TWEN);  // basic init for TWCR
    TWCR=(1<<TWINT)|(1<<TWSTA)|(0<<TWSTO)|(1<<TWEN);
    while (!(TWCR & (1<<TWINT))); // STAGE 2 E 4; start condtion has been transmitted
    if( (TWSR & 0xF8) != (TW_START)){
        printf(" Error  at TWSR start 0x%x\n",TWSR);
    }
    TWDR = SLA_W; // mudar addr
    TWCR = (1<<TWINT)|(1<<TWEN);
    while (!(TWCR & (1<<TWINT)));
    if ((TWSR & 0xF8) != TW_MT_SLA_ACK){
        printf(" Error at TWSR   0x%x    SLA_ACK  0x%x \n", TWSR, TW_MT_SLA_ACK);  // here is the problem !!!!!!!!!  TWSR value should be 0x18
    }
    return;
}

void send_addr(void) {
    TWDR = SLA_W;
    //TWAR = addr; //3
    TWCR = (0<<TWINT)|(1<<TWEA)|(0<<TWSTA)|(0<<TWSTO)|(0<<TWWC)|(1<<TWEN);
    return;
}

void send_data(uint16_t DATA) {
    TWDR = DATA; //5
    TWCR=(1<<TWINT)|(0<<TWSTA)|(0<<TWSTO)|(1<<TWEN); //clear twint bit in twcr to start transmition of data
    while (!(TWCR & (1<<TWINT)));
    if ((TWSR & 0xF8) != TW_MT_DATA_ACK){
        printf(" Error at TWSR   0x%x    SLA_ACK  0x%x \n", TWSR, TW_MT_SLA_ACK);  // here is the problem !!!!!!!!!  TWSR value should be 0x18
    }
    TWCR=(1<<TWINT)|(0<<TWSTA)|(1<<TWSTO)|(1<<TWEN); //transmite sto condition
}

uint8_t get_data(void) {
    TWCR=(1<<TWINT)|(1<<TWEA)|(1<<TWEN);
    //while (!(TWCR & (1<<TWINT)));
    return TWDR;
}

void setup() {
    // put your setup code here, to run once:
    Serial.begin(9600);
    I2C_init();
}

void loop() {
    uint8_t temp=get_data();
    Serial.print(temp,DEC);
    
    delay(250);
}