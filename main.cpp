/*
 * Implementación preliminar de alcoholimetro 
 *
 */
 //=====[Libraries]=============================================================
#include "mbed.h"
#include "arm_book_lib.h"
//#include <cstdint>

//=====[Definitions]=============================================================
#define BAUD_RATE                            9600         //Velocidad comunicación serial
#define NUMBER_OF_AVG_SAMPLES                10
#define OVER_ALCOHOL_LEVEL                   0.5  
#define BLINKING_TIME_OVER_ALCOHOL_ALARM     500
#define NUMBER_TIME_ADC_UART                 1500
#define TIME_INCREMENT_MS                    10

//=====[Declaration and initialization of public global objects]===============
DigitalIn on_off_button(BUTTON1);                             //Inicializacion Inputs
DigitalIn envio_parada_button(D2);

DigitalOut Encendido(LED1);                                   //Inicializacion Ouputs
DigitalOut Activation_Buzzer(LED2);

AnalogIn mq3(A1);                                             //Inicializacion Analog Inputs

static UnbufferedSerial serial_port(USBTX, USBRX, BAUD_RATE);   //Inicializacion comunicación serial

//=====[Declaration and initialization of public global variables]=============

bool init_system = OFF;                                      // Flag on/off sistema      
bool send = OFF;                                             // Flag enviar/parar UART
bool overAlcoholDetector = OFF;                              // Flag límite alcohol
uint8_t mq3SampleIndex = 0;
float mq3ReadingsArray [NUMBER_OF_AVG_SAMPLES];
float mq3ReadingsSum = 0.0;
static float mq3ReadingsAverage = 0.0;                      // Nivel promedio de alcohol
int accumulatedTimeAlarm = 0;                               // Variables para calcular tiempo acumulado
int accumulatedTimeADCUART = 0;

//=====[Declarations (prototypes) of public functions]=========================
void inputsInit();
/*configura entradas
Digitales 
*/
void outputsInit();
/**<
This function initializes the outputs of the system:
-# Encendido = OFF
-# Activation_Buzzer = OFF
*/
void mq3ReadingsArrayInit();
/**<
This function initializes the mq3ReadingsArray with zeros
*/
bool activation();
/**<
This function initializes all system
*/
bool send_data();
/**<
This function initializes send data 
*/
void ActivationUpdate();
/**<
This function initializes the mq3 lecture
*/
void initUART();
/**<
This function initializes the UART port
*/
void sendAdcUART();
/**<
This function send analogin to serial port
*/



void on_rx_interrupt()
{
    char c;

    // Toggle the LED.
    //led = !led;

    // Read the data to clear the receive interrupt.
    if (serial_port.read(&c, 1)) {
        // Echo the input back to the terminal.
        serial_port.write("#", 1);
    }
}

//=====[Main function, the program entry point after power on or reset]========


int main()
{
    inputsInit();
    initUART();
    outputsInit();
    mq3ReadingsArrayInit();
    //serial_port.attach(&on_rx_interrupt, SerialBase::RxIrq);

    while (true) {
        ActivationUpdate();
        if (activation()){
        
        Encendido = ON;
       
        }else{
        outputsInit();
        }
        if (Encendido && send_data())
        {
            sendAdcUART();
        }
        delay(TIME_INCREMENT_MS);
    }
}
//=====[Implementations of public functions]===================================
void inputsInit()
{
    on_off_button.mode(PullDown);       //Setup input
    envio_parada_button.mode(PullUp);           
    
}

void outputsInit()
{
    Encendido = OFF;
    Activation_Buzzer = OFF;

}

bool activation()
{
    if (on_off_button == ON){ 
       init_system = !init_system;
    }
    return init_system;
}
bool send_data()
{
    if (envio_parada_button == OFF){ 
       send = !send;
    }
    return send;
}


void mq3ReadingsArrayInit()
{
int i;
for( i=0; i<NUMBER_OF_AVG_SAMPLES ; i++ ) {
mq3ReadingsArray[i] = 0;
}
}
void ActivationUpdate()
{
    static int mq3SampleIndex = 0;
    int i = 0;

    mq3ReadingsArray[mq3SampleIndex] = mq3.read();
    mq3SampleIndex++;
    if (mq3SampleIndex >= NUMBER_OF_AVG_SAMPLES) {
        mq3SampleIndex = 0;
    }
    mq3ReadingsSum = 0.0;
    for (i = 0; i < NUMBER_OF_AVG_SAMPLES; i++) {
        mq3ReadingsSum = mq3ReadingsSum + mq3ReadingsArray[i];
    }
    mq3ReadingsAverage = mq3ReadingsSum / NUMBER_OF_AVG_SAMPLES;
    
    if (mq3ReadingsAverage > OVER_ALCOHOL_LEVEL) {
        overAlcoholDetector = ON;
    } else {
        overAlcoholDetector = OFF;
    }
  
    if(overAlcoholDetector) { 
        accumulatedTimeAlarm = accumulatedTimeAlarm + TIME_INCREMENT_MS;                                                                 
    
        if(accumulatedTimeAlarm >= BLINKING_TIME_OVER_ALCOHOL_ALARM) 
        {
                accumulatedTimeAlarm = 0;
                Activation_Buzzer = !Activation_Buzzer;
        }
    } else{
        Activation_Buzzer = OFF;                                
    }
}

void initUART()
{
    serial_port.baud(BAUD_RATE);
    serial_port.format(
        /* bits */ 8,
        /* parity */ SerialBase::None,
        /* stop bit */ 1
    );

}

void sendAdcUART()
{
    char str[40];
    int stringLength;
    
    accumulatedTimeADCUART = accumulatedTimeADCUART + TIME_INCREMENT_MS;
    if (accumulatedTimeADCUART >= NUMBER_TIME_ADC_UART)
    {
        accumulatedTimeADCUART=0;
        sprintf ( str, "mq3_level: %.3f \r\n", float(mq3ReadingsAverage));
        stringLength = strlen(str);
        serial_port.write( str, stringLength );
    }
} 

