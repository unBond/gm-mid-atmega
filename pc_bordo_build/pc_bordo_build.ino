//Projeto computador de bordo alternativo ao MID gm  - unBond
//27/10/2019 14:18 - ajustes nas inicializacao das variaveis pos falhas. 
//Incluir URL antes de compilar no arduino IDE
//https://mcudude.github.io/MightyCore/package_MCUdude_MightyCore_index.json

/*
Principais créditos 

1284p datasheet
ww1.microchip.com/downloads/en/DeviceDoc/doc8059.pdf

Possibilita usar o atmega1284p na IDE do arduino
Autor: MightyCore 
https://mcudude.github.io/MightyCore/

tempOut(), dimmerSensor(), tankSensor(), tempMotor() 
Autor: Fire1 - ArduinoMid shield atmega2560
https://github.com/fire1/ArduinoMid

Configuração do timer a cada 250ms, interrupções, VSS e velocidade
Autor: grzeeezzz
Baseado no video tutorial : https://www.youtube.com/watch?v=9uLg3KEshto
Leitura complementar: http://gammon.com.au/interrupts

Esqueleto/lógica dos menus: 
Autor: Eng. Wagner Rambo  
Lógica inspirada no video tutorial: Curso de Arduino e AVR 134: https://www.youtube.com/watch?v=tRoznRBYQHA

Funções de economia de energia/SleepMode
Autor: Make cource
Inspirado no video tutorial https://www.youtube.com/watch?v=HiAbxSO_9nU e óbvio na documentação oficial do microcontrolador

Funções de data/Hora e biblioteca com mod de cálculo de data futura 
Autor: vários users conforme o link:
https://github.com/adafruit/RTClib/pull/68

Algumas partes são de outros autores e inúmeras pesquisas que fiz, que acabei esquecendo de anotar para dar os devidos créditos.
O restante é de minha autoria

Não me responsabilizo por qualquer dano ou prejuízo que este venha causar.
Use por sua conta e risco. Sempre consulte um engenheiro/projetista profissional.
*/

//------------------------------------------------------------------------------------
// Biblioteca
//------------------------------------------------------------------------------------
#include <avr/sleep.h> //Hibernacao
#include <Wire.h>      //rtc
#include "RTClib.h"
//------------------------------------------------------------------------------------
// Debug Geral -- Descomente para ativar o modo debug via serial
//------------------------------------------------------------------------------------
//#define DEBUG_TEMPERATURE_OU
//#define DIM_SENS_DEBUG
//#define DEBUG_TANK_TEMP
//#define DEBUG_ENG_TEMP
//#define DEBUG_SENSORS
//#define DEBUG_LAMP_FREIO
//#define POS_CHAVE_DEBUG

//------------------------------------------------------------------------------------
// RTC
//------------------------------------------------------------------------------------

DS1307 rtc;

//char daysOfTheWeek[7][12] = {"Dom", "Seg", "Ter", "Qua", "Qui", "Sex", "Sab"};
DateTime dataAtual (2018, 06, 1, 8, 0 , 0);

//String semana = "";
//String timeAtual = "", dateAtual = "";

//------------------------------------------------------------------------------------
// Temperatura Externa - 2
//------------------------------------------------------------------------------------
unsigned long temperatureOutCollection = 0;
uint8_t CUR_OUT_TMP = 0;
uint16_t temperatureOutIndex = 0;

//------------------------------------------------------------------------------------
// Tensão pós-chave - 5 //31
//------------------------------------------------------------------------------------
//unsigned int
volatile float tensao;
volatile uint16_t tensaoPin;
volatile float buffer;

//------------------------------------------------------------------------------------
//  RPM - 6
//------------------------------------------------------------------------------------
volatile unsigned long rpm_pulse_count, rpm_lvl = 0;

//------------------------------------------------------------------------------------
//  Dimmer Sensor - 7
//------------------------------------------------------------------------------------
uint16_t backLightLevel = 100, temp_backLightLevel = 1;

//------------------------------------------------------------------------------------
// Variaveis de VSS 12 e Consumo 27
//------------------------------------------------------------------------------------

// Antes de prosseguir verifique no seu mid qual é o valor dos pulsos. Confira também no painel, qual o valor dos pulsos que está lá.
// Ambos os valores devem ser iguais.
// Se tiver usando um aro diferente do original, use o valor do painel. Mas lembre-se que não terá precisão.
// Se quiser precisão ignone o valor do painel e calcule o valor do vss com os arquivos auxiliares do projeto
// Pulsos do astra elite sedan aro 16 com pneu 205/55/16 conforme mid original por 1km = 14972 Painel = 14972
// representacao do vss em 100 metros 14972/10
volatile unsigned int vss = 14972;
// esse valor é a divisão de de 1/14972
volatile float pulse_distance = 0.00006679134368896484;
//0.00006679134
//0.0000667913438418
//0.00006679134368896484375
volatile float traveled_distance;
volatile unsigned long avg_speed_divider = 0, dist_pulse_count, all_speed_values;

//------------------------------------------------------------------------------------
// Variaveis do menu de inspecao
//------------------------------------------------------------------------------------
//Período troca de óleo default 5 mil km/ 6 meses - 182 dias
volatile uint16_t oil_change = 5000; //teste
volatile float oil_run = 0;
volatile boolean oil_insp, oil_insp_sen = 1;


//Variáveis de troca usadas para manipular/alterar data/hora.
volatile uint16_t ano = 2018;
volatile uint8_t mes = 1;
volatile uint8_t dia = 1;
volatile uint8_t hora = 8;
volatile uint8_t minuto = 0;


//------------------------------------------------------------------------------------
// Variáveis de Sinal Consumo e Consumo médio - 27
//------------------------------------------------------------------------------------
// vazao dos bicos do astra por segundo, com base em uma hora, use os arquivos auxiliares para calcular.
#define ref_mid_cons  1.1 //valor base para o veículo parado, temperatura de trabalho estabilizada.
volatile unsigned int ref_mid_pulse = 554; //valor preliminar sujeito a calibração.
//volatile long static double injection_value = 0.00000834851244687310;
volatile long double injection_value = 0.00000827316486161252; 
volatile double used_fuel, instant_fuel_consumption, display_instant_fuel_consumption;
volatile float all_fuel_consumption_values, average_fuel_consumption;
volatile unsigned long avg_fuel_divider = 0,  temp_injector_pulse_time, pwm_value, prev_time;
volatile unsigned int speed_temp, speed, avg_speed, max_speed = 0, save_counter, cemMetros, autonomia_km;

