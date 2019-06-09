//funcao bastante repetida, espaço entre o envio de informações ao nextion
void sendEsc() {
    Serial.write(0xff);
    Serial.write(0xff);
    Serial.write(0xff);
}
void beepMid() {
    /* Ligamos o buzzer com uma frequencia de 1500hz*/
    if(one_beep == false) { // usar um beep a cada dois minutos pra evitar algazarra
        tone(buzzer,3400);
        delay(500);
        noTone(buzzer);
        one_beep = true;
        /*Desligamos o Buzzer*/
    }
}
void backlightMenuF() {
    if (backLightLevel != temp_backLightLevel) {
        //---- Ajuste do backlight ----//
        Serial.print("dim=");
        Serial.print(backLightLevel);
        sendEsc();
        temp_backLightLevel = backLightLevel;
    }
}
// Funcao para escrever no menu de falhas
void writeMenuF (String a, String b, String c) {
    Serial.print("p0.pic=");
    Serial.print(a);
    sendEsc();
    //
    //
    Serial.print("t0.txt=");
    Serial.write(0x22);
    Serial.print(b);
    Serial.write(0x22);
    sendEsc();
    //
    Serial.print("t1.txt=");
    Serial.write(0x22);
    Serial.print(c);
    Serial.write(0x22);
    sendEsc();
    //
}
//Funcao para mostrar imagens menu principal

void writeMenuImgP(uint8_t p0, uint8_t p1, uint8_t p2, uint8_t p3) {
    //pic p0
    Serial.print("p0.pic=");
    Serial.print(p0);
    sendEsc();
    //Linha 3 . 1
    Serial.print("p1.pic=");
    Serial.print(p1);
    sendEsc();
    //Linha 3 . 2
    Serial.print("p2.pic=");
    Serial.print(p2);
    sendEsc();
    //Linha 3 . 3
    Serial.print("p3.pic=");
    Serial.print(p3);
    sendEsc();
}

// Funcao para escrever no menu principal
void writeMenuP (
    String t0, //valor
    float t1, 
    float t3,
    float t4,
    float t5,
	char unid, //unidade
	uint8_t x  //casas decimais
	) 
{
	//variavel para sigla/medidas
	String medida;
	switch (unid)
	{
		case 0:
		medida = "";
		break;
	
		case 1:
		medida = "L/h";
		break;

		case 2:
		medida = "Km/L";
		break;

		case 3:
		medida = "Lts";
		break;

		case 4:
		medida = "Km";
		break;

		case 5:
		medida = "C";
		break;
		
		case 6:
		medida = "Km/h";
		break;		
	}
//https://stackoverflow.com/questions/27651012/arduino-sprintf-float-not-formatting
//* 2 is mininum width, 2 is precision; float value is copied onto str_temp*/
//Transforma float em string e já ajusta a precisão para x digito
char str_temp[8];
dtostrf(t1, 2, x, str_temp);	
//compila em frase o valor e a unidade de medida.
char frase[23];
sprintf(frase, "%s %s", str_temp, medida.c_str());

    // Linha 1
    sendEsc();
    Serial.print("t0.txt=");
    Serial.write(0x22);
    Serial.print(t0);
    Serial.write(0x22);
    // Linha 2
    sendEsc();
    Serial.print("t1.txt=");
    Serial.write(0x22);
    Serial.print((String)frase);
    Serial.write(0x22);
    //
    sendEsc();
    Serial.print("t3.txt=");
    Serial.write(0x22);
    Serial.print(t3,0);
    Serial.write(0x22);
    //
    sendEsc();
    Serial.print("t4.txt=");
    Serial.write(0x22);
    Serial.print(t4,1);
    Serial.write(0x22);
    //
    sendEsc();
    Serial.print("t5.txt=");
    Serial.write(0x22);
    Serial.print(t5,1);
    Serial.write(0x22);
    sendEsc();

}

// Funcao para escrever no menu principal - segundo arg string
void writeMenuS (
    String t0,
    String t1,
    float t3,
    float t4,
    float t5
) {
    // Linha 1
    sendEsc();
    Serial.print("t0.txt=");
    Serial.write(0x22);
    Serial.print(t0);
    Serial.write(0x22);
    // Linha 2
    sendEsc();
    Serial.print("t1.txt=");
    Serial.write(0x22);
    Serial.print(t1);
    Serial.write(0x22);
    //
    sendEsc();
    Serial.print("t3.txt=");
    Serial.write(0x22);
    Serial.print(t3,0);
    Serial.write(0x22);
    //
    sendEsc();
    Serial.print("t4.txt=");
    Serial.write(0x22);
    Serial.print(t4,1);
    Serial.write(0x22);
    //
    sendEsc();
    Serial.print("t5.txt=");
    Serial.write(0x22);
    Serial.print(t5,1);
    Serial.write(0x22);
    sendEsc();

}


// Funcao para escrever no Cronometro
void writeCron (
    String t0,
    String t1,
    float t3,
    float t4,
    float t5
) {
    // Linha 1
    sendEsc();
    Serial.print("t0.txt=");
    Serial.write(0x22);
    Serial.print(t0);
    Serial.write(0x22);
    // Linha 2
    sendEsc();
    Serial.print("t1.txt=");
    Serial.write(0x22);
    Serial.print(t1);
    Serial.write(0x22);
    //
    sendEsc();
    Serial.print("t3.txt=");
    Serial.write(0x22);
    Serial.print(t3,0);
    Serial.write(0x22);
    //
    sendEsc();
    Serial.print("t4.txt=");
    Serial.write(0x22);
    Serial.print(t4,1);
    Serial.write(0x22);
    //
    sendEsc();
    Serial.print("t5.txt=");
    Serial.write(0x22);
    Serial.print(t5,1);
    Serial.write(0x22);
    sendEsc();


}
