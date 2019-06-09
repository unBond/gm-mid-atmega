/*
Menu principal
Esqueleto/lógica dos menus: 
Autor: Eng. Wagner Rambo  
Baseado no video tutorial: Curso de Arduino e AVR 134: https://www.youtube.com/watch?v=tRoznRBYQHA

Existe um bug no código, em que sempre que for enviado data/hora para a tela, essa deve ser a última/Ou única instrução enviada por loop, 
se ela for enviada primeiro que as outras o microcontrolado trava. Creio ser um bug na biblioteca de de data/hora, até por que editei ela na pressa
pra implementar a funcao de cálculo de data futura.

Dá pra contornar salvando em variaveis globais antes de se usar.

Tirando isso não notei problemas ou outros impactos.

*/
void menu_display() {
    if (retorno_menu_1 == 0) {
        // Retorno de entrada menu principal...
        indiceMenuOld = 0; //reset para reaparecer as imagens
        Serial.print("page 0");
        sendEsc();
        retorno_menu_1 = 1;
    }
    if (digitalRead(btn_set_pin)) menu_flag = 0x01; //Botão Up pressionado? Seta flag
    if (digitalRead(btn_reset_pin)) enter_flag = 0x01; //Botão Down pressionado? Seta flag

    if (!digitalRead(btn_set_pin) && menu_flag && !inSubMenu) //Botão Up solto e flag setada?
    {   //Sim...
        menu_flag = 0x00; //Limpa flag

        indiceMenu++; //Incrementa menu

        if (indiceMenu > totalMenu) indiceMenu = 0x01; //Se menu maior que totalMenu, volta a ser 1

    } //end menu

    if (!digitalRead(btn_reset_pin) && enter_flag && !inSubMenu) //Botão Down solto e flag setada?
    {   //Sim...
        enter_flag = 0x00; //Limpa flag
        inSubMenu = 1;
    } //end butreset

    //backLightLevel
    backlightMenuF();

    menuPrincipal(indiceMenu);
    if (indiceMenu <= totalMenu) menuImg(); // evitar erro caso nao exista imagens nos ultimos menu
} //endMenudisplay

// pra evitar flickering nas imagens, criamos uma função para atualizar somente uma vez as imagens do menu */
void menuImg() {

    if (indiceMenuOld != indiceMenu) { //Condição para que seja executada somente uma vez por Menu.
        indiceMenuOld = indiceMenu;
        switch (indiceMenu) {
        case 1:
            //menuConsInsta();
            writeMenuImgP(
                2, // uint8_t p0,
                0, // uint8_t p1
                14, // uint8_t p2
                13 // uint8_t p3
            );
            break;
        case 2:
            //menuConsMed();
            writeMenuImgP(
                2, // uint8_t p0,
                0, // uint8_t p1
                14, // uint8_t p2
                13 // uint8_t p3
            );
            break;
        case 3:
            //menuConsTot();
            writeMenuImgP(
                2, // uint8_t p0,
                0, // uint8_t p1
                14, // uint8_t p2
                13 // uint8_t p3
            );
            break;
        case 4:
            //menuVel();
            writeMenuImgP(
                17, // uint8_t p0,
                0, // uint8_t p1
                14, // uint8_t p2
                13 // uint8_t p3
            );
            break;
        case 5:
            //menuDist();
            writeMenuImgP(
                16, // uint8_t p0,
                0, // uint8_t p1
                14, // uint8_t p2
                13 // uint8_t p3
            );
            break;
        case 6:
            //menuAuton();
            writeMenuImgP(
                2, // uint8_t p0,
                0, // uint8_t p1
                3, // uint8_t p2
                13 // uint8_t p3
            );
            break;
        case 7:
            //funCronometro();
            writeMenuImgP(
                18, // uint8_t p0,
                0, // uint8_t p1
                14, // uint8_t p2
                13 // uint8_t p3
            );
            break;
        case 8:
            //menuTempEng();
            writeMenuImgP(
                19, // uint8_t p0,
                0, // uint8_t p1
                14, // uint8_t p2
                13 // uint8_t p3
            );
            break;
		case 9:
            //menuFullReset();
            writeMenuImgP(
                19, // uint8_t p0,
                0, // uint8_t p1
                14, // uint8_t p2
                13 // uint8_t p3
            );		
            break;
		case 10:
            //dataHora();
            writeMenuImgP(
                18, // uint8_t p0,
                0, // uint8_t p1
                14, // uint8_t p2
                13 // uint8_t p3
            );
            break;
        case 11:
            //menuConfig();
            writeMenuImgP(
                25, // uint8_t p0,
                0, // uint8_t p1
                14, // uint8_t p2
                13 // uint8_t p3
            );
            break;			
        }
    }
}
/* dica joão
Switch ( nessa variável ) {
	if 0 : {
		Abre menu
	}
	Default:
		Switch ( outra variável )
		{}
} */
void menuPrincipal(char p) {
    switch (p) {
    case 1:
        menuConsInsta();
        break;
    case 2:
        menuConsMed();
        break;
    case 3:
        menuConsTot();
        break;
    case 4:
        menuVel();
        break;
    case 5:
        menuDist();
        break;
    case 6:
        menuAuton();
        break;
    case 7:
        funCronometro();
        break;
    case 8:
        menuTempEng();
        break;
	case 9:
        menuFullReset();
        break;    
	case 10:
        dataHora();
        break;
    case 11:
        menuConfig();
        break;
    case 12:
        sensorConfig();
        break;
    case 13:
        inspConfig();
        break;
    case 14:
        vssConfig();
        break;		
    case 15:
        injectorConfig();
        break;		
    case 16:
        ajDataHora();
        break;				
    }
} //end menuPrincipal

//----------- Cons inst. ----------- //
void menuConsInsta() {
/*
if (speed_temp == 0){
    writeMenuP("1. Cons. Inst.", // String t0,
                display_instant_fuel_consumption, // float t1
                speed_temp, // float t3
                tensao, // float t4
                CUR_OUT_TMP, // float t5
				1, //litros hora
				1 // casa decimal
                );
} else {
    writeMenuP("1. Cons. Inst.", // String t0,
                display_instant_fuel_consumption, // float t1
                speed_temp, // float t3
                tensao, // float t4
                CUR_OUT_TMP, // float t5
				2, 	//km/l
				1 //casa decimal
                );		
}
inSubMenu = 0; //Sai do submenu


*/

    //writeMenuP("Cons. Inst.", display_instant_fuel_consumption);
    //inSubMenu = 0; //Nao existe submenu aqui

    if (!inSubMenu) {
		if (speed_temp == 0){
        writeMenuP("1. Cons. Inst.", // String t0,
                   display_instant_fuel_consumption, // float t1
                   speed_temp, // float t3
                   tensao, // float t4
                   CUR_OUT_TMP, // float t5
					1, //litros hora
					1 // casa decimal
                  );
		} else {
        writeMenuP("1. Cons. Inst.", // String t0,
                   display_instant_fuel_consumption, // float t1
                   speed_temp, // float t3
                   tensao, // float t4
                   CUR_OUT_TMP, // float t5
					2, 	//km/l
					1 //casa decimal
                  );		
		}

    } else {
        //Serial.println("Inside Medio");
        if (digitalRead(btn_set_pin)) menu_flag = 0x01; //Botão Up pressionado? Seta flag
        if (digitalRead(btn_reset_pin)) enter_flag = 0x01; //Botão Down pressionado? Seta flag
        if (!digitalRead(btn_set_pin) && menu_flag && inSubMenu) //Botão Up solto e flag setada?
        {   //Sim...
            menu_flag = 0x00; //Limpa flag
            indiceSubMenu++; //Incrementa menu
            if (indiceSubMenu > 0x02) indiceSubMenu = 0x01; //Se menu maior que 3, volta a ser 1

        } //end menu

        switch (indiceSubMenu) {
        case 1:
            writeMenuP("1.1 RPM", // String t0,
                       rpm_lvl, // float t1
                       speed_temp, // float t3
                       tensao, // float t4
                       CUR_OUT_TMP, // float t5
						0, //op
						0 //casa decimal
                      );
            break;
        case 2:
            //Serial.println("Sair");
            Serial.print("t0.txt=");
            Serial.write(0x22);
            Serial.print("1.2 Sair?");
            Serial.write(0x22);
            sendEsc();
            if (!digitalRead(btn_reset_pin) && enter_flag && inSubMenu) //Botão Down solto e flag setada?
            {   //Sim...
                enter_flag = 0x00; //Limpa flag
                //Serial.println("Saindo!!");
                indiceSubMenu = 1; // Retorna Posicao do Submenu pra 1
                inSubMenu = 0; //Sai do submenu
            } //end butreset
            break;
        } //fim switch (indiceSubMenu)
    } // fim else !inSubMenu





}
//----------- Fim Cons inst. ----------- //

//----------- Cons med. ----------- //
void menuConsMed() {
    if (!inSubMenu) {
        writeMenuP("2. Consumo", // String t0,
                   average_fuel_consumption, // float t1
                   speed_temp, // float t3
                   tensao, // float t4
                   CUR_OUT_TMP, // float t5
					2, //op
					1 //casa decimal
                  );
    } else {
        //Serial.println("Inside Medio");
        if (digitalRead(btn_set_pin)) menu_flag = 0x01; //Botão Up pressionado? Seta flag
        if (digitalRead(btn_reset_pin)) enter_flag = 0x01; //Botão Down pressionado? Seta flag
        if (!digitalRead(btn_set_pin) && menu_flag && inSubMenu) //Botão Up solto e flag setada?
        {   //Sim...
            menu_flag = 0x00; //Limpa flag
            indiceSubMenu++; //Incrementa menu
            if (indiceSubMenu > 0x02) indiceSubMenu = 0x01; //Se menu maior que 2, volta a ser 1

        } //end menu

        switch (indiceSubMenu) {
        case 1:
            //Serial.println("Zerar");
            Serial.print("t0.txt=");
            Serial.write(0x22);
            Serial.print("2.1 Zerar Med?");
            Serial.write(0x22);
            sendEsc();
            if (!digitalRead(btn_reset_pin) && enter_flag && inSubMenu) //Botão Down solto e flag setada?
            {   //Sim...
                enter_flag = 0x00; //Limpa flag
                indiceSubMenu = 2;
                //Serial.println("Zerou!!!!!!!!!!!!");
                zeraConsumo();
                Serial.print("t1.txt=");
                Serial.write(0x22);
                Serial.print("Resetado!");
                Serial.write(0x22);
                sendEsc();
            } //end butreset
            break;

        case 2:
            //Serial.println("Sair");
            Serial.print("t0.txt=");
            Serial.write(0x22);
            Serial.print("2.2 Sair?");
            Serial.write(0x22);
            sendEsc();
            if (!digitalRead(btn_reset_pin) && enter_flag && inSubMenu) //Botão Down solto e flag setada?
            {   //Sim...
                enter_flag = 0x00; //Limpa flag
                //Serial.println("Saindo!!");
                indiceSubMenu = 1; // Retorna Posicao do Submenu pra 1
                inSubMenu = 0; //Sai do submenu
            } //end butreset
            break;
        } //fim switch (indiceSubMenu)
    } // fim else !inSubMenu
} //----------- Fim Cons med. ----------- //