//------------------------------------------------------------------------------------
// Falha Oleo - 13
//------------------------------------------------------------------------------------
volatile boolean lvl_oil = 0, lvl_oil_sen = 0;

//------------------------------------------------------------------------------------
// Pedal de Freio - 15
//------------------------------------------------------------------------------------
volatile boolean brake_light_test = 1;

//------------------------------------------------------------------------------------
// Falha Fusivel Freio - 16
//------------------------------------------------------------------------------------
volatile boolean brake_fuse = 0;

//------------------------------------------------------------------------------------
// Falha Farol/Lanterna Freio - 17
//------------------------------------------------------------------------------------
volatile boolean lant_p1 = 0, lant_p1_sen = 1;

//------------------------------------------------------------------------------------
// Falha Luz de Freio - 18
//------------------------------------------------------------------------------------
volatile boolean freio_p1 = 0, freio_p1_sen = 1;
volatile int indexFreioTemp = 0, smoothFreioTemp = 0, valFreio = 0;
volatile uint8_t freio_p1_rate = 102; //valor de ajuste da detecção de falha lampada de freio

//------------------------------------------------------------------------------------
// Falha Pastilha freios - 19
//------------------------------------------------------------------------------------
volatile boolean brake_pad = 0, brake_pad_sen = 0;

//------------------------------------------------------------------------------------
// Nivel do Arrefecimen - 20
//------------------------------------------------------------------------------------
volatile boolean lvl_coolnt = 0, lvl_coolnt_sen = 1;

//------------------------------------------------------------------------------------
// Falha Cambio AT - 21
//------------------------------------------------------------------------------------
volatile boolean trans_status = 0, trans_status_sen = 0;
//------------------------------------------------------------------------------------
// Falha Lavador - 22
//------------------------------------------------------------------------------------
volatile boolean lvl_wash = 0, lvl_wash_sen = 1;

//------------------------------------------------------------------------------------
// Botoes alavanca - 23 - 24
//------------------------------------------------------------------------------------
//volatile boolean botao_reset, botao_set;
//variaveis relativas ao menus e respectivas teclas de navegacao - Nao esquecer de modificar conforme o numero de menus aumenta
volatile char indiceMenu = 1, indiceSubMenu = 1;
//No caso totalMenu é o número de menu navegáveis. Os outroa acima desses só são acessíveis por dentro desses 10 menus!
volatile char totalMenu = 11, totalSubMenu;
volatile boolean  menu_flag  = 0, enter_flag = 0, inSubMenu = 0, sub_menu = 0, submenu_once = 0;
volatile char indiceMenuOld = 0;

//------------------------------------------------------------------------------------
// Sinal Nivel Tanque - 26
//------------------------------------------------------------------------------------
volatile int indexTankTemp, smoothTankTemp, val_tnk;
volatile uint8_t CUR_TNK = 0, CUR_TNK_OLD = 0, tank_capacity = 52;

//------------------------------------------------------------------------------------
// Sinal temp motor - 31
//------------------------------------------------------------------------------------
volatile int indexEngineTemp, smoothEngineTemp, val;
volatile uint8_t CUR_ENT = 0, MAX_CUR_ENT = 0;
volatile boolean temp_eng_high = false;

//------------------------------------------------------------------------------------
// Auxiliares e contadores
//------------------------------------------------------------------------------------
volatile boolean to_shutdown = 1, to_checkdate = 1;
volatile unsigned long counter;
volatile unsigned int min15 = 0, min5 = 0, min2 = 0, halfmin = 0;
volatile byte falhas, contadorFalhas, retorno_menu_1, retorno_menu_2;

//------------------------------------------------------------------------------------
// Entradas de sinais - microcontrolador - plug 32 vias carro mid
//------------------------------------------------------------------------------------
#define temp_ext_pin 25             //Sinal temperatura externa = 2  // Implementado
#define pos_chave_pin 36              //Pos chave Pino            = 4 // Implementado
#define rpm_pin 2                  //Sinal RPM Pino            = 6  // Implementado
#define lvl_dimmer_pin 27           //Sinal Dimmer              = 7  // Implementado
#define vss_pin 10                   //Sinal VSS                 = 12 // Implementado
#define lvl_oil_pin 1              //Falha Oleo                = 13 // Implementado -
#define brake_light_pin 3          //Falha Luz de Freio        = 15 // Implementado
#define brake_fuse_pin 4             //Falha Fusivel Freio       = 16 // Implementado+
#define lant_p1_pin 20              //Modulo luzes #1 - lanterna = 17 // Implementado
#define freio_p1_pin 29              //Modulo Luzes #2 - luz de freio = 18 // Implementado
#define brake_pad_pin 23              //Falha Pastilha freios     = 19 // Implementado -
#define lvl_coolnt_pin 12            //Falha Arrefecimento       = 20 // Implementado -
#define trans_pin 22                  //Falha Cambio AT           = 21 // Implementado -
#define lvl_wash_pin 21               //Falha Lavador             = 22 // Implementado -
#define btn_reset_pin 14             //Botao Reset               = 23 // Implementado
#define btn_set_pin 13               //Botao Set                 = 24 // Implementado
#define lvl_tank_pin 26             //Sinal Nivel Tanque        = 26 // Implementado
#define injector_pin 11              //Sinal injetores           = 27 // Implementado
#define temp_engine_pin 24          //Sinal temp motor          = 31 // Implementado

//------------------------------------------------------------------------------------
// beep
//------------------------------------------------------------------------------------
#define buzzer 15          //buzzer
volatile boolean one_beep = false;
//------------------------------------------------------------------------------------
// Cronometro
//------------------------------------------------------------------------------------
volatile uint8_t cronSeg = 0, cronMin = 0, cronHor = 0;
volatile char cronStatus = 0;

