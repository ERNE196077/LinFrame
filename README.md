# LinFrame
Proyecto 1 : ITESO : Ing Sistemas Embebidos : Segundo Semestre : Proyecto LIN

Team:
Jorge Erick 
Juan Carlos
Ernesto Ramos

LIN project tested using the following parameters:

uint8_t sdutestvar[8] = {0x55,0x00,0x55,0x05};

LinPduType_t LinPduTest = { 0x0F,
               LIN_CLASSIC_CS,           
               LIN_MASTER_RESPONSE,
               4,            
               &sdutestvar};


![alt tag](https://github.com/ERNE196077/LinFrame/blob/master/SDS00003.BMP)