//----------- Cons. Total. ----------- //
void menuConsTot() {
    if (!inSubMenu) {
        writeMenuP("3. Cons. Total", // String t0,
                   used_fuel, // float t1
                   speed_temp, // float t3
                   tensao, // float t4
                   CUR_OUT_TMP, // float t5
					3, //op
					1 //casa decimal
                  );
    } else {
        //Serial.println("Inside Medio");
        if (digitalRead(btn_set_pin)) menu_flag = 0x01; //Botão Up pressionado? Seta flag
        if (digitalRead(btn_reset_pin)) enter_flag = 0x01; //Botão Down pressionado? Seta flag
        if (!digitalRead(btn_set_pin) && menu_flag && inSubMenu) //Botão Up solto e flag setada?
        {   //Sim...
            menu_flag = 0x00; //Limpa flag
            indiceSubMenu++; //Incrementa menu
            if (indiceSubMenu > 0x02) indiceSubMenu = 0x01; //Se menu maior que 2, volta a ser 1

        } //end menu

        switch (indiceSubMenu) {
        case 1:
            //Serial.println("Zerar");
            Serial.print("t0.txt=");
            Serial.write(0x22);
            Serial.print("3.1 Zerar Tot.?");
            Serial.write(0x22);
            sendEsc();
            if (!digitalRead(btn_reset_pin) && enter_flag && inSubMenu) //Botão Down solto e flag setada?
            {   //Sim...
                enter_flag = 0x00; //Limpa flag
                indiceSubMenu = 2;
                //Serial.println("Zerou!!!!!!!!!!!!");
                zeraUsedFuel();
                Serial.print("t1.txt=");
                Serial.write(0x22);
                Serial.print("Resetado!");
                Serial.write(0x22);
                sendEsc();
            } //end butreset
            break;

        case 2:
            //Serial.println("Sair");
            Serial.print("t0.txt=");
            Serial.write(0x22);
            Serial.print("3.2 Sair?");
            Serial.write(0x22);
            sendEsc();
            if (!digitalRead(btn_reset_pin) && enter_flag && inSubMenu) //Botão Down solto e flag setada?
            {   //Sim...
                enter_flag = 0x00; //Limpa flag
                //Serial.println("Saindo!!");
                indiceSubMenu = 1; // Retorna Posicao do Submenu pra 1
                inSubMenu = 0; //Sai do submenu
            } //end butreset
            break;
        } //fim switch (indiceSubMenu)
    } // fim else !inSubMenu

} //----------- Fim Cons. Total. ----------- //

//----------- Velocidade. ----------- //
void menuVel() {
    if (!inSubMenu) {
        writeMenuP("4. Vel. Med", // String t0,
                   avg_speed, // float t1
                   speed_temp, // float t3
                   tensao, // float t4
                   CUR_OUT_TMP, // float t5
					6, //op
					0 //casa decimal
                  );
    } else {
        //Serial.println("Inside Medio");
        if (digitalRead(btn_set_pin)) menu_flag = 0x01; //Botão Up pressionado? Seta flag
        if (digitalRead(btn_reset_pin)) enter_flag = 0x01; //Botão Down pressionado? Seta flag
        if (!digitalRead(btn_set_pin) && menu_flag && inSubMenu) //Botão Up solto e flag setada?
        {   //Sim...
            menu_flag = 0x00; //Limpa flag
            indiceSubMenu++; //Incrementa menu
            if (indiceSubMenu > 0x04) indiceSubMenu = 0x01; //Se menu maior que 2, volta a ser 1

        } //end menu

        switch (indiceSubMenu) {

        case 1:
            //Serial.println("Zerar");
            Serial.print("t0.txt=");
            Serial.write(0x22);
            Serial.print("4.1 Zerar Vel.?");
            Serial.write(0x22);
            sendEsc();
            if (!digitalRead(btn_reset_pin) && enter_flag && inSubMenu) //Botão Down solto e flag setada?
            {   //Sim...
                enter_flag = 0x00; //Limpa flag
                indiceSubMenu = 4;
                //Serial.println("Zerou!!!!!!!!!!!!");
                zeraAvgSpeed();
                Serial.print("t1.txt=");
                Serial.write(0x22);
                Serial.print("Resetado!");
                Serial.write(0x22);
                sendEsc();
            } //end butreset
            break;

        case 2:
            writeMenuP("4.2 Vel. Max.", // String t0,
                       max_speed, // float t1
                       speed_temp, // float t3
                       tensao, // float t4
                       CUR_OUT_TMP, // float t5	
						6, //op
						0 //casa decimal
                      );
            if (!digitalRead(btn_reset_pin) && enter_flag && inSubMenu) //Botão Down solto e flag setada?
            {   //Sim...
                enter_flag = 0x00; //Limpa flag
                indiceSubMenu = 2;
                //Serial.println("Zerou!!!!!!!!!!!!");
                zeraTopSpeed();
                Serial.print("t1.txt=");
                Serial.write(0x22);
                Serial.print("Resetado!");
                Serial.write(0x22);
                sendEsc();
            } //end butreset
            break;
        case 3:
            writeMenuP("4.3 Velocidade", // String t0,
                       speed_temp, // float t1
                       max_speed, // float t3
                       tensao, // float t4
                       CUR_OUT_TMP, // float t5
						6, //op
						0 //casa decimal
                      );
            break;

        case 4:
            //Serial.println("Sair");
            Serial.print("t0.txt=");
            Serial.write(0x22);
            Serial.print("4.4 Sair?");
            Serial.write(0x22);
            sendEsc();
            if (!digitalRead(btn_reset_pin) && enter_flag && inSubMenu) //Botão Down solto e flag setada?
            {   //Sim...
                enter_flag = 0x00; //Limpa flag
                //Serial.println("Saindo!!");
                indiceSubMenu = 1; // Retorna Posicao do Submenu pra 1
                inSubMenu = 0; //Sai do submenu
            } //end butreset
            break;
        } //fim switch (indiceSubMenu)
    } // fim else !inSubMenu

} //----------- Fim velocidade ----------- //

//----------- Distância. ----------- //
void menuDist() {
    if (!inSubMenu) {
        writeMenuP("5. Km Viagem", // String t0,
                   traveled_distance, // float t1
                   speed_temp, // float t3
                   tensao, // float t4
                   CUR_OUT_TMP, // float t5							// float t5
				   4, //op				
				   1 //casa decimal
                  );
    } else {
        //Serial.println("Inside Medio");
        if (digitalRead(btn_set_pin)) menu_flag = 0x01; //Botão Up pressionado? Seta flag
        if (digitalRead(btn_reset_pin)) enter_flag = 0x01; //Botão Down pressionado? Seta flag
        if (!digitalRead(btn_set_pin) && menu_flag && inSubMenu) //Botão Up solto e flag setada?
        {   //Sim...
            menu_flag = 0x00; //Limpa flag
            indiceSubMenu++; //Incrementa menu
            if (indiceSubMenu > 0x02) indiceSubMenu = 0x01; //Se menu maior que 2, volta a ser 1

        } //end menu

        switch (indiceSubMenu) {
        case 1:
            //Serial.println("Zerar");
            Serial.print("t0.txt=");
            Serial.write(0x22);
            Serial.print("5. Zerar Dist.?");
            Serial.write(0x22);
            sendEsc();
            if (!digitalRead(btn_reset_pin) && enter_flag && inSubMenu) //Botão Down solto e flag setada?
            {   //Sim...
                enter_flag = 0x00; //Limpa flag
                indiceSubMenu = 2;
                //Serial.println("Zerou!!!!!!!!!!!!");
                zeraDistancia();
                Serial.print("t1.txt=");
                Serial.write(0x22);
                Serial.print("Resetado!");
                Serial.write(0x22);
                sendEsc();
            } //end butreset
            break;

        case 2:
            //Serial.println("Sair");
            Serial.print("t0.txt=");
            Serial.write(0x22);
            Serial.print("5.2 Sair?");
            Serial.write(0x22);
            sendEsc();
            if (!digitalRead(btn_reset_pin) && enter_flag && inSubMenu) //Botão Down solto e flag setada?
            {   //Sim...
                enter_flag = 0x00; //Limpa flag
                //Serial.println("Saindo!!");
                indiceSubMenu = 1; // Retorna Posicao do Submenu pra 1
                inSubMenu = 0; //Sai do submenu
            } //end butreset
            break;
        } //fim switch (indiceSubMenu)
    } // fim else !inSubMenu

}
//----------- Distância. ----------- //

//----------- Autonomia. ----------- //
void menuAuton() {
    if (!inSubMenu) {
        writeMenuP("6. Autonomia", // String t0,
                   autonomia_km, // float t1
                   speed_temp, // float t3
                   CUR_TNK, // float t4
                   CUR_OUT_TMP, // float t5
					4, //op
					0 //casa decimal
                  );
    } else {
        //Serial.println("Inside Medio");
        if (digitalRead(btn_set_pin)) menu_flag = 0x01; //Botão Up pressionado? Seta flag
        if (digitalRead(btn_reset_pin)) enter_flag = 0x01; //Botão Down pressionado? Seta flag
        if (!digitalRead(btn_set_pin) && menu_flag && inSubMenu) //Botão Up solto e flag setada?
        {   //Sim...
            menu_flag = 0x00; //Limpa flag
            indiceSubMenu++; //Incrementa menu
            if (indiceSubMenu > 0x02) indiceSubMenu = 0x01; //Se menu maior que 2, volta a ser 1

        } //end menu

        switch (indiceSubMenu) {
        case 1:
            //Serial.println("Zerar");
            Serial.print("t0.txt=");
            Serial.write(0x22);
            Serial.print("6.1 Zerar Aut.?");
            Serial.write(0x22);
            sendEsc();
            if (!digitalRead(btn_reset_pin) && enter_flag && inSubMenu) //Botão Down solto e flag setada?
            {   //Sim...
                enter_flag = 0x00; //Limpa flag
                indiceSubMenu = 2;
                //Serial.println("Zerou!!!!!!!!!!!!");
                zeraAuton();
                Serial.print("t1.txt=");
                Serial.write(0x22);
                Serial.print("Resetado!");
                Serial.write(0x22);
                sendEsc();
            } //end butreset
            break;

        case 2:
            //Serial.println("Sair");
            Serial.print("t0.txt=");
            Serial.write(0x22);
            Serial.print("6.2 Sair?");
            Serial.write(0x22);
            sendEsc();
            if (!digitalRead(btn_reset_pin) && enter_flag && inSubMenu) //Botão Down solto e flag setada?
            {   //Sim...
                enter_flag = 0x00; //Limpa flag
                //Serial.println("Saindo!!");
                indiceSubMenu = 1; // Retorna Posicao do Submenu pra 1
                inSubMenu = 0; //Sai do submenu
            } //end butreset
            break;
        } //fim switch (indiceSubMenu)
    } // fim else !inSubMenu

}
//----------- Fim Autonomia. ----------- //