//--------- SETUP ---------//
void setup() {
    noInterrupts(); //Pausa nas interrupções para setup do micro-controlador
    analogReference(EXTERNAL); //cuidado
    Serial.begin(9600);
    /* while (!Serial) {; // wait for serial port to connect. Needed for native USB port only
     }*/

    
    TCCR1A = 0;
    TCCR1B |= ((1 << CS10) | (1 << CS11));
    TIMSK1 |= (1 << TOIE1);
    TCNT1 = 3036;





//---Definicao dos pinos no microcontrolador

    pinMode(temp_ext_pin, INPUT);           //Sinal temperatura externa = 2
    pinMode(pos_chave_pin, INPUT);          //Pos chave Pino            = 4
    pinMode(rpm_pin, INPUT);                //Sinal RPM Pino            = 6
    pinMode(lvl_dimmer_pin, INPUT);         //Sinal Dimmer              = 7
    pinMode(vss_pin, INPUT);                //Sinal VSS                 = 12
    pinMode(lvl_oil_pin, INPUT_PULLUP);     //Falha Oleo                = 13
    pinMode(brake_light_pin, INPUT);        //Falha Luz de Freio        = 15
    pinMode(brake_fuse_pin, INPUT_PULLUP);  //Falha Fusivel Freio       = 16
    pinMode(lant_p1_pin, INPUT_PULLUP);    //Modulo luzes #1           	= 17
    pinMode(freio_p1_pin, INPUT);          //Modulo Luzes #2           	= 18
    pinMode(brake_pad_pin, INPUT_PULLUP);   //Falha Pastilha freios     = 19
    pinMode(lvl_coolnt_pin, INPUT_PULLUP);  //Falha arrefecimento       = 20
    pinMode(trans_pin, INPUT_PULLUP);       //Falha Cambio AT           = 21
    pinMode(lvl_wash_pin, INPUT_PULLUP);    //Falha Lavador             = 22
    pinMode(btn_reset_pin, INPUT);          //Botao Reset               = 23
    pinMode(btn_set_pin, INPUT);            //Botao Set                 = 24
    pinMode(lvl_tank_pin, INPUT);           //Sinal Nivel Tanque        = 26
    pinMode(injector_pin, INPUT);           //Sinal injetores           = 27
    pinMode(temp_engine_pin, INPUT);        //Sinal temp motor          = 31

    //LED
    pinMode(0, OUTPUT);
    digitalWrite(0, HIGH);
//------------------------------------------------------------------------------------
// Fim Entradas de sinais - microcontrolador - plug 32 vias carro mid
//------------------------------------------------------------------------------------

//Interrupcoes, rpm, distancia, consumo
    attachInterrupt(digitalPinToInterrupt(2), rpm_count, RISING);				//INT2
    attachInterrupt(digitalPinToInterrupt(10), distance_count, FALLING); 		//INT0
    attachInterrupt(digitalPinToInterrupt(11), rising, RISING);					//INT1
    interrupts();  //Todas configurações concluídas, início das interrupções.

//--- RTC
#ifdef AVR
  Wire.begin();
#else
  Wire1.begin(); // Shield I2C pins connect to alt I2C bus on Arduino Due
#endif
rtc.begin();
rtc.adjust(DateTime(dataAtual));
 /*
  Serial.begin(57600);
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  if (! rtc.isrunning()) {
    Serial.println("RTC is NOT running!");
    // following line sets the RTC to the date & time this sketch was compiled
    // rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }
*/  

//---fim RTC

}
//--------- Fim Setup ---------//

//--------- LOOP ---------//
void loop() {
 
    if (tensao > 7) {
	dataAtual = rtc.now();// horario - jamais chamar dentro do interrupt	
        if (to_shutdown == 1) {                // Funcao de telas de boas vindas
            delay(1500);
            sendEsc();
            Serial.print("page 2");
            sendEsc();
            delay(1500);
            to_shutdown = 0;
        } else {

            if (brake_light_test == 1) { //Aguardamos sinal pedal de freio e encerramento tela de boas vindas
                retorno_menu_1 = 0;               //Mudança de page no display
                if (retorno_menu_2 == 0) {
                    // Funcao de entrada menu de falhas...
                    Serial.print("page 1");
                    sendEsc();
                    retorno_menu_2 = 1;
                }
                writeMenuF("24", "Pressione", "Freio!");       //Envio da mensagem
                if (digitalRead(brake_light_pin) == HIGH) {   //Se pedal é pressionado
                    brake_light_test = 0;           //Saimos da rotina de testes
                }
            } else {                      //Inicio rotina padrao do display - depois verificar se o tempo tá ok
                if(falhas == 1 && contadorFalhas < 55 && indiceMenu < 10) {   //Se houver falhas e nao estivermos no menu de configuracoes chamamos o menu de falhas
                    retorno_menu_1 = 0;
                    //reset envio imagens.
                    indiceMenuOld = 0;
                    falhas_display();
                } else {                    //Senão, exibição padrão do menu
                    retorno_menu_2 = 0;
                    menu_display();
                }
            }
        }
    } else {
        if (to_checkdate == 1) {
			//gambiarra pra nao chamar dentro do loop, somente uma vez quando for desligar

			to_checkdate = 0;
		}	
    }
    
}
//--------- FIM LOOP ---------//

//--------- rtc ---------//
/*
void dataEhora(uint8_t x) {
	if (x == 1){
	DateTime now = rtc.now();
	ano = now.year();
	mes = now.month();
	dia = now.day();
	hora = now.hour();
	minuto = now.minute();
	}
	if (x == 2) {
		
	ano = dataFuturaOleo.year();
	mes = dataFuturaOleo.month();
	dia = dataFuturaOleo.day();
	}	
	//semana = (String) daysOfTheWeek[now.dayOfTheWeek()];
	//timeAtual = String(now.hour(),DEC) + ":"+String(now.minute(),DEC);
	//dateAtual = ((String) daysOfTheWeek[now.dayOfTheWeek()] + "-" + String(now.day(),DEC) + "." + String(now.month(),DEC) + "." +  String(now.year(),DEC)); //+String(now.second(),DEC
}
*/
//--------- FIM rtc ---------//


//--------- Timer principal ----------------//

