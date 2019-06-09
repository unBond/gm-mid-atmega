# gm-mid-atmega
Alternative to gm/chevrolet/ multi display information - MID based on atmega 1284p / arduino editor/compiler o using standalone pcb
Compatible cars:

Gm astra with oem mid (brazil)
elite
SS
CD
GSI

Gm Vectra flex with oem mid (brazil)
GTX
Elegance
Elite 2.0/2.4

Another cars, maybe need some adaptations, or new calcs on fuel consumption factor/adjust vss

Video Demo:
[![Watch the video](http://i3.ytimg.com/vi/AK_T6fzSQlk/hqdefault.jpg)](https://youtu.be/AK_T6fzSQlk)


## pcb / boom / lista de peças
https://easyeda.com/unBond/mid-atmega-1284p-3
![gm-mid-atmega](https://raw.githubusercontent.com/unBond/gm-mid-atmega/master/pcb_beta.jpg)
Quase todos componentes você vai encontrar no esquemático no easy eda, fora isso você vai precisar 
de um

1 - ATMEGA 1284p
2 - Stepdown 12 pra 5v, pois um 7805 não suporta alimentar o LCD. Vou deixar um exemplo
![gm-mid-atmega](https://raw.githubusercontent.com/unBond/gm-mid-atmega/master/dd4012-65-40v-p-5v-step-down-dcdc-substituto-7805-D_NQ_NP_904249-MLB27779427662_072018-F.jpg)
3 - Um gravador USBTiny USBtinyISP AVR ISP Programador Bootloader Para Arduino ou usar um arduino UNO mesmo pra gravar a firmware

## Principais créditos:

* Autor: grzeeezzz
Baseado no video tutorial : https://www.youtube.com/watch?v=9uLg3KEshto
Leitura complementar: http://gammon.com.au/interrupts

* Esqueleto/lógica dos menus: 
Autor: Eng. Wagner Rambo  
Lógica inspirada no video tutorial: Curso de Arduino e AVR 134: https://www.youtube.com/watch?v=tRoznRBYQHA

* Funções de economia de energia/SleepMode
Autor: Make cource
Inspirado no video tutorial https://www.youtube.com/watch?v=HiAbxSO_9nU e óbvio na documentação oficial do microcontrolador

* Funções de data/Hora e biblioteca com mod de cálculo de data futura 
Autor: vários users conforme o link:
https://github.com/adafruit/RTClib/pull/68

* Funções de temperatura/leitura do tanque
Autor:Angel Zaprianov 
https://github.com/fire1/ArduinoMid/

* Algumas partes são de outros autores e inúmeras pesquisas que fiz, que acabei esquecendo de anotar para dar os devidos créditos.
O restante é de minha autoria

Não me responsabilizo por qualquer dano ou prejuízo que este venha causar.
Use por sua conta e risco. Sempre consulte um engenheiro/projetista profissional.

## Main credits:

* Author: grzeeezzz
Based on the video tutorial: https://www.youtube.com/watch?v=9uLg3KEshto
Further reading: http://gammon.com.au/interrupts

* Skeleton / Logic of the menus:
Author: Eng. Wagner Rambo
Logic inspired video tutorial: Arduino Course and AVR 134: https://www.youtube.com/watch?v=tRoznRBYQHA

* Power Saving Functions / SleepMode
Author: Make cource
Inspired by the video tutorial https://www.youtube.com/watch?v=HiAbxSO_9nU and obvious in the official microcontroller documentation

* Date / Time and Library Functions with Future Date Calculation Mod
Author: multiple users according to the link:
https://github.com/adafruit/RTClib/pull/68

* Temperature reading / tank reading
Author: Angel Zaprianov
https://github.com/fire1/ArduinoMid/

Some parts are of other authors and countless researches that I did, that I forgot to write down to give the due credits.
The rest is my own

I will not be liable for any damage or injury that this may cause.
Use at your own risk. Always consult a professional engineer / designer.