//----------- Cronometro ----------- //
void funCronometro() {
    String cronTime = ((String) cronHor + ":" + (String) cronMin + ":" + (String) cronSeg);
    if (!inSubMenu) {
        writeCron("7. Cronometro", // String t0,
                  cronTime, // float t1
                  speed_temp, // float t3
                  tensao, // float t4
                  CUR_OUT_TMP // float t5
                 );
    } else {
        //Serial.println("Inside Cronometro");
        if (digitalRead(btn_set_pin)) menu_flag = 0x01; //Botão Up pressionado? Seta flag
        if (digitalRead(btn_reset_pin)) enter_flag = 0x01; //Botão Down pressionado? Seta flag
        if (!digitalRead(btn_set_pin) && menu_flag && inSubMenu) //Botão Up solto e flag setada?
        {   //Sim...
            menu_flag = 0x00; //Limpa flag
            indiceSubMenu++; //Incrementa menu
            if (indiceSubMenu > 0x03) indiceSubMenu = 0x01; //Se menu maior que 2, volta a ser 1

        } //end menu

        switch (indiceSubMenu) {
        case 1:
            //Serial.println("Cont/Pause");
            writeCron("7.1 Cont/Parar", // String t0,
                      cronTime, // float t1
                      speed_temp, // float t3
                      tensao, // float t4
                      CUR_OUT_TMP // float t5
                     );
            if (!digitalRead(btn_reset_pin) && enter_flag && inSubMenu) //Botão Down solto e flag setada?
            {   //Sim...
                enter_flag = 0x00; //Limpa flag
                indiceSubMenu = 1;
                cronStatus = !cronStatus;
                /*Serial.print("t1.txt=");
                Serial.write(0x22);
                Serial.print("Resetado!");
                Serial.write(0x22);
                sendEsc();
                */
            } //end butreset
            break;

        case 2:
            //Serial.println("Zerar");
            writeCron("7.2 Zerar?", // String t0,
                      cronTime, // float t1
                      speed_temp, // float t3
                      tensao, // float t4
                      CUR_OUT_TMP // float t5
                     );
            if (!digitalRead(btn_reset_pin) && enter_flag && inSubMenu) //Botão Down solto e flag setada?
            {   //Sim...
                enter_flag = 0x00; //Limpa flag
                indiceSubMenu = 1;
                //Serial.println("Zerou!!!!!!!!!!!!");
                zeraCron();
                Serial.print("t1.txt=");
                Serial.write(0x22);
                Serial.print("Resetado!");
                Serial.write(0x22);
                sendEsc();
            } //end butreset
            break;

        case 3:
            //Serial.println("Sair");
            Serial.print("t0.txt=");
            Serial.write(0x22);
            Serial.print("7.3 Sair?");
            Serial.write(0x22);
            sendEsc();
            if (!digitalRead(btn_reset_pin) && enter_flag && inSubMenu) //Botão Down solto e flag setada?
            {   //Sim...
                enter_flag = 0x00; //Limpa flag
                //Serial.println("Saindo!!");
                indiceSubMenu = 1; // Retorna Posicao do Submenu pra 1
                inSubMenu = 0; //Sai do submenu
            } //end butreset
            break;

        } //fim switch (indiceSubMenu)
    } // fim else !inSubMenu
} //----------- Fim Cronometro ----------- //

//----------- Temperatura ----------- //
void menuTempEng() {
    if (!inSubMenu) {
        writeMenuP("8. Temp. Motor", // String t0,
                   CUR_ENT, // float t1
                   speed_temp, // float t3
                   tensao, // float t4
                   CUR_OUT_TMP, // float t5							// float t5
					5, //op
					0 //casa decimal
                  );
    } else {
        //Serial.println("Inside Medio");
        if (digitalRead(btn_set_pin)) menu_flag = 0x01; //Botão Up pressionado? Seta flag
        if (digitalRead(btn_reset_pin)) enter_flag = 0x01; //Botão Down pressionado? Seta flag
        if (!digitalRead(btn_set_pin) && menu_flag && inSubMenu) //Botão Up solto e flag setada?
        {   //Sim...
            menu_flag = 0x00; //Limpa flag
            indiceSubMenu++; //Incrementa menu
            if (indiceSubMenu > 0x02) indiceSubMenu = 0x01; //Se menu maior que 3, volta a ser 1

        } //end menu

        switch (indiceSubMenu) {
        case 1:
            writeMenuP("8.1 Max Temp.", // String t0,
                       MAX_CUR_ENT, // float t1
                       speed_temp, // float t3
                       tensao, // float t4
                       CUR_OUT_TMP, // float t5							// float t5
						5, //op		
						0 //casa decimal
                      );
            if (!digitalRead(btn_reset_pin) && enter_flag && inSubMenu) //Botão Down solto e flag setada?
            {   //Sim...
                enter_flag = 0x00; //Limpa flag
                indiceSubMenu = 1;
                //Serial.println("Zerou!!!!!!!!!!!!");
                MAX_CUR_ENT = 0;
                Serial.print("t1.txt=");
                Serial.write(0x22);
                Serial.print("Resetado!");
                Serial.write(0x22);
                sendEsc();
            } //end butreset
            break;

        case 2:
            //Serial.println("Sair");
            Serial.print("t0.txt=");
            Serial.write(0x22);
            Serial.print("8.2 Sair?");
            Serial.write(0x22);
            sendEsc();
            if (!digitalRead(btn_reset_pin) && enter_flag && inSubMenu) //Botão Down solto e flag setada?
            {   //Sim...
                enter_flag = 0x00; //Limpa flag
                //Serial.println("Saindo!!");
                indiceSubMenu = 1; // Retorna Posicao do Submenu pra 1
                inSubMenu = 0; //Sai do submenu
            } //end butreset
            break;
        } //fim switch (indiceSubMenu)
    } // fim else !inSubMenu
}
//----------- Fim Temperatura ----------- //

//----------- Full reset. ----------- //
void menuFullReset() {
    //writeMenuP("Cons. Inst.", display_instant_fuel_consumption);
    //inSubMenu = 0; //Nao existe submenu aqui

    if (!inSubMenu) {
        writeMenuP("9. Resetar tudo", // String t0,
                   average_fuel_consumption, // float t1
                   speed_temp, // float t3
                   tensao, // float t4
                   CUR_OUT_TMP, // float t5
					2, 	//op
					1 //casa decimal
                  );

    } else {
        //Serial.println("Inside Medio");
        if (digitalRead(btn_set_pin)) menu_flag = 0x01; //Botão Up pressionado? Seta flag
        if (digitalRead(btn_reset_pin)) enter_flag = 0x01; //Botão Down pressionado? Seta flag
        if (!digitalRead(btn_set_pin) && menu_flag && inSubMenu) //Botão Up solto e flag setada?
        {   //Sim...
            menu_flag = 0x00; //Limpa flag
            indiceSubMenu++; //Incrementa menu
            if (indiceSubMenu > 0x02) indiceSubMenu = 0x01; //Se menu maior que 3, volta a ser 1

        } //end menu

        switch (indiceSubMenu) {
        case 1:
            writeMenuP("9.1 Confirma?", // String t0,
                       average_fuel_consumption, // float t1
                       speed_temp, // float t3
                       tensao, // float t4
                       CUR_OUT_TMP, // float t5
						2, //op
						1 //casa decimal
                      );
            if (!digitalRead(btn_reset_pin) && enter_flag && inSubMenu) //Botão Down solto e flag setada?
            {   //Sim...
                enter_flag = 0x00; //Limpa flag
                indiceSubMenu = 2;
                //Serial.println("Zerou!!!!!!!!!!!!");
				fullReset();
                zeraAvgSpeed();
                Serial.print("t1.txt=");
                Serial.write(0x22);
                Serial.print("Resetado!");
                Serial.write(0x22);
                sendEsc();
            } //end butreset
            break;

        case 2:
            //Serial.println("Sair");
            Serial.print("t0.txt=");
            Serial.write(0x22);
            Serial.print("9.2 Sair?");
            Serial.write(0x22);
            sendEsc();
            if (!digitalRead(btn_reset_pin) && enter_flag && inSubMenu) //Botão Down solto e flag setada?
            {   //Sim...
                enter_flag = 0x00; //Limpa flag
                //Serial.println("Saindo!!");
                indiceSubMenu = 1; // Retorna Posicao do Submenu
                inSubMenu = 0; //Sai do submenu
            } //end butreset
            break;
        } //fim switch (indiceSubMenu)
    } // fim else !inSubMenu

}
//----------- Fim full reset. ----------- //

//----------- dataHora ----------- //
void dataHora() {

    if (!inSubMenu) {
		
		char buffer[5];
		sprintf(buffer, "%02d:%02d:%02d", dataAtual.hour(), dataAtual.minute(),dataAtual.second());
		char buffer2[10];
		sprintf(buffer2, "%02d.%02d.%d", dataAtual.day(), dataAtual.month(), dataAtual.year());
		//String seman = (String) daysOfTheWeek[now.dayOfTheWeek()];
		writeCron(buffer, // String t0,
			buffer2, // float t1
			speed_temp, // float t3
			tensao, // float t4
			CUR_OUT_TMP // float t5
			);
		
    } else {
        //Serial.println("Inside Medio");
        if (digitalRead(btn_set_pin)) menu_flag = 0x01; //Botão Up pressionado? Seta flag
        if (digitalRead(btn_reset_pin)) enter_flag = 0x01; //Botão Down pressionado? Seta flag
        if (!digitalRead(btn_set_pin) && menu_flag && inSubMenu) //Botão Up solto e flag setada?
        {   //Sim...
            menu_flag = 0x00; //Limpa flag
            indiceSubMenu++; //Incrementa menu
            if (indiceSubMenu > 0x02) indiceSubMenu = 0x01; //Se menu maior que 3, volta a ser 1

        } //end menu

        switch (indiceSubMenu) {
        case 1:

        char buffer3[30];
        
        sprintf(buffer3, "%02d.%02d.%d", dia, mes, ano);
        
                writeMenuS((String) oil_run + " km", // String t0,
                           (String) buffer3, // string
                           speed_temp, // float t3
                           tensao, // float t4
                           CUR_OUT_TMP
                          );
         break;

        case 2:
            //Serial.println("Sair");
            Serial.print("t0.txt=");
            Serial.write(0x22);
            Serial.print("Sair?");
            Serial.write(0x22);
            sendEsc();
            if (!digitalRead(btn_reset_pin) && enter_flag && inSubMenu) //Botão Down solto e flag setada?
            {   //Sim...
                enter_flag = 0x00; //Limpa flag
                //Serial.println("Saindo!!");
                indiceSubMenu = 1; // Retorna Posicao do Submenu pra 1
                inSubMenu = 0; //Sai do submenu
            } //end butreset
            break;

        } //fim switch (indiceSubMenu)
    } // fim else !inSubMenu

}
//----------- Fim dataHora ----------- //