ISR(TIMER1_OVF_vect) { 
	//--------- INICIO ROTINA 250ms ---------//
    counter++;
	//
    temp_injector_pulse_time = 0;
    rpm_lvl = (rpm_pulse_count * 4 * 30.5); // RPM - multiplicado por 4 pois essa rotina representa 250ms 30 é o fator aproximado..
    rpm_pulse_count = 0; //Zera para nova medição.
    freio_p1Sensor(); //Rotina da verificação da luz de freio.

    //Se não temos consumo, nem velocidade, nem pos-chave, significa que o carro está desligado, preparar pra acionar sleepSetup() em 30 segundos;
    if (!(instant_fuel_consumption > 0) && speed == 0 && (tensao < 7)) {
        save_counter++;
        freio_p1 = false;
        //reset envio imagens.
        indiceMenuOld = 0;
        //reset para pagina de falhas
        retorno_menu_2 = 0;
        //Novo teste do pedal
        brake_light_test = 1;
        //reset para chamar abertura da tela
        to_shutdown = 1;
        if (indiceMenu > 10) {
            indiceMenu = 1; //caso tenha desligado no menu não padrão, voltaremos ao inicio
            indiceSubMenu = 0;
            //Serial.print("page 0");
            //sendEsc();
        }
    }
    /* Mecanismo Hibernacao / Salvamento
    *
    *
    * Se atingir 30 e velocidade == 0 e sem pulsos dos injetores acionar procedimento sleepSetup()
    * Na primeira versão tive problemas para salvar na eeprom, por isso resolvi fazer como o mid original funciona, no qual ele fica permanentemente ligado. Motivo pelo qual há
	* duas fontes de alimentação.
    * Talvez na versão final, com a possível adição do relógio RTC, ele tenha que funcionar assim. Mas para fins de desenvolvimento, iremos colocar o micro-controlador
    * em hibernação, o que já trás uma economia significativa da bateria, sem menor prejuízo, Assim que detectado o primeiro pulso o micro-controlador é acordado
    * e reativado as funções de pin_interrupt sem prejuízo nos cálculos.
    *
    * Vale notar que quando estava desenvolvendo usando um arduino uno, eu não tinha um circuito específico pra detectar o pós-chave. Então na ausência de pulsos por 30
    * segundos o micro-controlador iria hibernar. Na versão final estarei usando como critério o pino pós chave, qualquer uma das duas lógicas funcionam muito bem
    */

    /*
    	//Desliga o micro-controlador na ausencia de pulsos em 30p
    	if (save_counter > 30 && speed == 0 && to_shutdown == 1) {
    		sleepSetup();
    	}

    	01/07/2018 Usando 30*250ms - 7 segundos - virada rapida de chave.
		19/12/2018 Usando 30*250ms - 30 segundos - virada rapida de chave.
		27/10/2019 120 * 0.250ms = 30 segundos aciona sleep
    */
    if (save_counter > 120 && speed == 0) {
        sleepSetup();
    }
	//--------- FIM ROTINA 250ms ---------//
    
	//--------- INICIO ROTINA 1s ---------//
    if (counter > 3) { //Aqui se passou 1 segundo 4x250ms !!!!!!!!!!!!!!!IMPORTANTE!!!!!!!!!!!!!!

        //fucoes constantes - A cada 1 seg.
        tempOut();			        // Temperatura externa
        tempMotor(); 	          // Temperatura motor
        dimmerSensor(); 	      // Dimmer
        posChaveSen();    	    // Pós - chave
        lant_p1Sensor();		// Verifica lanternas.
        centralFalhas();  	    // Central de falhas
        //
        //if (speed_temp == 0) {  // a cada 1 s. teste..
            tankSensor();         // Tanque
        //}

        //Contadores de 15, 5, e 2 minutos para as funcoes de rotina.
        min15++;
        min5++;
        min2++;
		halfmin++;
        if (min15 >= 900) {
            intervalo15();
            min15 = 0;
        }
        if (min5 >= 300) {
            intervalo5();
            min5 = 0;
        }
        if (min2 >= 120) {
            intervalo2();
            min2 = 0;
        }
        if (halfmin >= 30) {
            intervalohalfmin();
            halfmin = 0;
        }		
        cronometro(); 		    //Cronometro
        currentSpeed(); 	    //Velocidade
        fuelConsumption(); 	  //Consumo Combustivel

        //Devemos reduzir a velocidade média e o consumo médio se o carro tiver parado porém consumindo combustível, portanto validar
        //if (speed == 0 && instant_fuel_consumption > 0.3) <-- validar!
        if (speed > 1 || instant_fuel_consumption > 0.3) {
            avg_speed_divider++;
            avg_fuel_divider++;
            avgSpeed(); //O Cálculo velocidade média Só é atualizado em movimento.
            maxSpeed(); //Velocidade máxima
        }

        dist_pulse_count = 0;
        pwm_value = 0;
		counter = 0;

        //Entrega dos resultados
        //Arredondar o valor para 1 casa decimal
        display_instant_fuel_consumption = floorf(instant_fuel_consumption * 10) / 10;
        //display_traveled_distance =  floorf(traveled_distance * 10) / 10;
        speed_temp = speed; //variavel temporaria pra evitar erros estranhos
	//--------- FIM ROTINA 1s ---------//
	}
    TCNT1 = 3036;
}
//--------- Fim Timer principal ----------------//

//--------- Intervalo 30 sec ----------------//
void intervalohalfmin(){
	lvlCoolntSensor();
  
}
//--------- Fim Intervalo 30 sec ----------------//

//--------- Intervalo 2 minutos ----------------//
void intervalo2() {
    one_beep = false; //Variável para evitar muitos beeps de aviso, quando ouver algum erro, afim de evitar distrações.
    lvlOilSensor();
    brakeFuseSensor();
}
//--------- Fim Intervalo 2 minutos ----------------//

//--------- Intervalo 5 minutos ----------------//
void intervalo5() {
  transSensor();
	autonomia();
}
//--------- Fim Intervalo 5 minutos ----------------//

//--------- Intervalo 15 minutos ----------------//
void intervalo15() {
  inspOil();
	lvlWashSensor();
  brakePadSensor();
    //inspOil();
}
//--------- Fim Intervalo 15 minutos ----------------//

//--------- Hibernacao ---------//

void sleepSetup() {
    //Serial.println("dormi");
    //Serial.flush();
    //reset leitura freio
	//resetar leitura de falhas do freio.
    freio_p1 = false;
	valFreio = 0;
	indexFreioTemp = 0;

    //reset envio imagens.
    indiceMenuOld = 0;

    //reset dos intervalos
    //min15 = 0;
    //min5 = 0;
    min2 = 0;
	halfmin = 0;
    //reset para pagina de falhas
    retorno_menu_2 = 0;
    //teste pra evitar km a mais 29/07/2018
	//if (cemMetros > 0) cemMetros--;
	//Desliga led
    digitalWrite(0, LOW);
	//
    set_sleep_mode(SLEEP_MODE_PWR_DOWN); // sleep mode is set here
    detachInterrupt(digitalPinToInterrupt(11)); // stop LOW interrupt on D2
    noInterrupts();
    sleep_enable(); // enables the sleep bit in the mcucr register
    attachInterrupt(digitalPinToInterrupt(11), wake, CHANGE); // use interrupt 0 (pin 2) and run function - pino usado pra acordar o micro-controlador
    interrupts();
    sleep_cpu();
    //resume
}

//--------- Retorno da hibernacao ---------//

void wake() {
    sleep_disable(); // first thing after waking from sleep:
    detachInterrupt(digitalPinToInterrupt(11)); // stop LOW interrupt on D2
    save_counter = 0; //reset savecounter
    digitalWrite(0, HIGH); //liga o led
	//reconfigura interrupões vss e consumo
    attachInterrupt(1, falling, FALLING); 
    attachInterrupt(0, distance_count, FALLING);
    //Serial.println("acordar");
    //Serial.flush();

    //prevenir pulso a cada virada de chave
   // if (cemMetros > 0) cemMetros--;
    //Rechecagem dos sensores
    delay(500); //esperando capacitores estabilizarem pra evitar leituras malucas? aparentemente funcionou!

    lvlCoolntSensor();
    lvlOilSensor();

    brakeFuseSensor();
    brakePadSensor();
    lvlWashSensor();
	//autoriza update na data futura
	to_checkdate = 1;

}

