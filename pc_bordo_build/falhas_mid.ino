void falhas_display() {

    /*
    *
    * Aqui no caso a ordem dos if vai implicar diretamente na prioridade dos alertas, então vamos pros mais críticos primeiro.
    *
    * Sensores/Avisos personalizaveis (podem ser desabilitados em runtime):
    * lvl_oil  - ok
    * brake_pad - ok
    * lvl_coolnt - ok
    * trans_status - ok
    * lvl_wash - ok
    * lant_p1 - ok
    * freio_p1 - ok
    * oil_insp - ok
    *
    * Sensores permanentes
    * brake_fuse == true
    */
    if (retorno_menu_2 == 0) {
        // Funcao de entrada menu de falhas...
        Serial.print("page 1");
        sendEsc();
        retorno_menu_2 = 1;
    }
//backlight
    backlightMenuF();
//1 - Nivel liquido arrefecimento, sem agua o motor vai pro saco
    if (lvl_coolnt == true && contadorFalhas < 5) {
        writeMenuF("4", "Nivel Liquido ", "Arrefecimen. Verificar");
        beepMid();
		    one_beep = false; // aviso constante
        delay(3000);
    }

//2 - Nivel do olveo, sem oleo o motor vai pro saco - Geralmente motores 16v
    if (lvl_oil == true && contadorFalhas < 10) {
        writeMenuF("5", "Nivel Oleo Motor", "Verificar!");
        beepMid();
        delay(3000);
    }

//3 - Transmissao, falha, carros automaticos, nao faco a menor ideia de como funciona
    if (trans_status == true && contadorFalhas < 15) {
        writeMenuF("20", "Transmissao", "Verificar!");
        beepMid();
        delay(3000);
    }

//4 - Fusivel luz de freio, nao precisa explicar...
    if (brake_fuse == true && contadorFalhas < 20) {
        writeMenuF("6", "Fusi. Luz Freio", "Verificar!");
        beepMid();
        delay(3000);

    }

    //5 - Pastilhas de freio, desgaste elevado...
    if (brake_pad == true && contadorFalhas < 25) {
        writeMenuF("8", "Pastilhas de Freio", "Verificar!");
        beepMid();
        delay(3000);
        //afim de que exiba somente apos 15 minutos.
        brake_pad = false;
    }

    //6 - nivel do lavador vidro...
    if (lvl_wash == true && contadorFalhas < 30) {
        writeMenuF("7", "Nivel Agua Lavador", "Verificar!");
        beepMid();
        delay(3000);
        //afim de que exiba somente apos 15 minutos.
        lvl_wash = false;
    }

    //7 - inspecao oleo
    if (oil_insp == true && contadorFalhas < 35) {
		writeMenuF("23", "Troca do Oleo ", String(oil_run));
        beepMid();
        delay(3000);
        //afim de que exiba somente apos 15 minutos.
        oil_insp = false;
    }

    //8 - Farol/Lanterna Freio
    // Astra manual 2008 = "Medios. Lant. Tras."
    // Vectra manual 2008 = "Luz baixa. Lant. Tras."
    if (lant_p1 == true && contadorFalhas < 40) {
        writeMenuF("21", "Luz baixa. Lant. Tras.", "Verificar");
        beepMid();
        delay(3000);
        //afim de que exiba somente apos 15 minutos.
        //lant_p1 = false;
    }

    //9 - Farol/Lanterna Freio
    if (freio_p1 == true && contadorFalhas < 45) {
        writeMenuF("22", "Luz de Freio", "Verificar");
        beepMid();
        delay(3000);
        //afim de que exiba somente apos acionamento do gatilho.
        freio_p1 = false;
        
    }
    //10 - Temperatura excessiva
    if (temp_eng_high == true && contadorFalhas < 50) {
        writeMenuF("12", "Temp. alta", String(CUR_ENT));
        beepMid();
        delay(3000);
		one_beep = false; // aviso constante
        //afim de que exiba somente apos 15 minutos.
    }
	
}