//----------- Menu Config. ----------- //
void menuConfig() {
    if (rpm_lvl == 0) {

        if (!inSubMenu) {
            writeMenuP("11. Sist./Conf.", // String t0,
                       CUR_ENT, // float t1
                       speed_temp, // float t3
                       tensao, // float t4
                       CUR_OUT_TMP, // float t5							// float t5
						5, //op
						0 //casa decimal
                      );
        } else {
            //Serial.println("Inside Medio");
            if (digitalRead(btn_set_pin)) menu_flag = 0x01; //Botão Up pressionado? Seta flag
            if (digitalRead(btn_reset_pin)) enter_flag = 0x01; //Botão Down pressionado? Seta flag
            if (!digitalRead(btn_set_pin) && menu_flag && inSubMenu) //Botão Up solto e flag setada?
            {   //Sim...
                menu_flag = 0x00; //Limpa flag
                indiceSubMenu++; //Incrementa menu
                if (indiceSubMenu > 0x06) indiceSubMenu = 0x01; //Se menu maior que 3, volta a ser 1

            } //end menu

            switch (indiceSubMenu) {
            case 1:
			char buffer2[10];
            sprintf(buffer2, "%02d.%02d.%d", dataAtual.day(), dataAtual.month(), dataAtual.year());
                writeMenuS("11.1 Data/Hs", // String t0,
                           buffer2, // float t1
                           speed_temp, // float t3
                           tensao, // float t4
                           CUR_OUT_TMP // float t5							// float t5
                          );
                if (!digitalRead(btn_reset_pin) && enter_flag && inSubMenu) //Botão Down solto e flag setada?
                {   //Sim...
                    enter_flag = 0x00; //Limpa flag
                    indiceSubMenu = 2;
                    //Serial.println("Zerou!!!!!!!!!!!!");
                    /**************Aqui vamos fazer um jump para o menu insp. *******************/
                    indiceSubMenu = 1; // Retorna Posicao do Submenu pra 1
                    inSubMenu = 1; //
                    indiceMenu = 16; // ajDataHora
                    submenu_once = 1;
                    Serial.print("page 3");
                    sendEsc();

                } //end butreset			
				
				break;
            case 2:
                writeMenuS("11.2 Oleo/Km", // String t0,
                           (String) oil_run, // string
                           speed_temp, // float t3
                           tensao, // float t4
                           CUR_OUT_TMP
                          );
                if (!digitalRead(btn_reset_pin) && enter_flag && inSubMenu) //Botão Down solto e flag setada?
                {   //Sim...
                    enter_flag = 0x00; //Limpa flag
                    indiceSubMenu = 2;
                    //Serial.println("Zerou!!!!!!!!!!!!");
                    /**************Aqui vamos fazer um jump para o menu insp. *******************/
                    indiceSubMenu = 1; // Retorna Posicao do Submenu pra 1
                    inSubMenu = 1; //
                    indiceMenu = 13; // inspecao
                    submenu_once = 1;
                    Serial.print("page 3");
                    sendEsc();

                } //end butreset
                break;
            case 3:
                writeMenuP("11.3 Vss", // String t0,
                           pulse_distance, // float t1
                           speed_temp, // float t3
                           tensao, // float t4
                           CUR_OUT_TMP, // float t5							// float t5
						               0, //op
                          8 //casa decimal
                          );
                if (!digitalRead(btn_reset_pin) && enter_flag && inSubMenu) //Botão Down solto e flag setada?
                {   //Sim...
                    enter_flag = 0x00; //Limpa flag
                    indiceSubMenu = 2;
                    //Serial.println("Zerou!!!!!!!!!!!!");
                    /**************Aqui vamos fazer um jump para o menu insp. *******************/
                    indiceSubMenu = 1; // Retorna Posicao do Submenu pra 1
                    inSubMenu = 1; //
                    indiceMenu = 14; // Vss
                    submenu_once = 1;
                    Serial.print("page 3");
                    sendEsc();

                } //end butreset
                break;
            case 4:
                writeMenuP("11.4 Conf. Inj.", // String t0,
                           injection_value, // float t1
                           speed_temp, // float t3
                           tensao, // float t4
                           CUR_OUT_TMP, // float t5							// float t5
						   0, //op
						   8 //casa decimal
                          );
                if (!digitalRead(btn_reset_pin) && enter_flag && inSubMenu) //Botão Down solto e flag setada?
                {   //Sim...
                    enter_flag = 0x00; //Limpa flag
                    indiceSubMenu = 2;
                    //Serial.println("Zerou!!!!!!!!!!!!");
                    /**************Aqui vamos fazer um jump para o menu insp. *******************/
                    indiceSubMenu = 1; // Retorna Posicao do Submenu pra 1
                    inSubMenu = 1; //
                    indiceMenu = 15; // injector
                    submenu_once = 1;
                    Serial.print("page 3");
                    sendEsc();

                } //end butreset
                break;
            case 5:
                writeMenuP("11.5 Sensores", // String t0,
                           CUR_ENT, // float t1
                           speed_temp, // float t3
                           tensao, // float t4
                           CUR_OUT_TMP, // float t5							// float t5
						0, //op
						0 //casa decimal
                          );
                if (!digitalRead(btn_reset_pin) && enter_flag && inSubMenu) //Botão Down solto e flag setada?
                {   //Sim...
                    enter_flag = 0x00; //Limpa flag
                    indiceSubMenu = 1;
                    //Serial.println("Zerou!!!!!!!!!!!!");
                    /**************Aqui vamos fazer um jump para o menu sensores. *******************/
                    indiceSubMenu = 1; // Retorna Posicao do Submenu pra 1
                    inSubMenu = 1; //
                    indiceMenu = 12; // Sensores
                    submenu_once = 1;
                    Serial.print("page 3");
                    sendEsc();
                } //end butreset
                break;				
            case 6:
                //Serial.println("Sair");
                Serial.print("t0.txt=");
                Serial.write(0x22);
                Serial.print("11.6 Sair?");
                Serial.write(0x22);
                sendEsc();
                if (!digitalRead(btn_reset_pin) && enter_flag && inSubMenu) //Botão Down solto e flag setada?
                {   //Sim...
                    enter_flag = 0x00; //Limpa flag
                    //Serial.println("Saindo!!");
                    indiceSubMenu = 1; // Retorna Posicao do Submenu pra 1
                    inSubMenu = 0; //Sai do submenu
                } //end butreset
                break;
            } //fim switch (indiceSubMenu)
        } // fim else !inSubMenu

    } else { //else rpm == 0
        indiceSubMenu = 1; // Retorna Posicao do Submenu pra 1
        inSubMenu = 0; // Sai do submenu
        indiceMenu = 0x01; // Retorna a primeira posicao do menu principal
    } // fim if speed_temp

}
//----------- Fim Menu Config. ----------- //

//----------- Menu Sensores ----------- //
/*
 *  Configuração de sensores monitorados
 *  1 - Pastilhas
 *  2 - Nivel do Oleo
 *  3 - Nivel do Liq. Arrefecimento
 *  4 - Lavador
 *  5 - Transmissao
 *  6 - Lanterna-Farol
 *	7 - Luz de Freio 

*/