//--------- Cronometro ----------------//
void cronometro() {
    if (cronStatus == 1) {
        cronSeg++;
        if(cronSeg == 60) {
            cronSeg = 0;
            cronMin++;
            if (cronMin == 60) {
                cronHor++;
                cronMin = 0;
                if (cronHor == 24) {
                    cronHor = 0;
                    cronMin = 0;
                    cronSeg = 0;
                }
            }
        }
    }

}
//--------- Fim Cronometro ----------------//

//--------- Central de Falhas ---------//
void centralFalhas() {
    //Se qualquer uma dessa variáveis estiver true, e estivermos com pos-chave, existe uma falha, e se aciona a reserva do display para apresentar as falhas.
    if ((lvl_oil == true || brake_fuse == true || brake_pad == true || lvl_coolnt == true || trans_status == true || lvl_wash == true || oil_insp == true || freio_p1 == true || lant_p1 == true || temp_eng_high == true) && tensao > 8) {
        falhas = 1;       //Temos uma falha
        contadorFalhas++; //Criamos um contador de 60 segundos, nesses 60 segundos o display sera reservado pra exibir falhas
    } else {
        falhas = 0;
        contadorFalhas = 0;
    }
    if (contadorFalhas == 60) contadorFalhas = 0;
}
//--------- Fim Central de Falhas ---------//

//--------- Funcoes de Reset ---------//
void fullReset() {
    zeraConsumo();
    zeraUsedFuel();
    zeraDistancia();
    zeraAvgSpeed();
    zeraTopSpeed();
    zeraAuton();
}
void zeraConsumo() { //Combustivel
    all_fuel_consumption_values = 0;
    avg_fuel_divider = 0;
    instant_fuel_consumption = 0;
    average_fuel_consumption = 0;
    pwm_value = 0;
    prev_time = 0;
    temp_injector_pulse_time = 0;
    /* Zera leitura do tanque */
    indexTankTemp = 0, smoothTankTemp = 0, CUR_TNK = 0;
}
void zeraUsedFuel() { // Consumo Total Combustivel
    used_fuel = 0;
    /* Zera leitura do tanque */
    indexTankTemp = 0, smoothTankTemp = 0, CUR_TNK = 0;
}
void zeraDistancia() { //zera Vss
    dist_pulse_count = 0;
    cemMetros = 0;
    traveled_distance = 0;
}
void zeraAvgSpeed() {
    all_speed_values = 0;
    avg_speed_divider = 0;
    avg_speed = 0;
}

void zeraTopSpeed() {
    max_speed = 0;
}
void zeraAuton() {
    autonomia_km = 0;
}
void zeraCron() {
    cronStatus = 0;
    cronHor = 0;
    cronMin = 0;
    cronSeg = 0;
}
//--------- Fim Funcoes de Reset ---------//

//--------- Temperatura externa - 2 ---------//
void tempOut() {
    /******************************* Car's temperature sensor ******************************************/
    float temperatureC;
    /**
     * About GM Temperature sensor
     *      Temperature range to [°C]: 250
     *      Resistance [Ohm]: 5000
     * https://www.hackster.io/Marcazzan_M/how-easy-is-it-to-use-a-thermistor-e39321
     * ~ 40     °C      value 117
     * ~ 41     °C      value 120
     * ~ 36     °C      value 136
     * ~ 23+    °C      value 203 <- inside garage
     * ~ 22     °C      value 220
     * ~ 19     °C      value 225 <- hot engine
     * ~ 16     °C      value 238
     * ~ 20     °C      value 226
     * ~ 11.1   °C      value 281
     * ~ 8 (14) °C      value 295 <- guess
     * ~  15    °C      value 293 <- guess
     */
    uint16_t liveTemperatureValue;
    temperatureOutCollection += liveTemperatureValue = (uint16_t) analogRead(temp_ext_pin);
    temperatureOutIndex++;
    if (temperatureOutIndex >= 10) {
        //
        // Get more precise average value
        uint16_t readings = uint16_t(double(temperatureOutCollection / temperatureOutIndex) * 10);

        // (map(readings, 4100, 1200, 15, 390) * 0.1)
        // (map(readings, 2810, 1170, 160, 405) * 0.1) <- use this corrected to 16Ã‚Â°C
        // temperatureC = (map(readings, 2810, 1170, 167, 403) * 0.1);
        temperatureC =(map(readings, 3440, 1170, 94, 395) * 0.1);
        temperatureOutCollection = (readings * 3) / 10;
        temperatureOutIndex = 2;


#if defined(DEBUG_TEMPERATURE_OU)
        Serial.print("Read Temp |  smooth: ");
        Serial.print(readings);
        Serial.print(" / live: ");
        Serial.print(liveTemperatureValue);
        Serial.print(" | calculation:");
        Serial.println(temperatureC);
#endif

        //
        // Pass value to global
        CUR_OUT_TMP = temperatureC;
    }
}//--------- Fim temperatura externa - 2 ---------//

//--------- Pos chave - 5 ---------//
void posChaveSen() {
    float R1 = 100000;
    float R2 = 51000;
    float vref = 5.0; //5.48 correcao
    //
    //https://www.gammon.com.au/adc
    // float buffer = ((float) tensaoPin  + 0.5 ) / 1024.0 * Vin;
    tensaoPin = analogRead(pos_chave_pin); //analogRead(analogPin);
    buffer = (tensaoPin * vref)/1024.0;
    if(instant_fuel_consumption > 0.3) {
        tensao = (buffer / (R2/(R1+R2))+ 1.9); //  de correcao
    } else {
        tensao = (buffer / (R2/(R1+R2)) + 1.3); //  de correcao
    }

#if  defined(POS_CHAVE_DEBUG)
    Serial.print("Tensao: ");
    Serial.println(tensao);
    Serial.print("Tensao Pin: ");
    Serial.println(tensaoPin);
    Serial.print("Buffer: ");
    Serial.println(buffer);
#endif
}
//--------- Pos chave - 5 ---------//

//--------- rpm - 6 ---------//
void rpm_count() {
    rpm_pulse_count++;
}
//--------- rpm - 6 ---------//

//--------- Dimmer Sensor - 7 ---------//
void dimmerSensor() {
    //dois ultimos parametros, brilho minimo e maximo possiveis, diminuir caso incomede a noite
    //backLightLevel = (uint16_t) map(analogRead(lvl_dimmer_pin), 254, 1023, 5, 35); // 5k
    //backLightLevel = (uint16_t) map(analogRead(lvl_dimmer_pin), 145, 850, 5, 35); //10k
	backLightLevel = (uint16_t) map(analogRead(lvl_dimmer_pin), 254, 1023, 5, 35);
    if (analogRead(lvl_dimmer_pin) == 0) {
        backLightLevel = 95;   //Se lanterna estiver desligado, nao tem sinal de dimmer. Definir brilho padrão, geralmente durante o dia...
    }
#if  defined(DIM_SENS_DEBUG)
    //Serial.print("Dim collection ");
    //Serial.println(backLightReadCollection);
    //Serial.print("Dim index ");
    //Serial.println(backLightIndex);
    Serial.print("Display board read pure ");
    Serial.println(analogRead(lvl_dimmer_pin));
#endif
    /*
            if (backLightLevel > 25) {
                backLightLevel = 25;
            }

            if (backLightLevel < 1) {
                backLightLevel = SCREEN_DEF_LIGHT;
                CUR_DIM_ON = false;
            }
            CUR_DIM_ON = true;
            analogWrite(pinScreenOutput, backLightLevel * 10);
    */
#if  defined(DIM_SENS_DEBUG)
    Serial.print("Display Dim ");
    Serial.println(backLightLevel);
#endif

}//--------- Fim Dimmer Sensor - 7 ---------//


//--------- Sinal VSS ---------//
void updateVss () {
    pulse_distance = ((float) 1 / (float) vss);
}

/* contador da distancia/pulso */
void distance_count() {
    dist_pulse_count++;
    cemMetros++;
    if (cemMetros >= (vss/10)) {
        traveled_distance += 0.1; //distancia
        oil_run += 0.1; //troca oleo
        cemMetros = 0;
    }

}

//velocidade atual
void currentSpeed() {
    //FILTRO RUIDO
    /*
    // atualmente não vejo necessidade, não notei ruído. Talvez em carros que não tenha abs e com sensor no câmbio seja necessário ativar essa função
    if (speed == 0 && dist_pulse_count > 20) { // se a velocidade é igual a zero e estamos tendo mais que 20 pulsos, talvez seja um provável ruído.
      traveled_distance -= pulse_distance * dist_pulse_count;
      dist_pulse_count = 0;
    }
     */
    speed = pulse_distance * dist_pulse_count * 3600;
}
// velocidade maxima
void maxSpeed() {
    if (speed > max_speed) {
        max_speed = speed;
    }
}
// velocidade media
void avgSpeed() {
    all_speed_values += speed;
    avg_speed = all_speed_values / avg_speed_divider;
}
//--------- Fim VSS  ---------//

//--------- Falha Oleo - 13 ---------//
void lvlOilSensor() {
    if(lvl_oil_sen == 1) {
        if (digitalRead(lvl_oil_pin) == LOW) {
            lvl_oil = true;
        } else {
            lvl_oil = false;
        }
    }
#ifdef DEBUG_SENSORS
    Serial.print("lvl_oil_pin ");
    Serial.print(digitalRead(lvl_oil_pin));
    Serial.print("  result:");
    Serial.println(lvl_oil);
#endif
}//--------- Fim Falha Oleo - 13 ---------//

//--------- Falha Fusivel Freio - 16 ---------// //!Confirmar lógica invertida pós  funcionamento! !!!!!!!!!!! importante
void brakeFuseSensor() {
    if (digitalRead(brake_fuse_pin) == LOW) {
        brake_fuse = true;
    } else {
        brake_fuse = false;
    }
#ifdef DEBUG_SENSORS
    Serial.print("brake_fuse_pin ");
    Serial.print(digitalRead(brake_fuse_pin));
    Serial.print("  result:");
    Serial.println(brake_fuse);
#endif

}//--------- Falha Fusivel Freio - 16 ---------//

//--------- Falha lanterna - 17 ---------//
void lant_p1Sensor() {
    if(lant_p1_sen) {
        if (digitalRead(lant_p1_pin) == LOW) {
            lant_p1 = false;
        } else {
            lant_p1 = true;
        }
    }
#ifdef DEBUG_SENSORS
    Serial.print("lant_p1_pin ");
    Serial.print(digitalRead(lant_p1_pin));
    Serial.print("  result:");
    Serial.println(lant_p1);
#endif
}//--------- Fim Falha lanterna - 17 ---------//

//--------- Falha Luz Freio - 18 ---------
void freio_p1Sensor() {
    if (freio_p1_sen == 1) {
        if (digitalRead(brake_light_pin) == HIGH) {
            indexFreioTemp++;

            //estado de leitura
            if (indexFreioTemp <= 10) {
                // Make more measurements to calculate average
                smoothFreioTemp = analogRead(freio_p1_pin) + smoothFreioTemp;
            }

            if (indexFreioTemp == 10) {
                valFreio = (int)(smoothFreioTemp / indexFreioTemp);
                indexFreioTemp = 0;
                smoothFreioTemp = 0;

                if (valFreio > freio_p1_rate) {
                    freio_p1 = true;
                } else {
                    freio_p1 = false;
                }

#ifdef DEBUG_LAMP_FREIO
                Serial.print("Freio valFreio ");
                Serial.print(valFreio);
                Serial.print("Freio Status ");
                Serial.print(freio_p1);
                //Serial.print(" / Real:");
                //Serial.println(analogRead(freio_p1_pin));
#endif
            }
        }
    }
}
//--------- Fim Falha Luz Freio - 18 ---------//

//--------- Falha Pastilha Freio - 19 ---------//
void brakePadSensor() {
    if (brake_pad_sen == 1) {
        if (digitalRead(brake_pad_pin) == LOW) {
            brake_pad = false;
        } else {
            brake_pad = true;
        }
    }
#ifdef DEBUG_SENSORS
    Serial.print("brake_pad_pin ");
    Serial.print(digitalRead(brake_pad_pin));
    Serial.print("  result:");
    Serial.println(brake_pad);
#endif
}//--------- Fim Falha Pastrilha Freio - 19 ---------//

//--------- Falha Arrefecimento - 20 ---------//
void lvlCoolntSensor() {
    if (lvl_coolnt_sen == 1) {
        if (digitalRead(lvl_coolnt_pin) == LOW) {
            lvl_coolnt = false;
        } else {
            lvl_coolnt = true;
        }
    }
#ifdef DEBUG_SENSORS
    Serial.print("lvl_coolnt_pin ");
    Serial.print(digitalRead(lvl_coolnt_pin));
    Serial.print("  result:");
    Serial.println(lvl_coolnt);
#endif
}//--------- Fim Falha Arrefecimento - 20 ---------//