void sensorConfig() {
    if (submenu_once == 1) {
        //Imprime o menu inteiro uma única vez.
        Serial.print("t8.bco=");
        Serial.print(0);
        sendEsc();
        Serial.print("t8.txt=");
        Serial.write(0x22);
        Serial.print("11. Sensores");
        Serial.write(0x22);
        sendEsc();
        Serial.print("t0.txt=");
        Serial.write(0x22);
        Serial.print("1 - Pastilhas");
        Serial.write(0x22);
        sendEsc();
        Serial.print("t4.txt=");
        Serial.write(0x22);
        if (brake_pad_sen == 1) {
            Serial.print("On");
        } else {
            Serial.print("Off");
        }
        Serial.write(0x22);
        sendEsc();
//
        Serial.print("t1.txt=");
        Serial.write(0x22);
        Serial.print("2 - Oleo");
        Serial.write(0x22);
        sendEsc();
        Serial.print("t5.txt=");
        Serial.write(0x22);
        if (lvl_oil_sen == 1) {
            Serial.print("On");
        } else {
            Serial.print("Off");
        }
        Serial.write(0x22);
        sendEsc();
//
        Serial.print("t2.txt=");
        Serial.write(0x22);
        Serial.print("3 - Arrefecimento");
        Serial.write(0x22);
        sendEsc();
        Serial.print("t6.txt=");
        Serial.write(0x22);
        if (lvl_coolnt_sen == 1) {
            Serial.print("On");
        } else {
            Serial.print("Off");
        }
        Serial.write(0x22);
        sendEsc();
//
        Serial.print("t3.txt=");
        Serial.write(0x22);
        Serial.print("4 - Lavador");
        Serial.write(0x22);
        sendEsc();
        Serial.print("t7.txt=");
        Serial.write(0x22);
        if (lvl_wash_sen == 1) {
            Serial.print("On");
        } else {
            Serial.print("Off");
        }
        Serial.write(0x22);
        sendEsc();
        submenu_once = 0;
    }

    if (!inSubMenu) {

        //Imprime o menu inteiro
        Serial.print("t8.bco=");
        Serial.print(0);
        sendEsc();
        Serial.print("t8.txt=");
        Serial.write(0x22);
        Serial.print("11. Sensores");
        Serial.write(0x22);
        sendEsc();
        Serial.print("t0.txt=");
        Serial.write(0x22);
        Serial.print("1 - Pastilhas");
        Serial.write(0x22);
        sendEsc();
        Serial.print("t4.txt=");
        Serial.write(0x22);
        if (brake_pad_sen == 1) {
            Serial.print("On");
        } else {
            Serial.print("Off");
        }
        Serial.write(0x22);
        sendEsc();
//
        Serial.print("t1.txt=");
        Serial.write(0x22);
        Serial.print("2 - Oleo");
        Serial.write(0x22);
        sendEsc();
        Serial.print("t5.txt=");
        Serial.write(0x22);
        if (lvl_oil_sen == 1) {
            Serial.print("On");
        } else {
            Serial.print("Off");
        }
        Serial.write(0x22);
        sendEsc();
//
        Serial.print("t2.txt=");
        Serial.write(0x22);
        Serial.print("3 - Arrefec.");
        Serial.write(0x22);
        sendEsc();
        Serial.print("t6.txt=");
        Serial.write(0x22);
        if (lvl_coolnt_sen == 1) {
            Serial.print("On");
        } else {
            Serial.print("Off");
        }
        Serial.write(0x22);
        sendEsc();
//
        Serial.print("t3.txt=");
        Serial.write(0x22);
        Serial.print("4 - Lavador");
        Serial.write(0x22);
        sendEsc();
        Serial.print("t7.txt=");
        Serial.write(0x22);
        if (lvl_wash_sen == 1) {
            Serial.print("On");
        } else {
            Serial.print("Off");
        }
        Serial.write(0x22);
        sendEsc();

    } else {
        //Serial.println("Inside Medio");
        if (digitalRead(btn_set_pin)) menu_flag = 0x01; //Botão Up pressionado? Seta flag
        if (digitalRead(btn_reset_pin)) enter_flag = 0x01; //Botão Down pressionado? Seta flag
        if (!digitalRead(btn_set_pin) && menu_flag && inSubMenu) //Botão Up solto e flag setada?
        {   //Sim...
            menu_flag = 0x00; //Limpa flag
            indiceSubMenu++; //Incrementa menu
            if (indiceSubMenu > 0x08) indiceSubMenu = 0x01; //Se menu maior que 8, volta a ser 1

        } //end menu

        switch (indiceSubMenu) {
        case 1: //Pastilhas
            /*
            t0 t4
            t1 t5
            t2 t6
            t3 t7
            */
            Serial.print("t0.bco=");
            Serial.print(64520);
            sendEsc();
            Serial.print("t4.bco=");
            Serial.print(64520);
            sendEsc();
            //
            Serial.print("t8.bco=");
            Serial.print(0);
            sendEsc();
            Serial.print("t8.txt=");
            Serial.write(0x22);
            Serial.print("11. Sensores");
            Serial.write(0x22);
            sendEsc();
            Serial.print("t0.txt=");
            Serial.write(0x22);
            Serial.print("1 - Pastilhas");
            Serial.write(0x22);
            sendEsc();
            Serial.print("t4.txt=");
            Serial.write(0x22);
            if (brake_pad_sen == 1) {
                Serial.print("On");
            } else {
                Serial.print("Off");
            }
            Serial.write(0x22);
            sendEsc();

            if (!digitalRead(btn_reset_pin) && enter_flag && inSubMenu) //Botão Down solto e flag setada?
            {   //Sim...
                enter_flag = 0x00; //Limpa flag
                indiceSubMenu = 1;
                //Serial.println("Zerou!!!!!!!!!!!!");
                brake_pad_sen = !brake_pad_sen;
            } //end butreset
            break;

        case 2: //Nivel Oleo
            Serial.print("t0.bco=");
            Serial.print(50712);
            sendEsc();
            Serial.print("t4.bco=");
            Serial.print(50712);
            sendEsc();
            //
            Serial.print("t1.bco=");
            Serial.print(64520);
            sendEsc();
            Serial.print("t5.bco=");
            Serial.print(64520);
            sendEsc();
            //
            Serial.print("t8.bco=");
            Serial.print(0);
            sendEsc();
            Serial.print("t8.txt=");
            Serial.write(0x22);
            Serial.print("11. Sensores");
            Serial.write(0x22);
            sendEsc();
            Serial.print("t1.txt=");
            Serial.write(0x22);
            Serial.print("2 - Oleo");
            Serial.write(0x22);
            sendEsc();
            Serial.print("t5.txt=");
            Serial.write(0x22);
            if (lvl_oil_sen == 1) {
                Serial.print("On");
            } else {
                Serial.print("Off");
            }
            Serial.write(0x22);
            sendEsc();

            if (!digitalRead(btn_reset_pin) && enter_flag && inSubMenu) //Botão Down solto e flag setada?
            {   //Sim...
                enter_flag = 0x00; //Limpa flag
                indiceSubMenu = 2;
                //Serial.println("Zerou!!!!!!!!!!!!");
                lvl_oil_sen = !lvl_oil_sen;
            } //end butreset
            break;

        case 3: //Nivel arrefecimento
            Serial.print("t1.bco=");
            Serial.print(50712);
            sendEsc();
            Serial.print("t5.bco=");
            Serial.print(50712);
            sendEsc();
            //
            Serial.print("t2.bco=");
            Serial.print(64520);
            sendEsc();
            Serial.print("t6.bco=");
            Serial.print(64520);
            sendEsc();
            //
            Serial.print("t8.bco=");
            Serial.print(0);
            sendEsc();
            Serial.print("t8.txt=");
            Serial.write(0x22);
            Serial.print("11. Sensores");
            Serial.write(0x22);
            sendEsc();
            Serial.print("t2.txt=");
            Serial.write(0x22);
            Serial.print("3 - Liq. Arref.");
            Serial.write(0x22);
            sendEsc();
            Serial.print("t6.txt=");
            Serial.write(0x22);
            if (lvl_coolnt_sen == 1) {
                Serial.print("On");
            } else {
                Serial.print("Off");
            }
            Serial.write(0x22);
            sendEsc();

            if (!digitalRead(btn_reset_pin) && enter_flag && inSubMenu) //Botão Down solto e flag setada?
            {   //Sim...
                enter_flag = 0x00; //Limpa flag
                indiceSubMenu = 3;
                //Serial.println("Zerou!!!!!!!!!!!!");
                lvl_coolnt_sen = !lvl_coolnt_sen;
            } //end butreset
            break;

        case 4:
            Serial.print("t2.bco=");
            Serial.print(50712);
            sendEsc();
            Serial.print("t6.bco=");
            Serial.print(50712);
            sendEsc();
            //
            Serial.print("t3.bco=");
            Serial.print(64520);
            sendEsc();
            Serial.print("t7.bco=");
            Serial.print(64520);
            sendEsc();
            //
            Serial.print("t8.bco=");
            Serial.print(0);
            sendEsc();
            Serial.print("t8.txt=");
            Serial.write(0x22);
            Serial.print("11. Sensores");
            Serial.write(0x22);
            sendEsc();
            Serial.print("t3.txt=");
            Serial.write(0x22);
            Serial.print("4 - Niv. Lavador");
            Serial.write(0x22);
            sendEsc();
            Serial.print("t7.txt=");
            Serial.write(0x22);
            if (lvl_wash_sen == 1) {
                Serial.print("On");
            } else {
                Serial.print("Off");
            }
            Serial.write(0x22);
            sendEsc();

            if (!digitalRead(btn_reset_pin) && enter_flag && inSubMenu) //Botão Down solto e flag setada?
            {   //Sim...
                enter_flag = 0x00; //Limpa flag
                indiceSubMenu = 4;
                //Serial.println("Zerou!!!!!!!!!!!!");
                lvl_wash_sen = !lvl_wash_sen;
            } //end butreset
            break;

        case 5: //Transmissao
            //
            Serial.print("t3.bco=");
            Serial.print(50712);
            sendEsc();
            Serial.print("t7.bco=");
            Serial.print(50712);
            sendEsc();
            //
            Serial.print("t0.bco=");
            Serial.print(64520);
            sendEsc();
            Serial.print("t4.bco=");
            Serial.print(64520);
            sendEsc();
            //
            Serial.print("t8.bco=");
            Serial.print(0);
            sendEsc();
            Serial.print("t8.txt=");
            Serial.write(0x22);
            Serial.print("11. Sensores");
            Serial.write(0x22);
            sendEsc();
            Serial.print("t0.txt=");
            Serial.write(0x22);
            Serial.print("5 - Trans. AT");
            Serial.write(0x22);
            sendEsc();
            Serial.print("t4.txt=");
            Serial.write(0x22);
            if (trans_status_sen == 1) {
                Serial.print("On");
            } else {
                Serial.print("Off");
            }
            Serial.write(0x22);
            sendEsc();

            if (!digitalRead(btn_reset_pin) && enter_flag && inSubMenu) //Botão Down solto e flag setada?
            {   //Sim...
                enter_flag = 0x00; //Limpa flag
                indiceSubMenu = 5;
                //Serial.println("Zerou!!!!!!!!!!!!");
                trans_status_sen = !trans_status_sen;
            } //end butreset
            break;

        case 6: //Lanterna-Farol
            //
            Serial.print("t0.bco=");
            Serial.print(50712);
            sendEsc();
            Serial.print("t4.bco=");
            Serial.print(50712);
            sendEsc();
            //
            Serial.print("t1.bco=");
            Serial.print(64520);
            sendEsc();
            Serial.print("t5.bco=");
            Serial.print(64520);
            sendEsc();
            //
            Serial.print("t8.bco=");
            Serial.print(0);
            sendEsc();
            Serial.print("t8.txt=");
            Serial.write(0x22);
            Serial.print("11. Sensores");
            Serial.write(0x22);
            sendEsc();
            Serial.print("t1.txt=");
            Serial.write(0x22);
            Serial.print("6 - Lant. Farol.");
            Serial.write(0x22);
            sendEsc();
            Serial.print("t5.txt=");
            Serial.write(0x22);
            if (lant_p1_sen == 1) {
                Serial.print("On");
            } else {
                Serial.print("Off");
            }
            Serial.write(0x22);
            sendEsc();

            if (!digitalRead(btn_reset_pin) && enter_flag && inSubMenu) //Botão Down solto e flag setada?
            {   //Sim...
                enter_flag = 0x00; //Limpa flag
                indiceSubMenu = 6;
                //Serial.println("Zerou!!!!!!!!!!!!");
                lant_p1_sen = !lant_p1_sen;
            } //end butreset
            break;			


        case 7: //Luz de Freio
            Serial.print("t1.bco=");
            Serial.print(50712);
            sendEsc();
            Serial.print("t5.bco=");
            Serial.print(50712);
            sendEsc();
            //
            Serial.print("t2.bco=");
            Serial.print(64520);
            sendEsc();
            Serial.print("t6.bco=");
            Serial.print(64520);
            sendEsc();
            //
            Serial.print("t8.bco=");
            Serial.print(0);
            sendEsc();
            Serial.print("t8.txt=");
            Serial.write(0x22);
            Serial.print("11. Sensores");
            Serial.write(0x22);
            sendEsc();
            Serial.print("t2.txt=");
            Serial.write(0x22);
            Serial.print("7 - Luz de Freio");
            Serial.write(0x22);
            sendEsc();
            Serial.print("t6.txt=");
            Serial.write(0x22);
            if (freio_p1_sen == 1) {
                Serial.print("On");
            } else {
                Serial.print("Off");
            }
            Serial.write(0x22);
            sendEsc();

            if (!digitalRead(btn_reset_pin) && enter_flag && inSubMenu) //Botão Down solto e flag setada?
            {   //Sim...
                enter_flag = 0x00; //Limpa flag
                indiceSubMenu = 7;
                //Serial.println("Zerou!!!!!!!!!!!!");
                freio_p1_sen = !freio_p1_sen;
            } //end butreset
            break;

			
        case 8: /* saida */
//
            Serial.print("t2.bco=");
            Serial.print(50712);
            sendEsc();
            Serial.print("t6.bco=");
            Serial.print(50712);
            sendEsc();
//
            Serial.print("t8.txt=");
            Serial.write(0x22);
            Serial.print("Sair?");
            Serial.write(0x22);
            sendEsc();
            Serial.print("t8.bco=");
            Serial.print(63488);
            sendEsc();
            if (!digitalRead(btn_reset_pin) && enter_flag && inSubMenu) //Botão Down solto e flag setada?
            {   //Sim...
                enter_flag = 0x00; //Limpa flag
                //Serial.println("Saindo!!");
                indiceSubMenu = 5; // Retorna Posicao do Submenu pra 1
                inSubMenu = 1; //Sai do submenu
                //retorna menu 9
                indiceMenu = 11;
                retorno_menu_1 = 0; //Reaparece page 1
            } //end butreset
            break;
        } //fim switch (indiceSubMenu)
    } // fim else !inSubMenu
}
//----------- Menu Sensores ----------- //

//----------- Menu inspecao ----------- //
/*
Troca de oleo
1 - Intervalo / Status
2.Reset/Troca 
3.Aum. + km
3.Dim. - km
5 - Sair 

*/
void inspConfig() {
	
	char buffer[10];
  int oil_run_int = oil_run;
	sprintf(buffer, "Prox. Troca. %d", oil_run_int);
	String oilInterv = ("1.Aviso: "+ (String) oil_change +"Km");

    if (submenu_once == 1) {
        //Imprime o menu inteiro uma única vez.
        Serial.print("t8.bco=");
        Serial.print(0);
        sendEsc();
        Serial.print("t8.txt=");
        Serial.write(0x22);
        Serial.print((String) buffer);
		Serial.write(0x22);
        sendEsc();
        Serial.print("t0.txt=");
        Serial.write(0x22);
        Serial.print(oilInterv);
        Serial.write(0x22);
        sendEsc();
        Serial.print("t4.txt=");
        Serial.write(0x22);
        if (oil_insp_sen == 1) {
            Serial.print("On");
        } else {
            Serial.print("Off");
        }
        Serial.write(0x22);
        sendEsc();
//
        Serial.print("t1.txt=");
        Serial.write(0x22);
        Serial.print("2.Resetar");
        Serial.write(0x22);
        sendEsc();
        Serial.print("t5.txt=");
        Serial.write(0x22);
        Serial.print(oil_run);
        Serial.write(0x22);
        sendEsc();
//
        Serial.print("t2.txt=");
        Serial.write(0x22);
        Serial.print("3.Aum. + km");
        Serial.write(0x22);
        sendEsc();
        Serial.print("t6.txt=");
        Serial.write(0x22);
        Serial.print("");
        Serial.write(0x22);
        sendEsc();
//
        Serial.print("t3.txt=");
        Serial.write(0x22);
        Serial.print("4.Dim. - km");
        Serial.write(0x22);
        sendEsc();
        Serial.print("t7.txt=");
        Serial.write(0x22);
        Serial.print("");
        Serial.write(0x22);
        sendEsc();
        submenu_once = 0;
    }

    if (!inSubMenu) {
        //Imprime o menu inteiro uma única vez.
        Serial.print("t8.bco=");
        Serial.print(0);
        sendEsc();
        Serial.print("t8.txt=");
        Serial.write(0x22);
        Serial.print((String) buffer);
        Serial.write(0x22);
        sendEsc();
        Serial.print("t0.txt=");
        Serial.write(0x22);
        Serial.print(oilInterv);
        Serial.write(0x22);
        sendEsc();
        Serial.print("t4.txt=");
        Serial.write(0x22);
        if (oil_insp_sen == 1) {
            Serial.print("On");
        } else {
            Serial.print("Off");
        }
        Serial.write(0x22);
        sendEsc();
//
        Serial.print("t1.txt=");
        Serial.write(0x22);
        Serial.print("2.Resetar");
        Serial.write(0x22);
        sendEsc();
        Serial.print("t5.txt=");
        Serial.write(0x22);
        Serial.print(oil_run);
        Serial.write(0x22);
        sendEsc();
//
        Serial.print("t2.txt=");
        Serial.write(0x22);
        Serial.print("3.Aum. + km");
        Serial.write(0x22);
        sendEsc();
        Serial.print("t6.txt=");
        Serial.write(0x22);
        Serial.print("");
        Serial.write(0x22);
        sendEsc();
//
        Serial.print("t3.txt=");
        Serial.write(0x22);
        Serial.print("4.Dim. - km");
        Serial.write(0x22);
        sendEsc();
        Serial.print("t7.txt=");
        Serial.write(0x22);
        Serial.print("");
        Serial.write(0x22);
        sendEsc();

    } else {
        //Serial.println("Inside Medio");
        if (digitalRead(btn_set_pin)) menu_flag = 0x01; //Botão Up pressionado? Seta flag
        if (digitalRead(btn_reset_pin)) enter_flag = 0x01; //Botão Down pressionado? Seta flag
        if (!digitalRead(btn_set_pin) && menu_flag && inSubMenu) //Botão Up solto e flag setada?
        {   //Sim...
            menu_flag = 0x00; //Limpa flag
            indiceSubMenu++; //Incrementa menu
            if (indiceSubMenu > 0x05) indiceSubMenu = 0x01; //Se menu maior que 3, volta a ser 1

        } //end menu

        switch (indiceSubMenu) {
        case 1: //Intervalo/Status
            /*
            t0 t4
            t1 t5
            t2 t6
            t3 t7
            */
            Serial.print("t0.bco=");
            Serial.print(64520);
            sendEsc();
            Serial.print("t4.bco=");
            Serial.print(64520);
            sendEsc();
            //
            Serial.print("t8.bco=");
            Serial.print(0);
            sendEsc();
            Serial.print("t8.txt=");
            Serial.write(0x22);
            Serial.print((String) buffer);
            Serial.write(0x22);
            sendEsc();
            Serial.print("t0.txt=");
            Serial.write(0x22);
            Serial.print(oilInterv);
            Serial.write(0x22);
            sendEsc();
            Serial.print("t4.txt=");
            Serial.write(0x22);
			if (oil_insp_sen == 1) {
            Serial.print("On");
			} else {
            Serial.print("Off");
			}
            Serial.write(0x22);
            sendEsc();

            if (!digitalRead(btn_reset_pin) && enter_flag && inSubMenu) //Botão Down solto e flag setada?
            {   //Sim...
                enter_flag = 0x00; //Limpa flag
                indiceSubMenu = 1;
                //Serial.println("Zerou!!!!!!!!!!!!");
                oil_insp_sen = !oil_insp_sen;
            } //end butreset
            break;

        case 2: //Resetar
            Serial.print("t0.bco=");
            Serial.print(50712);
            sendEsc();
            Serial.print("t4.bco=");
            Serial.print(50712);
            sendEsc();
            //
            Serial.print("t1.bco=");
            Serial.print(64520);
            sendEsc();
            Serial.print("t5.bco=");
            Serial.print(64520);
            sendEsc();
            //
            Serial.print("t8.bco=");
            Serial.print(0);
            sendEsc();
            Serial.print("t8.txt=");
            Serial.write(0x22);
            Serial.print((String) buffer);
            Serial.write(0x22);
            sendEsc();
            Serial.print("t1.txt=");
            Serial.write(0x22);
            Serial.print("2.Resetar");
            Serial.write(0x22);
            sendEsc();
            Serial.print("t5.txt=");
            Serial.write(0x22);
            Serial.print(oil_run);
            Serial.write(0x22);
            sendEsc();

            if (!digitalRead(btn_reset_pin) && enter_flag && inSubMenu) //Botão Down solto e flag setada?
            {   //Sim...
                enter_flag = 0x00; //Limpa flag
                indiceSubMenu = 6;
                //Serial.println("Zerou!!!!!!!!!!!!");
				//reset km do oleo
				oil_run = 0;
				//update na tela
				Serial.print("t5.bco=");
				Serial.print(50712);
				sendEsc();
				Serial.print("t1.bco=");
				Serial.print(50712);
				sendEsc();
				Serial.print("t1.txt=");
				Serial.write(0x22);
				Serial.print("2.Resetado!");
				Serial.write(0x22);
				sendEsc();				
            } //end butreset
            break;



        case 3: //Aumentar intervalo oleo
            Serial.print("t1.bco=");
            Serial.print(50712);
            sendEsc();
            Serial.print("t5.bco=");
            Serial.print(50712);
            sendEsc();
            //
            Serial.print("t2.bco=");
            Serial.print(64520);
            sendEsc();
            Serial.print("t6.bco=");
            Serial.print(64520);
            sendEsc();
			//
			Serial.print("t0.txt=");
			Serial.write(0x22);
			Serial.print(oilInterv);
			Serial.write(0x22);
			sendEsc();
			Serial.print("t4.txt=");
			Serial.write(0x22);
			if (oil_insp_sen == 1) {
            Serial.print("On");
			} else {
            Serial.print("Off");
			}
			Serial.write(0x22);
			sendEsc();
            //
            Serial.print("t8.bco=");
            Serial.print(0);
            sendEsc();
            Serial.print("t8.txt=");
            Serial.write(0x22);
            Serial.print((String) buffer);
            Serial.write(0x22);
            sendEsc();
            Serial.print("t2.txt=");
            Serial.write(0x22);
            Serial.print("3.Aum. + km");
            Serial.write(0x22);
            sendEsc();
            Serial.print("t6.txt=");
            Serial.write(0x22);
            Serial.print("");
            Serial.write(0x22);
            sendEsc();

            if (!digitalRead(btn_reset_pin) && enter_flag && inSubMenu) //Botão Down solto e flag setada?
            {   //Sim...
                enter_flag = 0x00; //Limpa flag
                indiceSubMenu = 3;
                //Serial.println("Zerou!!!!!!!!!!!!");
				if(oil_change < 15000) {
                    oil_change += 500;
                }
            } //end butreset
            break;

       case 4: //Diminuir intervalo oleo
            Serial.print("t2.bco=");
            Serial.print(50712);
            sendEsc();
            Serial.print("t6.bco=");
            Serial.print(50712);
            sendEsc();
            //
            Serial.print("t3.bco=");
            Serial.print(64520);
            sendEsc();
            Serial.print("t7.bco=");
            Serial.print(64520);
            sendEsc();
			//
			Serial.print("t0.txt=");
			Serial.write(0x22);
			Serial.print(oilInterv);
			Serial.write(0x22);
			sendEsc();
			Serial.print("t4.txt=");
			Serial.write(0x22);
			if (oil_insp_sen == 1) {
            Serial.print("On");
			} else {
            Serial.print("Off");
			}
			Serial.write(0x22);
			sendEsc();
			//
            Serial.print("t8.bco=");
            Serial.print(0);
            sendEsc();
            Serial.print("t8.txt=");
            Serial.write(0x22);
            Serial.print((String) buffer);
            Serial.write(0x22);
            sendEsc();
            Serial.print("t3.txt=");
            Serial.write(0x22);
            Serial.print("3.Dim. - km");
            Serial.write(0x22);
            sendEsc();
            Serial.print("t7.txt=");
            Serial.write(0x22);
            Serial.print("");
            Serial.write(0x22);
            sendEsc();

            if (!digitalRead(btn_reset_pin) && enter_flag && inSubMenu) //Botão Down solto e flag setada?
            {   //Sim...
                enter_flag = 0x00; //Limpa flag
                indiceSubMenu = 4;
                //Serial.println("Zerou!!!!!!!!!!!!");
                if(oil_change >= 501) {
                    oil_change -= 500;
                }
            } //end butreset
            break;

		case 5: //Sair
            Serial.print("t0.bco=");
            Serial.print(50712);
            sendEsc();
            Serial.print("t4.bco=");
            Serial.print(50712);
            sendEsc();
//
            Serial.print("t8.txt=");
            Serial.write(0x22);
            Serial.print("Sair?");
            Serial.write(0x22);
            sendEsc();
            Serial.print("t8.bco=");
            Serial.print(63488);
            sendEsc();
            if (!digitalRead(btn_reset_pin) && enter_flag && inSubMenu) //Botão Down solto e flag setada?
            {   //Sim...
                enter_flag = 0x00; //Limpa flag
                //Serial.println("Saindo!!");
                indiceSubMenu = 2; // Retorna Posicao do Submenu pra 1
                inSubMenu = 1; //Sai do submenu
                //retorna menu 9
                indiceMenu = 11;
                retorno_menu_1 = 0; //Reaparece page 1
            } //end butreset
            break;

        } //fim switch (indiceSubMenu)
    } // fim else !inSubMenu
}
//----------- Config VSS ----------- //
void vssConfig() {
/*
1 - Aumentar
2 - Diminuir
3 - Sair
*/  
    if (submenu_once == 1) {
        //Imprime o menu inteiro uma única vez.
        Serial.print("t8.bco=");
        Serial.print(0);
        sendEsc();
        Serial.print("t8.txt=");
        Serial.write(0x22);
        Serial.print("VSS.");
        Serial.write(0x22);
        sendEsc();
        Serial.print("t0.txt=");
        Serial.write(0x22);
        Serial.print(vss);
        Serial.write(0x22);
        sendEsc();
        Serial.print("t4.txt=");
        Serial.write(0x22);
        Serial.print("Pulsos");
        Serial.write(0x22);
        sendEsc();
//
        Serial.print("t1.txt=");
        Serial.write(0x22);
        Serial.print("1 - Aumentar");
        Serial.write(0x22);
        sendEsc();
        Serial.print("t5.txt=");
        Serial.write(0x22);
        Serial.print("");
        Serial.write(0x22);
        sendEsc();
//
        Serial.print("t2.txt=");
        Serial.write(0x22);
        Serial.print("2 - Diminuir");
        Serial.write(0x22);
        sendEsc();
        Serial.print("t6.txt=");
        Serial.write(0x22);
        Serial.print("");
        Serial.write(0x22);
        sendEsc();
        submenu_once = 0;
    } 

    if (!inSubMenu) {
        //Imprime o menu inteiro uma única vez.
        Serial.print("t8.bco=");
        Serial.print(0);
        sendEsc();
        Serial.print("t8.txt=");
        Serial.write(0x22);
        Serial.print("VSS.");
        Serial.write(0x22);
        sendEsc();
        Serial.print("t0.txt=");
        Serial.write(0x22);
        Serial.print(vss);
        Serial.write(0x22);
        sendEsc();
        Serial.print("t4.txt=");
        Serial.write(0x22);
        Serial.print("Pulsos");
        Serial.write(0x22);
        sendEsc();
//
        Serial.print("t1.txt=");
        Serial.write(0x22);
        Serial.print("1 - Aumentar");
        Serial.write(0x22);
        sendEsc();
        Serial.print("t5.txt=");
        Serial.write(0x22);
        Serial.print("");
        Serial.write(0x22);
        sendEsc();
//
        Serial.print("t2.txt=");
        Serial.write(0x22);
        Serial.print("2 - Diminuir");
        Serial.write(0x22);
        sendEsc();
        Serial.print("t6.txt=");
        Serial.write(0x22);
        Serial.print("");
        Serial.write(0x22);
        sendEsc();
    } else {
        //Serial.println("Inside Medio");
        if (digitalRead(btn_set_pin)) menu_flag = 0x01; //Botão Up pressionado? Seta flag
        if (digitalRead(btn_reset_pin)) enter_flag = 0x01; //Botão Down pressionado? Seta flag
        if (!digitalRead(btn_set_pin) && menu_flag && inSubMenu) //Botão Up solto e flag setada?
        {   //Sim...
            menu_flag = 0x00; //Limpa flag
            indiceSubMenu++; //Incrementa menu
            if (indiceSubMenu > 0x03) indiceSubMenu = 0x01; //Se menu maior que 3, volta a ser 1

        } //end menu

        switch (indiceSubMenu) {
        case 1: //Aumentar
            /*
            t0 t4
            t1 t5
            t2 t6
            t3 t7
            */
			Serial.print("t8.bco=");
			Serial.print(0);
			sendEsc();
			Serial.print("t8.txt=");
			Serial.write(0x22);
			Serial.print("VSS.");
			Serial.write(0x22);
			sendEsc();
			//
            Serial.print("t1.bco=");
            Serial.print(64520);
            sendEsc();
            Serial.print("t5.bco=");
            Serial.print(64520);
            sendEsc();
            //
			Serial.print("t0.txt=");
			Serial.write(0x22);
			Serial.print(vss);
			Serial.write(0x22);
			sendEsc();
			Serial.print("t4.txt=");
			Serial.write(0x22);
			Serial.print("Pulsos");
			Serial.write(0x22);
			sendEsc();		
			//	
			Serial.print("t1.txt=");
			Serial.write(0x22);
			Serial.print("1 - Aumentar");
			Serial.write(0x22);
			sendEsc();
			Serial.print("t5.txt=");
			Serial.write(0x22);
			Serial.print("");
			Serial.write(0x22);
			sendEsc();

            if (!digitalRead(btn_reset_pin) && enter_flag && inSubMenu) //Botão Down solto e flag setada?
            {   //Sim...
                enter_flag = 0x00; //Limpa flag
                indiceSubMenu = 1;
                //Serial.println("Zerou!!!!!!!!!!!!");
                vss = vss+1;
            } //end butreset
            break;

        case 2: //Diminuir
            Serial.print("t1.bco=");
            Serial.print(50712);
            sendEsc();
            Serial.print("t5.bco=");
            Serial.print(50712);
            sendEsc();
            //
            Serial.print("t2.bco=");
            Serial.print(64520);
            sendEsc();
            Serial.print("t6.bco=");
            Serial.print(64520);
            sendEsc();
            //
			Serial.print("t0.txt=");
			Serial.write(0x22);
			Serial.print(vss);
			Serial.write(0x22);
			sendEsc();
			Serial.print("t4.txt=");
			Serial.write(0x22);
			Serial.print("Pulsos");
			Serial.write(0x22);
			sendEsc();		
			//	
			Serial.print("t2.txt=");
			Serial.write(0x22);
			Serial.print("2 - Diminuir");
			Serial.write(0x22);
			sendEsc();
			Serial.print("t6.txt=");
			Serial.write(0x22);
			Serial.print("");
			Serial.write(0x22);

            if (!digitalRead(btn_reset_pin) && enter_flag && inSubMenu) //Botão Down solto e flag setada?
            {   //Sim...
                enter_flag = 0x00; //Limpa flag
                indiceSubMenu = 2;
                //Serial.println("Zerou!!!!!!!!!!!!");
                vss = vss-1;
            } //end butreset
            break;


        case 3: /* saida */
			//
            Serial.print("t2.bco=");
            Serial.print(50712);
            sendEsc();
            Serial.print("t6.bco=");
            Serial.print(50712);
            sendEsc();
			//
            Serial.print("t8.txt=");
            Serial.write(0x22);
            Serial.print("Sair?");
            Serial.write(0x22);
            sendEsc();
            Serial.print("t8.bco=");
            Serial.print(63488);
            sendEsc();
            if (!digitalRead(btn_reset_pin) && enter_flag && inSubMenu) //Botão Down solto e flag setada?
            {   //Sim...
                enter_flag = 0x00; //Limpa flag
                //Serial.println("Saindo!!");
                //atualizacao.
				updateVss();
				indiceSubMenu = 3; // Retorna Posicao do Submenu pra 1
                inSubMenu = 1; //Sai do submenu
                //retorna menu 9
                indiceMenu = 11;
                retorno_menu_1 = 0; //Reaparece page 1
            } //end butreset
            break;
        } //fim switch (indiceSubMenu)
    } // fim else !inSubMenu

}//----------- Fim vss Config ----------- //