//--------- Falha Cambio AT - 21 ---------//
void transSensor() {
    if (trans_status_sen == 1) {
        if (digitalRead(trans_pin) == LOW) {
            trans_status = true;
        } else {
            trans_status = false;
        }
    }
#ifdef DEBUG_SENSORS
    Serial.print("trans_pin ");
    Serial.print(digitalRead(trans_pin));
    Serial.print("  result:");
    Serial.println(trans_status);
#endif
}//--------- Fim Falha Cambio AT - 21 ---------//

//--------- Falha Lavador - 22 ---------//
void lvlWashSensor() {
    if(lvl_wash_sen == 1) {
        if (digitalRead(lvl_wash_pin) == LOW) {
            lvl_wash = false;
        } else {
            lvl_wash = true;
        }
    }
#ifdef DEBUG_SENSORS
    Serial.print("lvl_wash_pin ");
    Serial.print(digitalRead(lvl_wash_pin));
    Serial.print("  result:");
    Serial.println(lvl_wash);
#endif
}//--------- Fim Falha Lavador - 22 ---------//

//--------- Sensor de nível do tanque - 26 ---------//
void tankSensor() {
	if (tensao > 7){
		indexTankTemp++;
		//estado de leitura
		if (indexTankTemp <= 10) {
			// Make more measurements to calculate average
			smoothTankTemp = analogRead(lvl_tank_pin) + smoothTankTemp;
		}
		//estado pos leitura, calcular a media
		if (indexTankTemp == 10) {
			val_tnk = (int)(smoothTankTemp / indexTankTemp);
			indexTankTemp = 0;
			smoothTankTemp = 0;
			CUR_TNK_OLD = CUR_TNK;
			/*
			carro Off

			Engine temperature 133  result:45 / Real:131
			Tank sensor 542  result:8 / Real:542

			carro On

			Engine temperature 155  result:47 / Real:156
			Tank sensor 638  result:9 / Real:639
			
			valor mostrado =(x - 156) * (6 - 52) / (660 - 156) + 52

			*/
			//CUR_TNK = (uint8_t) map(val_tnk, 122, 542, 52, 8); //
			CUR_TNK = (uint8_t) map(val_tnk, 122, 570, 52, 5); //10/10/10'8
			
			if (rpm_lvl > 700) { // carro em funcionamento
				//CUR_TNK = (uint8_t) map(val_tnk, 156, 639, 52, 6); // run 1 - 11/07/2018
				//CUR_TNK = (uint8_t) map(val_tnk, 145, 649, 51, 5);   // run 3 - 29/07/2018
				//CUR_TNK = (uint8_t) map(val_tnk, 145, 645, 51, 5);   // run 4 - 10/10/2018
				//CUR_TNK = (uint8_t) map(val_tnk, 122, 603, 52, 5);   // run 4 - 10/10/2018 = 122 cheio 612 8 litros
				//CUR_TNK = (uint8_t) map(val_tnk, 145, 639, 52, 5);   // run 5 - 19/12/2018
				CUR_TNK = (uint8_t) map(val_tnk, 145, 645, 52, 5);   // run 6 - 03/04/2019
			}
			// Maximo registrato 112c - 7.40v (convertido 3.544 - 726)
			// Minimo registrado 27c  - 1.250v (convertido 0.599 - 123)
			// CUR_TNK = (uint8_t) map(val, 123, 586, 27, 102);
		#ifdef DEBUG_TANK_TEMP
			Serial.print("Tank sensor ");
			Serial.print(val_tnk);
			Serial.print("  result:");
			Serial.print(CUR_TNK);
			Serial.print(" / Real:");
			Serial.println(analogRead(lvl_tank_pin));
		#endif
/* errado antigo
		// Smooth para subida
			// se o tanque atual for maior que o tanque antigo em 6p ignora porém aumenta em 1L para leitura smooth
			// atual 40 e old 35
			// 
			//  se (40 >= (35 + (35/6)))
			//	entao
			//  CUR_TNK = CUR_TNK_OLD + 1;
			//  CUR_TNK = 51

			if (CUR_TNK > (CUR_TNK_OLD + (CUR_TNK_OLD/6))){
				CUR_TNK = CUR_TNK - (CUR_TNK_OLD/6);
			}
			//
			// se o tanque atual for menor que o tanque antigo em 6p ignora porém diminui em 1L para leitura smooth
			// atual 25 e old 35
			// entao atual = atual + (antigo + (antigo /6))
			//   25         (35 - (35/6)
			//      35 - (35 - (35/6)
			if (CUR_TNK < (CUR_TNK_OLD - (CUR_TNK_OLD/6))){
				CUR_TNK =  CUR_TNK_OLD - (CUR_TNK_OLD/6);
			}			
*/
/* subida semi rapida
			if (CUR_TNK > (CUR_TNK_OLD + (CUR_TNK_OLD/6))){
				CUR_TNK = CUR_TNK_OLD + (CUR_TNK/6);
			}
			//

			//  Smooth para descida
			if (CUR_TNK < (CUR_TNK_OLD - (CUR_TNK_OLD/6))){
				CUR_TNK =  CUR_TNK_OLD - (CUR_TNK_OLD/6);
			}	
*/
/* subida lenta */
			if (CUR_TNK > CUR_TNK_OLD){  //  Smooth para subida
				CUR_TNK = CUR_TNK_OLD + 1;
			} else			
			if (CUR_TNK < CUR_TNK_OLD){ //  Smooth para descida
				CUR_TNK =  CUR_TNK_OLD - 1;
			}			
			
			
			if (CUR_TNK > 52) CUR_TNK = 52; //Correcao para valores maiores que o maximo do tanque.
		}
	}
}
//--------- Fim Sensor de nível do tanque ---------//

//--------- Update Fuel injection_value ---------//
void updateInjection() {
//Atualiza o valor do injection value, mesmos cálculos da planilha auxiliar porém resumidos.
    float lh_min = ref_mid_cons / 60; //Quantos litros hora por minuto?
    float lh_seg = lh_min / 60;   //Quantos litros hora por segundo?
    float pulse_s = ((float)ref_mid_pulse / 60);  // Quantos pulsos por segundo de sinal de injeção?
    injection_value = ((1*lh_seg)/pulse_s)/4;	//Litros por segundo divididos por pulsos por segundo, dividido por 4 (janela 250ms timer)

}
//--------- Sinal Consumo ---------//