//----------- Config injector ----------- //
void injectorConfig () {
/*
1 - Pulsos +
2 - Pulsos -
3 - L/h Val+
4 - L/h Val-
5 - Sair
*/  
    if (submenu_once == 1) {
        //Imprime o menu inteiro uma única vez.
        Serial.print("t8.bco=");
        Serial.print(0);
        sendEsc();
        Serial.print("t8.txt=");
        Serial.write(0x22);
        Serial.print("Conf. Consumo");
        Serial.write(0x22);
        sendEsc();
        Serial.print("t0.txt=");
        Serial.write(0x22);
        Serial.print("1 - Pulsos +");
        Serial.write(0x22);
        sendEsc();
        Serial.print("t4.txt=");
        Serial.write(0x22);
        Serial.print("");
        Serial.write(0x22);
        sendEsc();
//
        Serial.print("t1.txt=");
        Serial.write(0x22);
        Serial.print("2 - Pulsos -");
        Serial.write(0x22);
        sendEsc();
        Serial.print("t5.txt=");
        Serial.write(0x22);
        Serial.print("");
        Serial.write(0x22);
        sendEsc();

        submenu_once = 0;
    }

    if (!inSubMenu) {

        //Imprime o menu inteiro
        Serial.print("t8.bco=");
        Serial.print(0);
        sendEsc();
        Serial.print("t8.txt=");
        Serial.write(0x22);
        Serial.print("Conf. Consumo");
        Serial.write(0x22);
        sendEsc();
        Serial.print("t0.txt=");
        Serial.write(0x22);
        Serial.print("1 - Pulsos +");
        Serial.write(0x22);
        sendEsc();
        Serial.print("t4.txt=");
        Serial.write(0x22);
        Serial.print("");
        Serial.write(0x22);
        sendEsc();
//
        Serial.print("t1.txt=");
        Serial.write(0x22);
        Serial.print("2 - Pulsos -");
        Serial.write(0x22);
        sendEsc();
        Serial.print("t5.txt=");
        Serial.write(0x22);
        Serial.print("");
        Serial.write(0x22);
        sendEsc();
//

    } else {
        //Serial.println("Inside Medio");
        if (digitalRead(btn_set_pin)) menu_flag = 0x01; //Botão Up pressionado? Seta flag
        if (digitalRead(btn_reset_pin)) enter_flag = 0x01; //Botão Down pressionado? Seta flag
        if (!digitalRead(btn_set_pin) && menu_flag && inSubMenu) //Botão Up solto e flag setada?
        {   //Sim...
            menu_flag = 0x00; //Limpa flag
            indiceSubMenu++; //Incrementa menu
            if (indiceSubMenu > 0x03) indiceSubMenu = 0x01; //Se menu maior que 8, volta a ser 1

        } //end menu

        switch (indiceSubMenu) {
        case 1: //1 - Pulsos +
            /*
            t0 t4
            t1 t5
            t2 t6
            t3 t7
            */
            Serial.print("t0.bco=");
            Serial.print(64520);
            sendEsc();
            Serial.print("t4.bco=");
            Serial.print(64520);
            sendEsc();
            //
            Serial.print("t8.bco=");
            Serial.print(0);
            sendEsc();
            Serial.print("t8.txt=");
            Serial.write(0x22);
            Serial.print(ref_mid_pulse);
            Serial.write(0x22);
            sendEsc();
            Serial.print("t0.txt=");
            Serial.write(0x22);
            Serial.print("1 - Pulsos +");
            Serial.write(0x22);
            sendEsc();
            Serial.print("t4.txt=");
            Serial.write(0x22);
            Serial.print("");
            Serial.write(0x22);
            sendEsc();

            if (!digitalRead(btn_reset_pin) && enter_flag && inSubMenu) //Botão Down solto e flag setada?
            {   //Sim...
                enter_flag = 0x00; //Limpa flag
                indiceSubMenu = 1;
                //Serial.println("Zerou!!!!!!!!!!!!");
                 ref_mid_pulse++;
            } //end butreset
            break;

        case 2: //2 - Pulsos -
            Serial.print("t0.bco=");
            Serial.print(50712);
            sendEsc();
            Serial.print("t4.bco=");
            Serial.print(50712);
            sendEsc();
            //
            Serial.print("t1.bco=");
            Serial.print(64520);
            sendEsc();
            Serial.print("t5.bco=");
            Serial.print(64520);
            sendEsc();
            //
            Serial.print("t8.bco=");
            Serial.print(0);
            sendEsc();
            Serial.print("t8.txt=");
            Serial.write(0x22);
            Serial.print(ref_mid_pulse);
            Serial.write(0x22);
            sendEsc();
            Serial.print("t1.txt=");
            Serial.write(0x22);
            Serial.print("2 - Pulsos -");
            Serial.write(0x22);
            sendEsc();
            Serial.print("t5.txt=");
            Serial.write(0x22);
            Serial.print("");
            Serial.write(0x22);
            sendEsc();

            if (!digitalRead(btn_reset_pin) && enter_flag && inSubMenu) //Botão Down solto e flag setada?
            {   //Sim...
                enter_flag = 0x00; //Limpa flag
                indiceSubMenu = 2;
                //Serial.println("Zerou!!!!!!!!!!!!");
                ref_mid_pulse--;
            } //end butreset
            break;

	
        case 3: /* saida */
//
            Serial.print("t1.bco=");
            Serial.print(50712);
            sendEsc();
            Serial.print("t5.bco=");
            Serial.print(50712);
            sendEsc();
//
            Serial.print("t8.txt=");
            Serial.write(0x22);
            Serial.print("Sair?");
            Serial.write(0x22);
            sendEsc();
            Serial.print("t8.bco=");
            Serial.print(63488);
            sendEsc();
            if (!digitalRead(btn_reset_pin) && enter_flag && inSubMenu) //Botão Down solto e flag setada?
            {   //Sim...
                enter_flag = 0x00; //Limpa flag
                //Serial.println("Saindo!!");
				//atualizacao
				updateInjection();
                indiceSubMenu = 4; // Retorna Posicao do Submenu pra 4
                inSubMenu = 5; //Sai do submenu
                //retorna menu 9
                indiceMenu = 11;
                retorno_menu_1 = 0; //Reaparece page 1
            } //end butreset
            break;
        } //fim switch (indiceSubMenu)
    } // fim else !inSubMenu
}
//----------- Fim Config injector ----------- //
//----------- Menu ajDataHora ----------- //
/*
1 - Hora
2 - Minuto 
3 - Dia	
4 - Mes
5 - Ano +
6 - Ano -
7 - Sair 

*/
void ajDataHora() {

    if (submenu_once == 1) {
        
		//chama as variaveis do relogio uma unica vez.
		ano = dataAtual.year();
		mes = dataAtual.month();
		dia = dataAtual.day();
		hora = dataAtual.hour();
		minuto = dataAtual.minute();
		//Imprime o menu inteiro uma única vez.
		Serial.print("t8.bco=");
        Serial.print(0);
        sendEsc();
        Serial.print("t8.txt=");
        Serial.write(0x22);
        Serial.print("Aj. Dt/hs");
        Serial.write(0x22);
        sendEsc();
        Serial.print("t0.txt=");
        Serial.write(0x22);
        Serial.print("1 - Hora");
        Serial.write(0x22);
        sendEsc();
        Serial.print("t4.txt=");
        Serial.write(0x22);
        Serial.print(hora);
        Serial.write(0x22);
        sendEsc();
//
        Serial.print("t1.txt=");
        Serial.write(0x22);
        Serial.print("2 - Min");
        Serial.write(0x22);
        sendEsc();
        Serial.print("t5.txt=");
        Serial.write(0x22);
        Serial.print(minuto);
        Serial.write(0x22);
        sendEsc();
//
        Serial.print("t2.txt=");
        Serial.write(0x22);
        Serial.print("3 - Dia");
        Serial.write(0x22);
        sendEsc();
        Serial.print("t6.txt=");
        Serial.write(0x22);
        Serial.print(dia);
        Serial.write(0x22);
        sendEsc();
//
        Serial.print("t3.txt=");
        Serial.write(0x22);
        Serial.print("4 - Mes");
        Serial.write(0x22);
        sendEsc();
        Serial.print("t7.txt=");
        Serial.write(0x22);
        Serial.print(mes);
        Serial.write(0x22);
        sendEsc();
        submenu_once = 0;
    }

    if (!inSubMenu) {
        //Imprime o menu inteiro uma única vez.
        Serial.print("t8.bco=");
        Serial.print(0);
        sendEsc();
        Serial.print("t8.txt=");
        Serial.write(0x22);
        Serial.print("Aj. Dt/hs");
        Serial.write(0x22);
        sendEsc();
        Serial.print("t0.txt=");
        Serial.write(0x22);
        Serial.print("1- Hora");
        Serial.write(0x22);
        sendEsc();
        Serial.print("t4.txt=");
        Serial.write(0x22);
        Serial.print(hora);
        Serial.write(0x22);
        sendEsc();
//
        Serial.print("t1.txt=");
        Serial.write(0x22);
        Serial.print("2 - Min");
        Serial.write(0x22);
        sendEsc();
        Serial.print("t5.txt=");
        Serial.write(0x22);
        Serial.print(minuto);
        Serial.write(0x22);
        sendEsc();
//
        Serial.print("t2.txt=");
        Serial.write(0x22);
        Serial.print("3 - Dia");
        Serial.write(0x22);
        sendEsc();
        Serial.print("t6.txt=");
        Serial.write(0x22);
        Serial.print(dia);
        Serial.write(0x22);
        sendEsc();
//
        Serial.print("t3.txt=");
        Serial.write(0x22);
        Serial.print("4 - Mes");
        Serial.write(0x22);
        sendEsc();
        Serial.print("t7.txt=");
        Serial.write(0x22);
        Serial.print(mes);
        Serial.write(0x22);
        sendEsc();

    } else {
        //Serial.println("Inside datahira");
        if (digitalRead(btn_set_pin)) menu_flag = 0x01; //Botão Up pressionado? Seta flag
        if (digitalRead(btn_reset_pin)) enter_flag = 0x01; //Botão Down pressionado? Seta flag
        if (!digitalRead(btn_set_pin) && menu_flag && inSubMenu) //Botão Up solto e flag setada?
        {   //Sim...
            menu_flag = 0x00; //Limpa flag
            indiceSubMenu++; //Incrementa menu
            if (indiceSubMenu > 0x07) indiceSubMenu = 0x01; //Se menu maior que 7, volta a ser 1

        } //end menu

        switch (indiceSubMenu) {
        case 1: //Hora
            /*
            t0 t4
            t1 t5
            t2 t6
            t3 t7
            */
            Serial.print("t0.bco=");
            Serial.print(64520);
            sendEsc();
            Serial.print("t4.bco=");
            Serial.print(64520);
            sendEsc();
            //
            Serial.print("t8.bco=");
            Serial.print(0);
            sendEsc();
            Serial.print("t8.txt=");
            Serial.write(0x22);
            Serial.print("Aj. Dt/hs");
            Serial.write(0x22);
            sendEsc();
            Serial.print("t0.txt=");
            Serial.write(0x22);
            Serial.print("1 - Hora");
            Serial.write(0x22);
            sendEsc();
            Serial.print("t4.txt=");
            Serial.write(0x22);
            Serial.print(hora);
            Serial.write(0x22);
            sendEsc();

            if (!digitalRead(btn_reset_pin) && enter_flag && inSubMenu) //Botão Down solto e flag setada?
            {   //Sim...
                enter_flag = 0x00; //Limpa flag
                indiceSubMenu = 1;
                //Serial.println("Zerou!!!!!!!!!!!!");
                if (hora < 23){
					hora++;
				}	else {
					hora = 0;
				}
            } //end butreset
            break;

        case 2: //Minuto
            Serial.print("t0.bco=");
            Serial.print(50712);
            sendEsc();
            Serial.print("t4.bco=");
            Serial.print(50712);
            sendEsc();
            //
            Serial.print("t1.bco=");
            Serial.print(64520);
            sendEsc();
            Serial.print("t5.bco=");
            Serial.print(64520);
            sendEsc();
            //
            Serial.print("t8.bco=");
            Serial.print(0);
            sendEsc();
            Serial.print("t8.txt=");
            Serial.write(0x22);
            Serial.print("Aj. Dt/hs");
            Serial.write(0x22);
            sendEsc();
            Serial.print("t1.txt=");
            Serial.write(0x22);
            Serial.print("2 - Min");
            Serial.write(0x22);
            sendEsc();
            Serial.print("t5.txt=");
            Serial.write(0x22);
            Serial.print(minuto);
            Serial.write(0x22);
            sendEsc();

            if (!digitalRead(btn_reset_pin) && enter_flag && inSubMenu) //Botão Down solto e flag setada?
            {   //Sim...
                enter_flag = 0x00; //Limpa flag
                indiceSubMenu = 2;
                //Serial.println("incrementa minuto");
                if (minuto < 59){
					minuto++;
				} else {
					minuto = 0;
				}	
            } //end butreset
            break;



        case 3: //Dia
            Serial.print("t1.bco=");
            Serial.print(50712);
            sendEsc();
            Serial.print("t5.bco=");
            Serial.print(50712);
            sendEsc();
            //
            Serial.print("t2.bco=");
            Serial.print(64520);
            sendEsc();
            Serial.print("t6.bco=");
            Serial.print(64520);
            sendEsc();
			//
            Serial.print("t8.bco=");
            Serial.print(0);
            sendEsc();
            Serial.print("t8.txt=");
            Serial.write(0x22);
            Serial.print("Aj. Dt/hs");
            Serial.write(0x22);
            sendEsc();
            Serial.print("t2.txt=");
            Serial.write(0x22);
            Serial.print("3 - Dia");
            Serial.write(0x22);
            sendEsc();
            Serial.print("t6.txt=");
            Serial.write(0x22);
            Serial.print(dia);
            Serial.write(0x22);
            sendEsc();

            if (!digitalRead(btn_reset_pin) && enter_flag && inSubMenu) //Botão Down solto e flag setada?
            {   //Sim...
                enter_flag = 0x00; //Limpa flag
                indiceSubMenu = 3;
                //Serial.println("Incrementa dia");
				if (dia < 31){
					dia++;
				} else {
					dia = 1;
				}	
            } //end butreset
            break;

       case 4: //Mes
            Serial.print("t2.bco=");
            Serial.print(50712);
            sendEsc();
            Serial.print("t6.bco=");
            Serial.print(50712);
            sendEsc();
            //
            Serial.print("t3.bco=");
            Serial.print(64520);
            sendEsc();
            Serial.print("t7.bco=");
            Serial.print(64520);
            sendEsc();
            Serial.print("t8.bco=");
            Serial.print(0);
            sendEsc();
            Serial.print("t8.txt=");
            Serial.write(0x22);
            Serial.print("Aj. Dt/hs");
            Serial.write(0x22);
            sendEsc();
            Serial.print("t3.txt=");
            Serial.write(0x22);
            Serial.print("4 - Mes");
            Serial.write(0x22);
            sendEsc();
            Serial.print("t7.txt=");
            Serial.write(0x22);
            Serial.print(mes);
            Serial.write(0x22);
            sendEsc();

            if (!digitalRead(btn_reset_pin) && enter_flag && inSubMenu) //Botão Down solto e flag setada?
            {   //Sim...
                enter_flag = 0x00; //Limpa flag
                indiceSubMenu = 4;
                //Serial.println("Incrementa mes");
				if (mes < 12){
					mes++;
				} else {
					mes = 1;
				}	
            } //end butreset
            break;

		case 5: //Ano +
            Serial.print("t3.bco=");
            Serial.print(50712);
            sendEsc();
            Serial.print("t7.bco=");
            Serial.print(50712);
            sendEsc();
            //
            Serial.print("t0.bco=");
            Serial.print(64520);
            sendEsc();
            Serial.print("t4.bco=");
            Serial.print(64520);
            sendEsc();
			//
			Serial.print("t0.txt=");
			Serial.write(0x22);
			Serial.print("5 - Ano +");
			Serial.write(0x22);
			sendEsc();
			Serial.print("t4.txt=");
			Serial.write(0x22);
            Serial.print(ano);
			Serial.write(0x22);
			sendEsc();

            if (!digitalRead(btn_reset_pin) && enter_flag && inSubMenu) //Botão Down solto e flag setada?
            {   //Sim...
                enter_flag = 0x00; //Limpa flag
                indiceSubMenu = 5;
                //Serial.println("Incrementa ano");
                ano++;
            } //end butreset
            break;

		case 6: //Ano -
            Serial.print("t0.bco=");
            Serial.print(50712);
            sendEsc();
            Serial.print("t4.bco=");
            Serial.print(50712);
            sendEsc();
            //
            Serial.print("t1.bco=");
            Serial.print(64520);
            sendEsc();
            Serial.print("t5.bco=");
            Serial.print(64520);
            sendEsc();		
			//
			Serial.print("t1.txt=");
			Serial.write(0x22);
			Serial.print("6 - Ano -");
			Serial.write(0x22);
			sendEsc();
			Serial.print("t5.txt=");
			Serial.write(0x22);
            Serial.print(ano);
			Serial.write(0x22);
			sendEsc();

            if (!digitalRead(btn_reset_pin) && enter_flag && inSubMenu) //Botão Down solto e flag setada?
            {   //Sim...
                enter_flag = 0x00; //Limpa flag
                indiceSubMenu = 6;
                //Serial.println("Incrementa ano");
                ano--;
            } //end butreset
            break;			
			
        case 7: /* saida */
//
            Serial.print("t1.bco=");
            Serial.print(50712);
            sendEsc();
            Serial.print("t5.bco=");
            Serial.print(50712);
            sendEsc();
//
            Serial.print("t8.txt=");
            Serial.write(0x22);
            Serial.print("Sair?");
            Serial.write(0x22);
            sendEsc();
            Serial.print("t8.bco=");
            Serial.print(63488);
            sendEsc();
            if (!digitalRead(btn_reset_pin) && enter_flag && inSubMenu) //Botão Down solto e flag setada?
            {   //Sim...
                enter_flag = 0x00; //Limpa flag
                //Ajustando data e hora.
				rtc.adjust(DateTime(ano, mes, dia, hora, minuto, 0));
                indiceSubMenu = 1; // Retorna Posicao do Submenu pra 1
                inSubMenu = 1; //Sai do submenu
                //retorna menu 9
                indiceMenu = 11;
                retorno_menu_1 = 0; //Reaparece page 1
            } //end butreset
            break;
        } //fim switch (indiceSubMenu)
    } // fim else !inSubMenu
}
//----------- Fim Menu ajDataHora ----------- //