void rising() {
    attachInterrupt(1, falling, FALLING);
    prev_time = millis();
}

void falling() {
    attachInterrupt(1, rising, RISING);
    pwm_value += millis() - prev_time;
    temp_injector_pulse_time += temp_injector_pulse_time + pwm_value;
}

//--------- Consumo medio ---------//

void fuelConsumption() {
    //Estamos trabalhando com janelas de 250ms no timer principal (por isso o counter até 3 no timer principal, portanto vamos multiplicar tudo por 4
    //Cenário - 1 - Carro em movimento
    if (speed > 1) {
        //em alguns cenários usar a variável speed_temp como auxiliar para evitar uns erros estranhos, sem movimento não ocorre o erro.
        speed_temp = speed;
        // Em caso de curt-off
        if (((((pwm_value * injection_value) * 3600) * 4) == 0) /*|| (speed_temp == 0)*/) {
            instant_fuel_consumption = 0;
            //curt-off diminuir o divisor afim de aumentar a media em movimento, se nao, não interferir
            avg_fuel_divider--;
        } else { //carro fora de cut-off, consumindo combustível
            //instant_fuel_consumption = (100 * (((pwm_value * injection_value) * 3600) * 4)) / speed; // medida imperial ex: 10L por 100KM
            // calculo consumo médio em Km/L
            instant_fuel_consumption = speed_temp / (((pwm_value * injection_value) * 3600) * 4);
            // se o valor do consumo instantaneo for maior que a velocidade - 80 litros/km à 60km/h -  ignorar pois tal cenário é pouco provável, fruto de ruido. - Investigar
            if (instant_fuel_consumption > speed_temp) {
                instant_fuel_consumption = speed_temp;
            }
			//por fim adicionamos o valor obtido a variável para cálculo da média
            all_fuel_consumption_values += instant_fuel_consumption;
        }
        //calcula a media
		//O uso da função isnan é pra evitar erros nos calculos de não divisão.
        average_fuel_consumption = all_fuel_consumption_values / avg_fuel_divider;
        if (isnan(average_fuel_consumption)) average_fuel_consumption = 0;

     //Cenário 2 - Carro parado
    } else {
        //litros em uma hora
        instant_fuel_consumption = ((pwm_value * injection_value) * 3600) * 4;
        //checar máximo de consumo com o carro parado afim de filtrar - verificar depois de circuito pronto.
        if (speed == 0 && instant_fuel_consumption > 20) {
            instant_fuel_consumption = 0;
            pwm_value = 0;
        }
		//adicionamos o valor obtido a variável para cálculo da média
        all_fuel_consumption_values += instant_fuel_consumption;
        //dando erro na divisão!super importante
        //infere na media geral caso esteja funcionando parado - validar
        //if (instant_fuel_consumption > 0) average_fuel_consumption = all_fuel_consumption_values / avg_speed_divider;
        average_fuel_consumption = all_fuel_consumption_values / avg_fuel_divider;
        //Se não obtermos um valor real iremos manter a variável zero. Isso acontece quando ainda não se deslocamos suficiente para obter um cálculo real.
		if (isnan(average_fuel_consumption)) average_fuel_consumption = 0;
    }


    //--------- Consumo Total preempitivo, Autonomia ---------//
    //aqui iremos armazenar um valor aproximado do consumo de combustível.
    //talvez seja completamente inútil já que na versão final teremos a leitura do volume do tanque.
    //Mas talvez seja útil para futura calibração.

    used_fuel += (pwm_value * injection_value);
    //if (tank_capacity > 0) tank_capacity = tank_capacity - used_fuel;
}     //--------- Fim consumo medio ---------//

//--------- Autonomia ---------//
void autonomia() {
   /*
	//Mostra autonomia ate a reserva - 6 litros
	if (CUR_TNK >= 6){
		autonomia_km = (average_fuel_consumption * (CUR_TNK - 6));
	//Mostra autonomia da reserva ate acabar tudo.
	} else {
		autonomia_km = (average_fuel_consumption * CUR_TNK);
	}
    */
	autonomia_km = (average_fuel_consumption * CUR_TNK);
	//Se a autonomia tiver menos que 60 e a distancia percorrida maior que 15km, vamos mostrar a tela de autonomia, pro sujeito se situar.
	if (autonomia_km <= 60 && traveled_distance > 15) {
        indiceMenu = 6;
        indiceSubMenu = 1;
        inSubMenu = 0;
        //Serial.print("page 0");
        //sendEsc();
    }
}
//--------- Fim autonomia ---------//

//--------- Temperatura Motor ---------//
void tempMotor() {
    indexEngineTemp++;

    //estado de leitura
    if (indexEngineTemp <= 10) {
        // Make more measurements to calculate average
        smoothEngineTemp = analogRead(temp_engine_pin) + smoothEngineTemp;
    }

    //estado pÃƒÂ³s leitura, calcular a mÃƒÂ©dia
    if (indexEngineTemp == 10) {
        val = (int)(smoothEngineTemp / indexEngineTemp);
        indexEngineTemp = 0;
        smoothEngineTemp = 0;

        // cap 47uf 225 - 80C / 515 - 90C //revisar com carro quente
		//CUR_ENT = (uint8_t) map(val, 156, 759, 46, 102);
		CUR_ENT = (uint8_t) map(val, 156, 775, 46, 104);


        if (CUR_ENT > MAX_CUR_ENT) MAX_CUR_ENT = CUR_ENT;
        if (CUR_ENT >= (uint8_t) 110) {
            temp_eng_high = true;
        } else {
            temp_eng_high = false;
        }
        //if (CUR_ENT < MIN_CUR_ENT) MIN_CUR_ENT = CUR_ENT;
        // Maximo registrato 112c - 7.40v (convertido 3.544 - 726)
        // Minimo registrado 27c  - 1.250v (convertido 0.599 - 123)
        // CUR_ENT = (uint8_t) map(val, 123, 586, 27, 102);

#ifdef DEBUG_ENG_TEMP
        Serial.print("Engine temperature ");
        Serial.print(val);
        Serial.print("  result:");
        Serial.print(CUR_ENT);
        Serial.print(" / Real:");
        Serial.println(analogRead(temp_engine_pin));
#endif
    }
} //--------- Fim Temperatura Motor ---------//

//--------- Inspecao Oleo ---------//
void inspOil() {
    if(oil_insp_sen == 1) {
		//se oil_run e menor que oil_change ou data atual <= data futura
        if (oil_run < oil_change) {
            oil_insp = false;
        } else {
            oil_insp = true;
        }
    }
}//--------- Fim Inspecao Oleo ---------//


