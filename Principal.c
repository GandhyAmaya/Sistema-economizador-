/* 
 * File:   Principal.c
 * Author: Grupo5
 *
 * Created on 29 de mayo de 2016, 10:44 PM
 */

#include <Principal.h>
#include <rtcc.c>
#include <24C321.c>
#include "HDM64GS12.c"
#include "graphics.c"
#include <finalizado.h>
#include <Interfaz.h>
#include <Portada.h>
#include <DataAsist.h>
#include <saving.h>
#include <Tipo_control.h>
#define trig      PIN_A6   
#define echo    PIN_A7  

#use rs232 (baud=9600, xmit=pin_C6, rcv=pin_C7) 

//============================================================================================
//=============================== DECLARACION DE VARIABLES =======================================
int c,t0overflow,t1overflow,dayset,monthset,yrset,hrset,minset,hr,min,sec,oldsec,dia,mes,dow,yr,s;
unsigned int1 am_pm,lavamanos,tanquelleno,colorp,exitflag;
long int i=0,i2=0,pulsos_ducha,pulsos_lavamanos,tiempodist;
long int duty_cycle,Percent,nuevoc,L_wasted,nuevo,l_ahorro,litrosd_int,litroslav_int,eread,actualbyte,oldactualbyte;
float pre_duty_cycle,litros_ducha,litros_lavamanos,litrosd_float,litroslav_float,nivel,oldnivel,t0count,oldt0count,t1count,oldt1count;
float totalwater,oldtotalwater,sum_lav,sum_ducha;
//============PARA CONSULTA DE DATOS=========================
float L_ducha,L_lav,Sum,buffer,t0count_continuous,t1count_continuous;
int c_dia,c_mes,c_yr,c_hr,c_min;
signed long int page;
//==================================================================================
//=============================== MENSAJES EN PANTALLA================================

char time[];
char Pgeneral[20];
char Pgeneral2[20];
char Pgeneral3[20];
char Pgeneral4[20];
char Pgeneral5[20];


#reserve 0x080:0xAFF 
  
char MENSAJE1[]="SISTEMA ECONOMIZADOR";
char SETTIME[]="Configurar ?";
char TIMENOW[]="Hora/Fecha Actual:";
char DOMINGO[]=   "Domingo";
char LUNES[]=       "Lunes";
char MARTES[]=     "Martes";
char MIERCOLES[]= "Miercoles";
char JUEVES[]=      "Jueves";
char VIERNES[]=     "Viernes";
char SABADO[]=     "Sabado";
char SI[]="Si";
char NOO[]="No";
char SET_TIME[]="Hora";
char SETDATE[]="Fecha";
char AJUSTEHORA[]="AJUSTE DE HORA";
char AJUSTEFECHA[]="AJUSTE DE FECHA";
char EEPROM[]="AJUSTE EEPROM";
char CONSUMO[]="CONSULTA DE CONSUMOS";
char CAPACITY[]="Capacidad:";
char UNREAD[]="N/A  / 4096 B";
char LEER[]="Leer";
char BORRAR[]="Borrar";
char SALIR[]="Salir";
char LOADING[]="...";
char AM[]="AM";
char PM[]="PM";
char ESTABLECER[]="Establecer";
char DATOS[]="Datos";
char DATOS2[]="Guardados!";
char READY[]="Sistema listo!";
char COMENZARS[]="Iniciar Ahora";
char CONSULTARC[]="Desea consultar los";
char CONSULTARC2[]="consumos a la fecha?";
char VOLVERR[]="Volver";
char SELECCIONAR[]="Seleccionar";
char T_CONTROL[]="TIPO DE CONTROL:";
char H_CONTROL[]="Habilitar control?";
char LIMITE[]="Consumo Maximo(L):";
char LIMITE_ALC[]="LIMITE ALCANZADO!";

#locate MENSAJE1                   = 0X081
#locate SETTIME                     = 0X110
#locate TIMENOW                    = 0X135
#locate DOMINGO                     = 0X150
#locate LUNES                         = 0X170
#locate MARTES                       = 0X190
#locate MIERCOLES                   = 0X1B0
#locate JUEVES                        = 0X1D0
#locate VIERNES                       = 0X1F0
#locate SABADO                       = 0X210
#locate SI                            = 0X230
#locate NOO                           = 0X250
#locate SET_TIME                      = 0X270
#locate SETDATE                       = 0X290
#locate AJUSTEHORA                    = 0X2B5
#locate AJUSTEFECHA                   = 0X2E0
#locate EEPROM                        = 0X305
#locate CAPACIDAD                     = 0X325
#locate UNREAD                        = 0X345
#locate AM                            = 0X360
#locate PM                            = 0X375
#locate ESTABLECER                    =0X385
#locate DATOS                         =0X405
#locate DATOS2                        =0X420
#locate READY                         =0X440
#locate COMENZARS                     =0x465
#locate CONSUMO                       =0X480
#locate CONSULTARC                    =0x4A0
#locate CONSULTARC2                   =0x4C0
#locate VOLVERR                       =0x4E0
#locate SELECCIONAR                   =0X505
#locate T_CONTROL                     =0x520
#locate H_CONTROL                     =0x540
#locate LIMITE                        =0x565
#locate LIMITE_ALC                    =0x585
#locate Pgeneral                      =0x5A0
#locate LEER                          =0x5C0
#locate BORRAR                        =0x5D0
#locate SALIR                         =0x5E0
#locate LOADING                       =0x5F0

#locate Pgeneral2                     =0x600
#locate Pgeneral3                     =0x640
#locate Pgeneral4                     =0x680
#locate Pgeneral5                     =0x6B0

        

//==================================================================================
//========================INTERRUPCION LAVAMANOS======================================
#INT_EXT 
void  EXT_isr(void)  
{
    lavamanos = 1;
}
//==================================================================================
//========================INTERRUPCION INODORO LLENO===================================
#INT_EXT1
void  EXT1_isr(void) 
{
    tanquelleno = 1;
}
//==================================================================================
//==========================INTERRUPCION ADICIONAL=====================================
//#INT_EXT2
//void  EXT2_isr(void)
//{
//
//}
//==========================================================================================
//==========================================================================================
#INT_TIMER0 
void  TIMER0_isr(void)  
{
t0overflow=t0overflow + 1;
}
//==========================================================================================
//==========================================================================================
#INT_TIMER1
void  TIMER1_isr(void)  
{
t1overflow=t1overflow + 1;
}
//==================================================================================
//========================FUNCI?N PARA LEER RTC========================================
void leerRTC(){
    ds1307_get_time(hr,am_pm,min,sec);
    ds1307_get_date(dia,mes,yr,dow);
    
    if(am_pm==1)
    time='P';
    else
    time='A';
    delay_us(100);
}
//==================================================================================
//========================FUNCI?N PARA LEER NIVEL========================================
void leernivel(){
    output_high(trig);                       
    delay_us(20);                            
    output_low(trig);
    
    while(input(echo)==0)                      
    {}
    set_timer3(0);                          
    while(input(echo)==1)                      
    {}
    
    tiempodist=get_timer1();     
    nivel=tiempodist*0.017;

}
//==================================================================================
//==================================================================================

void arrow(){
   glcd_pixel(110,48,0); 
   glcd_pixel(109,47,0); 
   glcd_pixel(109,49,0); 
   glcd_pixel(108,46,0); 
   glcd_pixel(108,50,0); 
   glcd_pixel(107,45,0);
   glcd_pixel(107,51,0);
   glcd_pixel(106,48,0); 
   glcd_pixel(105,47,0); 
   glcd_pixel(105,49,0); 
   glcd_pixel(104,46,0); 
   glcd_pixel(104,50,0); 
   glcd_pixel(103,45,0);
   glcd_pixel(103,51,0); 
   glcd_pixel(102,48,0); 
   glcd_pixel(101,47,0); 
   glcd_pixel(101,49,0); 
   glcd_pixel(100,46,0); 
   glcd_pixel(100,50,0); 
   glcd_pixel(99,45,0);
   glcd_pixel(99,51,0);
}
//==================================================================================
//==================================================================================

void arrow2(){
   glcd_pixel(110,53,colorp); 
   glcd_pixel(109,52,colorp); 
   glcd_pixel(109,54,colorp); 
   glcd_pixel(108,51,colorp); 
   glcd_pixel(108,55,colorp); 
   glcd_pixel(107,50,0);
   glcd_pixel(107,56,0);
   glcd_pixel(106,53,0); 
   glcd_pixel(105,52,0); 
   glcd_pixel(105,54,0); 
   glcd_pixel(104,51,0); 
   glcd_pixel(104,55,0); 
   glcd_pixel(103,50,0);
   glcd_pixel(103,56,0); 
   glcd_pixel(102,53,0); 
   glcd_pixel(101,52,0); 
   glcd_pixel(101,54,0); 
   glcd_pixel(100,51,0); 
   glcd_pixel(100,55,0); 
   glcd_pixel(99,50,0);
   glcd_pixel(99,56,0);
   
   glcd_pixel(15,53,0); 
   glcd_pixel(16,52,0); 
   glcd_pixel(16,54,0);
   glcd_pixel(17,51,0); 
   glcd_pixel(17,55,0);
   glcd_pixel(18,50,0); 
   glcd_pixel(18,56,0);
   glcd_pixel(19,53,0); 
   glcd_pixel(20,52,0); 
   glcd_pixel(20,54,0);
   glcd_pixel(21,51,0); 
   glcd_pixel(21,55,0);
   glcd_pixel(22,50,0); 
   glcd_pixel(22,56,0);
   glcd_pixel(23,53,0); 
   glcd_pixel(24,52,0); 
   glcd_pixel(24,54,0);
   glcd_pixel(25,51,0); 
   glcd_pixel(25,55,0);
   glcd_pixel(26,50,0); 
   glcd_pixel(26,56,0);
 
}
//==================================================================================
//==================================================================================
void consulta_datos(){

glcd_fillScreen(0);
glcd_assist();
glcd_text57(5,4,CONSUMO, 1, 1);

glcd_rect(0,44,41,63,YES,ON);
glcd_rect(43,44,83,63,YES,ON);
glcd_rect(85,44,127,63,YES,ON);
glcd_text57(49,50,SALIR, 1, 0);
arrow2();
page = 0;

goto PAGE1;

READ_PAGE:

if(page > 0)
arrow2();

s = read_adc();
delay_ms(50);
while(s > 240){
    s = read_adc();
    delay_ms(50);//********************
}

 if(s > 195 && s < 225) {//enter
     exitflag=1;
     return;
}

if(s > 115 && s < 135){ //Devolver
    
     if (page < 1){
         page = 0;
         goto PAGE1;
     }
     if(page == 10)
         page = page - 10;
     else
         page = page -9;
}

if(s > 165 &&  s < 185){ //Avanzar
    if (page > 9)
        page = page + 9;
    else
        page = page + 10;
//  if (page > 31)
//      page = 0; 
}

if (page == 0) { //PRIMERA PAGINA
    PAGE1:
    glcd_rect(0,44,41,63,YES,ON);
//==================================================================================
    buffer = read_ext_eeprom(2);
    L_ducha = buffer;
    buffer = read_ext_eeprom(3); 
    L_ducha = L_ducha+(buffer/100);
 //==================================================================================   
    buffer = read_ext_eeprom(4);
    L_lav = buffer;
    buffer = read_ext_eeprom(5); 
    L_lav = L_lav+(buffer/100);  
//==================================================================================
    c_hr = read_ext_eeprom(6);
    c_min = read_ext_eeprom(7);
    c_dia = read_ext_eeprom(8);
    c_mes = read_ext_eeprom(9);
    c_yr = read_ext_eeprom(10);
//================================================================================== 
    Sum = L_ducha + L_lav;
    glcd_rect(76,32,122,39,YES,OFF);
    glcd_rect(19,18,57,26,YES,OFF);
    glcd_rect(19,32,57,39,YES,OFF);
    glcd_rect(76,18,122,25,YES,OFF);
}
else{
//==================================================================================
    buffer = read_ext_eeprom(page +1);
    L_ducha = buffer;
    buffer = read_ext_eeprom(page + 2); 
    L_ducha = L_ducha+(buffer/100);
 //================================================================================== 
    buffer = read_ext_eeprom(page + 3);
    L_lav = buffer;
    buffer = read_ext_eeprom(page + 4); 
    L_lav = L_lav+(buffer/100);  
//==================================================================================
    c_hr = read_ext_eeprom(page + 5);
    c_min = read_ext_eeprom(page + 6);
    c_dia = read_ext_eeprom(page + 7);
    c_mes = read_ext_eeprom(page + 8);
    c_yr = read_ext_eeprom(page + 9);
//================================================================================== 
    Sum = L_ducha + L_lav;
    glcd_rect(76,32,122,39,YES,OFF);
    glcd_rect(19,18,57,26,YES,OFF);
    glcd_rect(19,32,57,39,YES,OFF);
    glcd_rect(76,18,122,25,YES,OFF);

}

sprintf(Pgeneral,"%02d/%02d/%02d",c_dia,c_mes,c_yr);
glcd_text57(76,32,Pgeneral, 1, 1);

sprintf(Pgeneral2,"%.2fL",L_ducha);
glcd_text57(19,18,Pgeneral2, 1, 1);

sprintf(Pgeneral3,"%.2fL",L_lav);
glcd_text57(19,32,Pgeneral3, 1, 1);

sprintf(Pgeneral4,"%.2fL",Sum);
glcd_text57(76,18,Pgeneral4, 1, 1);

  
goto READ_PAGE;    
}

//==================================================================================
//========================FUNCI?N PARA GUARDAR EN EEPROM===============================
void storeDATA(){
//======================CALCULO PARA LA DUCHA========================    
//    disable_interrupts(GLOBAL);
//    pulsos_ducha= t0overflow*65535 + get_timer0();
//    litros_ducha= ((0.0657*t0overflow*t0overflow-4.17*t0overflow+78)/ 60.0);
//    set_timer0(0);//====================>>>>>>>>>>>
//    enable_interrupts(GLOBAL);
//==================================================================
//======================CALCULO PARA LAVAMANOS======================== 
//    disable_interrupts(GLOBAL);
//    pulsos_lavamanos= t1overflow*65535 + get_timer1();
//    litros_lavamanos= ((0.0657*t1overflow*t1overflow-4.17*t1overflow+78)/ 60.0);
//    set_timer1(0);//====================>>>>>>>>>>>
//    enable_interrupts(GLOBAL);
//===================================================================
//    litrosd_int = litros_ducha;
//    litrosd_float = (litros_ducha - litrosd_int)*100; // DOS DIGITOS DE RESOLUCI?N
//    
//    litroslav_int = litros_lavamanos;
//    litroslav_float = (litros_lavamanos - litroslav_int)*100;   // DOS DIGITOS DE RESOLUCI?N
    
//=======================================================================  
    litrosd_int = sum_ducha;
    litrosd_float = (sum_ducha - litrosd_int)*100; // DOS DIGITOS DE RESOLUCI?N
    
    litroslav_int = sum_lav;
    litroslav_float = (sum_lav - litroslav_int)*100;   // DOS DIGITOS DE RESOLUCI?N
//========================================================================
//========================================================================
//Escritura de bloques de 10 casillas en la EEPROM====================================
//|    0        |    1          |      2       |     3         | 4               |  5                   |  6   |  7    |  8  |   9   | 10   |
//------------------------------------------------------             
//|Reservada| Reservada |  LTD_INT  | LTD Float | LT_Lav Int  | LT_LAV_Float    |  HR | MIN | DIA | MES | A?O |
//| para i     |  para i      |               |               |                 |                        |  HR | MIN | DIA | MES | A?O | 
//=========================================================================   
glcd_fillScreen(0);
glcd_saving();
glcd_rect(5,50,122,58,NO,ON);

//======================
//litrosd_int=2;
//litrosd_float=3;
//litroslav_int=2;
//litroslav_float=3;
//hr= 12;
//min= 30;
//dia=8;
//mes=12;
//yr=16;
//======================

//loop:
glcd_rect(6,51,121,57,YES,OFF);
delay_ms(200);
i = read_ext_eeprom(0);    //  =======> INDICE
i2 = read_ext_eeprom(1);  //  =======> DESPLAZAMIENTO 8 BITS 0xFF;

delay_ms(1);
if(i==0)
    goto skip;
i++;//=========> !!!!!!!!!!!!!!!!!!!!!!!
skip:
write_ext_eeprom(2+i+(i2*255),litrosd_int);
glcd_rect(5,50,18,58,YES,ON);
delay_ms(50);
i++;
write_ext_eeprom(2+i+(i2*255),litrosd_float);
glcd_rect(18,50,31,58,YES,ON);
delay_ms(50);
i++;
write_ext_eeprom(2+i+(i2*255),litroslav_int);
glcd_rect(31,50,44,58,YES,ON);
delay_ms(50);
i++;
write_ext_eeprom(2+i+(i2*255),litroslav_float);
glcd_rect(44,50,57,58,YES,ON);
delay_ms(50);
i++;
write_ext_eeprom(2+i+(i2*255),hr);
glcd_rect(57,50,70,58,YES,ON);
delay_ms(50);
i++;
write_ext_eeprom(2+i+(i2*255),min);
glcd_rect(70,50,83,58,YES,ON);
delay_ms(50);
i++;
write_ext_eeprom(2+i+(i2*255),dia);
glcd_rect(70,50,83,58,YES,ON);
delay_ms(50);
i++;
write_ext_eeprom(2+i+(i2*255),mes);
glcd_rect(83,50,96,58,YES,ON);
delay_ms(50);
i++;
write_ext_eeprom(2+i+(i2*255),yr);
glcd_rect(96,50,122,58,YES,ON);
delay_ms(50);
//====================!!!!!!!!!!!!!!!
if (i >= 255){
    i2++;
    i = i - 255;
}
//============================
//if (i == 255){
//    i = 0;
//    i2++;
//}

delay_ms(100);
write_ext_eeprom(0,i);
write_ext_eeprom(1,i2);
litros_lavamanos = 0;
litros_ducha = 0;
set_timer0(0);
set_timer1(0);
//goto loop;
}
//==================================================================================
//========================FUNCI?N DEL CONTROLADOR DEL SISTEMA===========================
void controlador(){
//===================CONTROL LINEAL=============  
if(c == 0){

   Percent = ((L_wasted-nuevo)*100)/l_ahorro; //===PORCENTAJE DE AGUA GASTADA
   //======C?lculo en microsegundos=================
   //====LIMITES DEL SERVOMOTOR 0.8ms A 2,3ms=======
//   pre_duty_cycle = (percent*0.0023)/100.0;
//   duty_cycle = pre_duty_cycle * 745000;
   pre_duty_cycle = (Percent*0.015/100.0)+0.008;
   duty_cycle = (pre_duty_cycle * 1000000);
   for(i=0;i<20;i++){
       output_low(PIN_B3);
       delay_us(1);
       output_high(PIN_B3);
       delay_us(duty_cycle);
       output_low(PIN_B3);
       delay_us(20000 - duty_cycle);
    }
  
   return;
}
//===============CONTROL LINEAL RETARDADO========
if(c ==1){
  Percent = (L_wasted*100)/l_ahorro;
  if(Percent >= 50){
//   pre_duty_cycle = (percent*0.0023)/100.0;
//   duty_cycle = pre_duty_cycle * 1000000;
   pre_duty_cycle = (Percent*0.015/100.0)+0.008;
   duty_cycle = (pre_duty_cycle * 1000000);
   for(i=0;i<20;i++){
       output_low(PIN_B3);
       delay_us(1);
       output_high(PIN_B3);
       delay_us(duty_cycle);
       output_low(PIN_B3);
       delay_us(20000 - duty_cycle);
    } 
  }  
  return;
}
}
//=================================================================================
//==========================PROGRAMA PRINCIPAL DEL SISTEMA============================

void main(){
    
enable_interrupts(GLOBAL);
enable_interrupts(INT_EXT);
enable_interrupts(INT_EXT1);
//enable_interrupts(INT_EXT2);
enable_interrupts(INT_TIMER0);
enable_interrupts(INT_TIMER1);

setup_timer_0(RTCC_EXT_L_TO_H|RTCC_DIV_1| 0x00);  //TIMER0 EN 16 BITS //CAUDAL 1
setup_timer_1(T1_EXTERNAL_SYNC|T1_DIV_BY_1);          //CAUDAL 2
setup_timer_3(T1_INTERNAL|T1_DIV_BY_2);     //SENSOR ULTRAS?NICO
setup_adc_ports(AN0,VSS_VDD); 
setup_adc(ADC_CLOCK_DIV_32);

REBOOT:

t0count = 0;
t1count = 0;
totalwater = 0;
L_wasted = 0;
Percent = Percent*0;

set_timer0(0);
set_timer1(0);
set_timer3(0);

output_low(PIN_C1); //VALVULA LAVAMANOS
output_low(PIN_C2); //MOTOBOMBA
//==============================
//C = 0;
//goto EMPEZAR;
//==============================
   glcd_init(ON);       // Inicializaci?n de GLCD
   glcd_fillScreen(0); //LIMPIAMOS LA PANTALLA 
   glcd_portada();
   delay_ms(2000);
   
   glcd_fillScreen(0); //==============
//   storeDATA();
//   goto SIGUIENTE2;
//   goto BORRAR_EEPROM;
//   goto EMPEZAR;
   
   ds1307_init();
   glcd_rect(0,0,127,63,NO,ON);
   glcd_rect(1,1,126,62,NO,ON);
   glcd_text57(5,5,MENSAJE1, 1, 1);
   glcd_text57(8,20,SETTIME, 1, 1);
   glcd_rect(5,37,61,58,YES,ON);
   glcd_rect(65,37,120,58,YES,ON);
   glcd_text57(28,44,SI, 1, 0);
   glcd_text57(86,44,NOO, 1, 0);
   
   glcd_rect(6,38,60,57,NO,OFF);
   //glcd_rect(65,37,120,58,YES,ON);
   
   set_adc_channel(0);
   delay_us(20);
   
   START1:
   
   s = read_adc();
   delay_ms(100); //***************************
   
   while(s > 240){
    s = read_adc();
    delay_ms(50);
   }
   
   if(s > 195 && s < 225) //enter
       goto CONF_RTC;
   else if (s > 165 &&  s < 185){
       glcd_rect(6,38,60,57,NO,ON);
       glcd_rect(66,38,119,57,NO,OFF);
       delay_ms(100);
       s = read_adc();
       delay_us(40);
       while(s > 240){
            s = read_adc();
            delay_ms(50);//*************************
            }
       if(s > 195 && s < 225)
           goto SISTEMA;
       if(s > 115 && s < 135){
           glcd_rect(66,38,119,57,NO,ON);
           glcd_rect(6,38,60,57,NO,OFF);
           delay_ms(50);
           goto START1;
       }   
       else 
           goto START1;
   }
    goto START1;
    
   
  CONF_RTC: 
  glcd_fillScreen(0);
  glcd_rect(0,0,127,63,NO,ON);
  glcd_rect(1,1,126,62,NO,ON);
  glcd_text57(5,5,TIMENOW, 1, 1);
  leerRTC();
  sprintf(Pgeneral,"\%02d:\%02d:\%02d \%cM", hr,min,sec,time);
  glcd_text57(5,15,Pgeneral, 1, 1);
  sprintf(Pgeneral,"\%02d/\%02d/\%02d", dia,mes,yr);
  glcd_text57(5,25,Pgeneral, 1, 1);

    if(dow == 1)
      glcd_text57(60,25,DOMINGO, 1, 1);
    if(dow == 2)
      glcd_text57(60,25,LUNES, 1, 1);
    if(dow == 3)
      glcd_text57(60,25,MARTES, 1, 1);
    if(dow == 4)
      glcd_text57(60,25,MIERCOLES, 1, 1);
    if(dow == 5)
      glcd_text57(60,25,JUEVES, 1, 1);
    if(dow == 6)
      glcd_text57(60,25,VIERNES, 1, 1);
    if(dow == 7)
      glcd_text57(60,25,SABADO, 1, 1);
  
   glcd_rect(8,37,43,58,YES,ON);
   glcd_rect(46,37,81,58,YES,ON);
   glcd_rect(84,37,119,58,YES,ON);
   
 //===============================
   arrow();
//===============================
   
   glcd_rect(9,38,42,57,NO,OFF);

   
//   glcd_rect(47,38,80,57,NO,OFF);
//   glcd_rect(85,38,118,57,NO,OFF);

   glcd_text57(14,45,SET_TIME, 1, 0);  
   glcd_text57(49,45,SETDATE, 1, 0); 
//================================================================    
//================================================================  
  START2:
  s = read_adc();
  delay_ms(50);
  
  while(s>230){
  leerRTC();
  delay_ms(50);
  
      if(sec != oldsec){
          glcd_text57(5,15,Pgeneral, 1, 0);  
          sprintf(Pgeneral,"\%02d:\%02d:\%02d \%cM", hr,min,sec,time);
          glcd_text57(5,15,Pgeneral, 1, 1);  
          oldsec = sec;   
      }
  
  s = read_adc();
  delay_us(40);
  }
  
   if(s > 195 && s < 225) //enter
       goto CONF_HORA;
   else if (s > 165 &&  s < 185){
       Return1:
       glcd_rect(9,38,42,57,NO,ON);
       glcd_rect(47,38,80,57,NO,OFF);
       glcd_rect(85,38,118,57,NO,ON);
       delay_ms(500);
       s = read_adc();
       delay_ms(50);
       while(s > 240){
            s = read_adc();
            delay_ms(50);//********************
            }
       if(s > 195 && s < 225) //enter
           goto CONF_FECHA;
       if(s > 115 && s < 135){ //devolver
           
           glcd_rect(9,38,42,57,NO,OFF);
           glcd_rect(47,38,80,57,NO,ON);
           glcd_rect(85,38,118,57,NO,ON);
           delay_ms(50);
           goto START2;
       }   
       else if(s > 165 &&  s < 185){ //SALIR
           glcd_rect(9,38,42,57,NO,ON);
           glcd_rect(47,38,80,57,NO,ON);
           glcd_rect(85,38,118,57,NO,OFF);
           delay_ms(100);
           s = read_adc();
           delay_us(40);
           while(s > 240){
           s = read_adc();
           delay_ms(50);//******
            }
            
           if(s > 195 && s < 225) //enter
           goto SIGUIENTE;
           else if(s > 115 && s < 135)
               goto Return1;
           else if(s > 165 &&  s < 185)
               goto START2;
           else 
               goto START2;    
       }    
   }
    goto START2; 
//========================================================================================
//=========================================================================================             
    CONF_HORA:
    
         glcd_fillScreen(0);
         glcd_rect(0,0,127,63,NO,ON);
         glcd_rect(1,1,126,62,NO,ON);
         glcd_text57(22,5,AJUSTEHORA, 1, 1);
         sprintf(Pgeneral,"\%02d:\%02d \%cM", hr,min,time);
         glcd_text57(20,18,Pgeneral, 2, 1);
         
         glcd_rect(20,34,40,35,YES,ON);
         glcd_rect(8,40,119,58,YES,ON);
         glcd_text57(34,46,ESTABLECER, 1, 0);
         
         hrset = hr;
         minset = min;
         dayset= dia;
         monthset = mes;
         yrset = yr;
        
        START4:
         glcd_rect(20,34,40,35,YES,ON);
         glcd_rect(52,34,73,35,YES,OFF); 
         glcd_rect(85,34,107,35,YES,OFF);
         
         glcd_rect(9,41,118,57,NO,ON);
             s = read_adc();    
             delay_ms(50);
  
             while(s>230){
                   s = read_adc();    
                   delay_ms(50);
             }
             if (s > 10 && s < 40){
                 
                 if(hrset > 11)
                     hrset = 0;
                 hrset ++;
                 
                 glcd_rect(20,18,40,32,YES,OFF);
                 sprintf(Pgeneral,"%02d", hrset);
                 glcd_text57(20,18,Pgeneral, 2, 1);
                 delay_ms(100);
                 goto START4;
             }
             else if(s > 60 && s < 90){
                 
                 if(hrset < 1 )
                     hrset = 13;
                 hrset --;
                 glcd_rect(20,18,40,32,YES,OFF);
                 sprintf(Pgeneral,"%02d", hrset);
                 glcd_text57(20,18,Pgeneral, 2, 1);
                 delay_ms(100);
                 goto START4;
             }
             
             else if(s > 165 &&  s < 185){
                 
             START5: 
             glcd_rect(20,34,40,35,YES,OFF);
             glcd_rect(52,34,73,35,YES,ON);
             glcd_rect(85,34,107,35,YES,OFF);
             glcd_rect(9,41,118,57,NO,ON);
             delay_ms(200);
             s = read_adc();    
             delay_ms(50);
             while(s>230){
                   s = read_adc();    
                   delay_ms(50);
                   }
             if (s > 10 && s < 40){
                 minset ++;
                 if(minset > 59)
                     minset = 0;
                 
                 glcd_rect(52,18,74,32,YES,OFF);
                 sprintf(Pgeneral,"%02d", minset);
                 glcd_text57(52,18,Pgeneral, 2, 1);
                 delay_ms(200);
                 goto START5;
             }
              if(s > 60 && s < 90){
                 
                 minset --;
                 if(minset == -1 )
                     minset = 59;
                 glcd_rect(52,18,74,32,YES,OFF);
                 sprintf(Pgeneral,"%02d", minset);
                 glcd_text57(52,18,Pgeneral, 2, 1);
                 delay_ms(100);
                 goto START5;
             } 
              if (s > 115 && s < 135){
                 
                 glcd_rect(20,34,40,35,YES,ON);
                 glcd_rect(52,34,73,35,YES,OFF);
                 glcd_rect(85,34,107,35,YES,OFF);
                 glcd_rect(9,41,118,57,NO,ON);
                 delay_ms(200);
                 goto START4;
             }
              if(s > 165 &&  s < 185){
                 START6:
                 
                 glcd_rect(20,34,40,35,YES,OFF);
                 glcd_rect(52,34,73,35,YES,OFF); 
                 glcd_rect(85,34,107,35,YES,ON);
                 glcd_rect(9,41,118,57,NO,ON);
                 
                 delay_ms(200);
                 s = read_adc();    
                 delay_ms(50);
                 while(s>230){
                   s = read_adc();    
                   delay_ms(50);
                   }
                  if (s > 10 && s < 40){
                     
                     time = 1;
                     glcd_rect(85,18,102,32,YES,OFF);
                     glcd_text57(86,18,AM, 2, 1);
                     goto START6;
                  }
                   if (s > 60 && s < 90){
                     
                     time = 0;
                     glcd_rect(85,18,102,32,YES,OFF);
                     glcd_text57(86,18,PM, 2, 1);
                     goto START6;
                  }
                  if (s > 115 && s < 135)
                    goto START5;
                 
                 if(s > 165 &&  s < 185){
                    glcd_rect(85,34,107,35,YES,OFF); 
                    glcd_rect(9,41,118,57,NO,OFF);
                    START7:
                    delay_ms(200);
                    s = read_adc();    
                    delay_ms(50);
                    while(s>230){
                       s = read_adc();    
                       delay_ms(50);
                    }
                    if(s > 195 && s < 225)
                        goto SET_RTC;
                    if(s > 115 && s < 135)
                        goto START6;
                 }
                 else
                     goto START7;
             }  
          }
             else 
                 goto START4;
             goto START4;
//========================================================================================
//=========================================================================================             

    CONF_FECHA:
    
         glcd_fillScreen(0);
         glcd_rect(0,0,127,63,NO,ON);
         glcd_rect(1,1,126,62,NO,ON);
         glcd_text57(22,5,AJUSTEFECHA, 1, 1);
         //year = yr+2000;
         sprintf(Pgeneral,"\%02d/\%02d/\%02d", dia,mes,yr);
         glcd_text57(20,18,Pgeneral, 2, 1);
         
         glcd_rect(20,34,40,35,YES,ON);
         glcd_rect(8,40,119,58,YES,ON);
         glcd_text57(34,46,ESTABLECER, 1, 0);
         
         hrset = hr;
         minset = min;
         dayset = dia;
         monthset = mes;
         yrset = yr;
         START8:
         glcd_rect(20,34,40,35,YES,ON);
         glcd_rect(52,34,73,35,YES,OFF); 
         glcd_rect(85,34,107,35,YES,OFF);
         
         glcd_rect(9,41,118,57,NO,ON);
             s = read_adc();    
             delay_ms(50);
  
             while(s>230){
                   s = read_adc();    
                   delay_ms(50);
             }
             if (s > 10 && s < 40){
                 
                 if(dayset > 30)
                     dayset = 0;
                 dayset ++;
                 glcd_rect(20,18,40,32,YES,OFF);
                 sprintf(Pgeneral,"%02d", dayset);
                 glcd_text57(20,18,Pgeneral, 2, 1);
                 delay_ms(100);
                 goto START8;
             }
             else if(s > 60 && s < 90){
                 dayset --;
                 if(dayset < 1 )
                     dayset = 31;
                 
                 glcd_rect(20,18,40,32,YES,OFF);
                 sprintf(Pgeneral,"%02d", dayset);
                 glcd_text57(20,18,Pgeneral, 2, 1);
                 delay_ms(100);
                 goto START8;
             }
             
             else if(s > 165 &&  s < 185){
                 
             START9: 
             glcd_rect(20,34,40,35,YES,OFF);
             glcd_rect(52,34,73,35,YES,ON);
             glcd_rect(85,34,107,35,YES,OFF);
             glcd_rect(9,41,118,57,NO,ON);
             delay_ms(200);
             s = read_adc();    
             delay_ms(50);
             while(s>230){
                   s = read_adc();    
                   delay_ms(50);
                   }
             if (s > 10 && s < 40){
                 monthset ++;
                 if(monthset > 12)
                     monthset = 1;
                 
                 glcd_rect(52,18,74,32,YES,OFF);
                 sprintf(Pgeneral,"%02d", monthset);
                 glcd_text57(52,18,Pgeneral, 2, 1);
                 delay_ms(200);
                 goto START9;
             }
              if(s > 60 && s < 90){
                 
                 monthset --;
                 if(monthset == 0 )
                     monthset = 12;
                 glcd_rect(52,18,74,32,YES,OFF);
                 sprintf(Pgeneral,"%02d", monthset);
                 glcd_text57(52,18,Pgeneral, 2, 1);
                 delay_ms(100);
                 goto START9;
             } 
              if (s > 115 && s < 135){
                 
                 glcd_rect(20,34,40,35,YES,ON);
                 glcd_rect(52,34,73,35,YES,OFF);
                 glcd_rect(85,34,107,35,YES,OFF);
                 glcd_rect(9,41,118,57,NO,ON);
                 delay_ms(200);
                 goto START8;
             }
              if(s > 165 &&  s < 185){
                 START10:
                 
                 glcd_rect(20,34,40,35,YES,OFF);
                 glcd_rect(52,34,73,35,YES,OFF); 
                 glcd_rect(85,34,107,35,YES,ON);
                 glcd_rect(9,41,118,57,NO,ON);
                 
                 delay_ms(200);
                 s = read_adc();    
                 delay_ms(50);
                 while(s>230){
                   s = read_adc();    
                   delay_ms(50);
                   }
                  if (s > 10 && s < 40){
                      yrset++;
                      if(yrset > 80)
                          yrset = 2;
                      glcd_rect(85,18,107,32,YES,OFF);
                      sprintf(Pgeneral,"%02d", yrset);
                      glcd_text57(87,18,Pgeneral, 2, 1);
                     goto START10;
                  }
                 
                   if (s > 60 && s < 90){
                     yrset--;
                      if(yrset < 2)
                          yrset = 80;
                      glcd_rect(85,18,107,32,YES,OFF);
                      sprintf(Pgeneral,"%02d", yrset);
                      glcd_text57(87,18,Pgeneral, 2, 1);
                     goto START10;
                  }
                  if (s > 115 && s < 135)
                    goto START9;
                 
                 if(s > 165 &&  s < 185){
                    glcd_rect(85,34,107,35,YES,OFF); 
                    glcd_rect(9,41,118,57,NO,OFF);
                    START11:
                    delay_ms(200);
                    s = read_adc();    
                    delay_ms(50);
                    while(s>230){
                       s = read_adc();    
                       delay_ms(50);
                    }
                    if(s > 195 && s < 225)
                        goto SET_RTC;
                    if(s > 115 && s < 135)
                        goto START10;
                 }
                 else
                     goto START11;
             }  
          }
             else 
                 goto START8;
             goto START8;
//========================================================================================
//=========================================================================================             

         
         
  SET_RTC:
  ds1307_set_date_time(dayset,monthset,yrset,dow,hrset,time,minset,0);
  glcd_fillScreen(0);
  glcd_text57(22,3,AJUSTEFECHA, 1, 1);
  glcd_finalizado();
  glcd_text57(45,25,DATOS, 1, 1);
  glcd_text57(45,35,DATOS2, 1, 1);
  delay_ms(3000);
  goto CONF_RTC;
         
         
         
         
    SIGUIENTE:
         
        glcd_fillScreen(0);
        glcd_rect(0,0,127,63,NO,ON);
        glcd_rect(1,1,126,62,NO,ON);
        glcd_text57(28,5,EEPROM, 1, 1);
        glcd_rect(10,14,25,33,YES,ON);
        glcd_rect(8,15,10,17,YES,ON);
        glcd_rect(8,20,10,22,YES,ON);
        glcd_rect(8,25,10,27,YES,ON);
        glcd_rect(8,30,10,32,YES,ON);
        glcd_rect(25,15,27,17,YES,ON);
        glcd_rect(25,20,27,22,YES,ON);
        glcd_rect(25,25,27,27,YES,ON);
        glcd_rect(25,30,27,32,YES,ON);
        glcd_text57(35,15,CAPACITY, 1, 1);
        glcd_text57(35,25,UNREAD, 1, 1);
        
        glcd_rect(4,37,41,58,YES,ON);
        glcd_rect(43,37,83,58,YES,ON);
        glcd_rect(85,37,123,58,YES,ON);
        glcd_text57(10,45,LEER, 1, 0);  
        glcd_text57(47,45,BORRAR, 1, 0); 
        arrow();
        //glcd_text57(90,45,SALIR, 1, 0); 
        
        
//================================================================    
//================================================================  
  START3:
  glcd_rect(5,38,40,57,NO,OFF);
  s = read_adc();
  delay_ms(50);
  
  while(s>230){
  s = read_adc();
  delay_us(40);
  }
  
   if(s > 195 && s < 225) //enter
       goto LEER_EEPROM;
   else if (s > 165 &&  s < 185){
       Return2:
       glcd_rect(5,38,40,57,NO,ON);
       glcd_rect(44,38,82,57,NO,OFF);
       glcd_rect(86,38,122,57,NO,ON);
       delay_ms(500);
       s = read_adc();
       delay_ms(50);
       while(s > 240){
            s = read_adc();
            delay_ms(50);//********************
            }
       if(s > 195 && s < 225) //enter
           goto BORRAR_EEPROM;
       if(s > 115 && s < 135){ //devolver
           
           glcd_rect(5,38,40,57,NO,OFF);
           glcd_rect(44,38,82,57,NO,ON);
           glcd_rect(86,38,122,57,NO,ON);
           delay_ms(50);
           goto START3;
       }   
       else if(s > 165 &&  s < 185){ //avanzar
           glcd_rect(5,38,40,57,NO,ON);
           glcd_rect(44,38,82,57,NO,ON);
           glcd_rect(86,38,122,57,NO,OFF);
           delay_ms(100);
           s = read_adc();
           delay_us(40);
           while(s > 240){
           s = read_adc();
           delay_ms(50);//********************
            }
           if(s > 195 && s < 225) //enter
           goto SIGUIENTE2;
           else if(s > 115 && s < 135)
               goto Return2;
           else if(s > 165 &&  s < 185)
               goto START3;
           else 
               goto START3;    
       }    
   }
    goto START3; 
//================================================================  
//================================================================ 
    LEER_EEPROM:
     glcd_rect(35,25,60,35,YES,OFF);
    
//     for(eread=0;eread<1024;eread++){
//         write_ext_eeprom(eread,0x38);
//         delay_us(100);
//     }
//   
    glcd_rect(4,37,41,58,YES,ON);
 //   glcd_rect(5,38,40,57,NO,OFF);
    
    glcd_text57(15,45,LOADING 1, 0);
    actualbyte = 0;
    for(eread=0;eread<256;eread++){
        
        buffer = read_ext_eeprom(eread);
        delay_us(200);
        
        if(buffer != 0xFF){
            actualbyte++;

            if(actualbyte != oldactualbyte){
           
                 glcd_text57(35,25,Pgeneral, 1, 0); 
                 sprintf(Pgeneral,"%lu", actualbyte);
                 glcd_text57(35,25,Pgeneral, 1, 1); 
                 oldactualbyte = actualbyte;
            }
        }
    }
    
    glcd_text57(35,25,UNREAD, 1, 1);
    glcd_rect(35,25,60,35,YES,OFF);
    sprintf(Pgeneral,"%lu", actualbyte);
    glcd_text57(35,25,Pgeneral, 1, 1); 
    
    glcd_rect(4,37,41,58,YES,ON);
    glcd_rect(5,38,40,57,NO,OFF);
    glcd_text57(10,45,LEER, 1, 0);  
    goto START3;
    

    BORRAR_EEPROM:
    
    glcd_rect(43,37,83,58,YES,ON);
    glcd_text57(56,45,LOADING 1, 0);
    
     for(eread=0;eread<1024;eread++){
         write_ext_eeprom(eread,0xFF);
         delay_us(10);
     }
    
    write_ext_eeprom(0,0);
    write_ext_eeprom(1,0);
//    write_ext_eeprom(2,16);
//    write_ext_eeprom(3,12);
//    write_ext_eeprom(4,11);
//    write_ext_eeprom(5,23);
//    write_ext_eeprom(6,2);
//    write_ext_eeprom(7,30);
//    write_ext_eeprom(8,8);
//    write_ext_eeprom(9,6);
//    write_ext_eeprom(10,16);
////==========================    
//    write_ext_eeprom(11,17);
//    write_ext_eeprom(12,63);
//    write_ext_eeprom(13,12);
//    write_ext_eeprom(14,68);
//    write_ext_eeprom(15,4);
//    write_ext_eeprom(16,30);
//    write_ext_eeprom(17,9);
//    write_ext_eeprom(18,6);
//    write_ext_eeprom(19,16);
////==========================    
//    write_ext_eeprom(20,9);
//    write_ext_eeprom(21,58);
//    write_ext_eeprom(22,11);
//    write_ext_eeprom(23,14);
//    write_ext_eeprom(24,6);
//    write_ext_eeprom(25,30);
//    write_ext_eeprom(26,10);
//    write_ext_eeprom(27,6);
//    write_ext_eeprom(28,16);
////==========================    
//    write_ext_eeprom(29,14);
//    write_ext_eeprom(30,78);
//    write_ext_eeprom(31,6);
//    write_ext_eeprom(32,87);
//    write_ext_eeprom(33,8);
//    write_ext_eeprom(34,30);
//    write_ext_eeprom(35,11);
//    write_ext_eeprom(36,6);   
//    write_ext_eeprom(37,16);

    
    glcd_text57(35,25,UNREAD, 1, 1);
    glcd_rect(35,25,60,35,YES,OFF);
    actualbyte = 0;
    sprintf(Pgeneral,"%lu", actualbyte);
    glcd_text57(55,25,Pgeneral, 1, 1); 
    glcd_rect(43,37,83,58,YES,ON);
    glcd_text57(47,45,BORRAR, 1, 0); 
    
    goto START3;
//========================================================================================================  
//========================================================================================================     
    SIGUIENTE2:
    glcd_fillScreen(0);
    glcd_rect(0,0,127,63,NO,ON);
    glcd_rect(1,1,126,62,NO,ON);
    glcd_text57(5,5,CONSUMO, 1, 1); 
    
    glcd_text57(5,18,CONSULTARC, 1, 1);
    glcd_text57(5,28,CONSULTARC2, 1, 1);
    
    glcd_rect(4,37,41,58,YES,ON);
    glcd_rect(43,37,83,58,YES,ON);
    glcd_rect(85,37,123,58,YES,ON);
    
    glcd_rect(5,38,40,57,NO,OFF);
      
    glcd_text57(17,45,SI, 1, 0);  
    glcd_text57(47,45,VOLVERR, 1, 0);
   glcd_text57(90,45,SALIR, 1, 0); 
   
   
  START20: //2
  s = read_adc();
  delay_ms(50);
  
  while(s>230){
  s = read_adc();
  delay_us(80);
  }
  
   if(s > 195 && s < 225) //enter
       goto CONSULTAR_CONSUMOS;
   else if (s > 165 &&  s < 185){
       Return10: //1
       glcd_rect(5,38,40,57,NO,ON);
       glcd_rect(44,38,82,57,NO,OFF);
       glcd_rect(86,38,122,57,NO,ON);
       delay_ms(500);
       s = read_adc();
       delay_ms(50);
       while(s > 240){
            s = read_adc();
            delay_ms(50);//********************
            }
       if(s > 195 && s < 225) //enter
           goto VOLVER;
       if(s > 115 && s < 135){ //devolver
           
           glcd_rect(5,38,40,57,NO,OFF);
           glcd_rect(44,38,82,57,NO,ON);
           glcd_rect(86,38,122,57,NO,ON);
           delay_ms(50);
           goto START20; //2
       }   
       else if(s > 165 &&  s < 185){ //avanzar
           glcd_rect(5,38,40,57,NO,ON);
           glcd_rect(44,38,82,57,NO,ON);
           glcd_rect(86,38,122,57,NO,OFF);
           delay_ms(100);
           s = read_adc();
           delay_us(40);
           while(s > 240){
           s = read_adc();
           delay_ms(50);//********************
            }
           if(s > 195 && s < 225) //enter
           goto SISTEMA;
           else if(s > 115 && s < 135)
               goto Return10;//1
           else if(s > 165 &&  s < 185)
               goto START20;
           else 
               goto START20;    
       }    
   }
    goto START20; 
    
//========================================================================================================      
    
    CONSULTAR_CONSUMOS:
    consulta_datos();
    if(exitflag == 1){
        exitflag = 0;
        goto SISTEMA;
    }
//========================================================================================================  
    VOLVER:
        delay_ms(2000);
//========================================================================================================  
//    EXIT:
//    
//    delay_ms(2000);
//========================================================================================================  
//========================================================================================================    
// ?HABILITAR CONTROL?====================
 SISTEMA:
   glcd_fillScreen(0);
   glcd_rect(0,0,127,63,NO,ON);
   glcd_rect(1,1,126,62,NO,ON);
   glcd_text57(5,5,MENSAJE1, 1, 1);
   glcd_text57(10,20,H_CONTROL, 1, 1);
   glcd_rect(5,37,61,58,YES,ON);
   glcd_rect(65,37,120,58,YES,ON);
   glcd_text57(28,44,SI, 1, 0);
   glcd_text57(86,44,NOO, 1, 0);
   
   glcd_rect(6,38,60,57,NO,OFF);
 
   START100:
   s = read_adc();
   delay_ms(100); //************************
   
   while(s > 240){
    s = read_adc();
    delay_ms(50);
   }
   
   if(s > 195 && s < 225) //enter
       goto PRESET_CONTROL;
   else if(s > 165 &&  s < 185){
       glcd_rect(6,38,60,57,NO,ON);
       glcd_rect(66,38,119,57,NO,OFF);
       delay_ms(100);
       s = read_adc();
       delay_us(40);
       while(s > 240){
            s = read_adc();
            delay_ms(50);//********************
            }
       if(s > 195 && s < 225){
           c = 2;
           goto SISTEMA3;
       }
       if(s > 115 && s < 135){
           glcd_rect(66,38,119,57,NO,ON);
           glcd_rect(6,38,60,57,NO,OFF);
           delay_ms(50);
           goto START100;
       }   
       else 
           goto START100;
   }
    goto START100;
    
 //=====================================================      
 //=====================================================   
    
 //=====================================================      
 //=====================================================  
    PRESET_CONTROL:
    glcd_fillScreen(0);
    glcd_control();
    glcd_text57(18,4,T_CONTROL, 1, 1);
    glcd_rect(9,15,61,46,NO,ON);
    glcd_rect(9,48,117,59,YES,ON);
    glcd_text57(30,50,SELECCIONAR, 1, 0);
    //glcd_rect(64,15,117,46,NO,ON);
    SET_CONTROL:
    s = read_adc();
    delay_ms(200);
    
    while(s>240){
        s = read_adc();
        delay_ms(200);
    }
    
     if(s > 195 && s < 225) {
         c = 0;
         goto SISTEMA2;
     }
     else if (s > 165 &&  s < 185){
       glcd_rect(9,15,61,46,NO,OFF);
       glcd_rect(64,15,117,46,NO,ON);
       delay_ms(100);
       s = read_adc();
       delay_us(40);
       while(s > 240){
            s = read_adc();
            delay_ms(50);//********************
            }
       if(s > 195 && s < 225){
           c = 1;
           goto SISTEMA2;
       }
           
       if(s > 115 && s < 135){
           glcd_rect(9,15,61,46,NO,ON);
           glcd_rect(64,15,117,46,NO,OFF);
           delay_ms(50);
           goto SET_CONTROL;
       }   
       else 
           goto SET_CONTROL;
   }
    goto SET_CONTROL;
    
//=====================================================      
//=====================================================   
   SISTEMA2:
   glcd_fillScreen(0);
   glcd_rect(0,0,127,63,NO,ON);
   glcd_rect(1,1,126,62,NO,ON);
   glcd_text57(9,5,LIMITE, 1, 1);
   glcd_rect(9,48,117,59,YES,ON);
   glcd_text57(30,50,SELECCIONAR, 1, 0);
   l_ahorro = 1;
   sprintf(Pgeneral,"%luL", l_ahorro);
   glcd_text57(40,20,Pgeneral 2, 1);

   Loop3:
   s = read_adc();
   delay_ms(200);

   while(s>240){
       s = read_adc();
       delay_ms(200);
   }

    glcd_rect(40,20,110,39,YES,OFF);
    if(s > 195 && s < 225) {
         goto SISTEMA3;
     }
   
    if (s > 10 && s < 40){
        l_ahorro = l_ahorro+1;
        sprintf(Pgeneral,"%luL", l_ahorro);
        glcd_text57(40,20,Pgeneral 2, 1);
    }
       
    if (s > 60 && s < 90){
        l_ahorro = l_ahorro-1;
        sprintf(Pgeneral,"%luL", l_ahorro);
        glcd_text57(40,20,Pgeneral 2, 1);
    }
    goto Loop3;    
  
    
    
//=====================================================  
   SISTEMA3:
    glcd_fillScreen(0);
    glcd_rect(0,0,127,63,NO,ON);
    glcd_rect(1,1,126,62,NO,ON);
    glcd_text57(5,5,MENSAJE1, 1, 1);
    glcd_text57(20,23,READY, 1, 1);
    
    glcd_rect(8,40,119,58,YES,ON);
    glcd_rect(9,41,118,57,NO,OFF);
    glcd_text57(24,46,COMENZARS, 1, 0);
    
    START13:
    delay_ms(200);
    s = read_adc();
    delay_us(40);
    while(s > 240){
     s = read_adc();
     delay_ms(50);
     set_timer0(0);
     set_timer1(0);
     set_timer3(0);
    }

    if(s > 195 && s < 225)
        goto EMPEZAR;
    else
        goto START13;
    
    EMPEZAR:
    
//    L_ahorro = 500;
//    set_timer0(0);
//    set_timer1(0);
//    while(L_wasted != 0 ){
////    totalwater = 0;
//    L_wasted = L_wasted*0;
//    }
//

    glcd_fillScreen(0);
    glcd_interfaz();
    while(TRUE){
    leerRTC();
//    leernivel();
    delay_us(200);
//===============================   
    if( c == 0 || c == 1){

       if(l_ahorro + nuevo <= L_wasted ){  // + nuevo
          glcd_fillScreen(0);
          glcd_rect(0,0,127,63,NO,ON);
          glcd_rect(1,1,126,62,NO,ON);
          glcd_text57(15,30,LIMITE_ALC, 1, 1);
          s = read_adc();
          delay_ms(200);
          nuevo = L_wasted;

          while(s>240){
          s = read_adc();
          delay_ms(200);
          Percent = Percent*0;
          }

          if(s < 240)
              goto REBOOT;
      }


        controlador();
     
    }  
//================================    
    sprintf(Pgeneral,"\%02d/\%02d/\%02d", dia,mes,yr);
    glcd_text57(76,53,Pgeneral, 1, 1);
    
    if(oldnivel != nivel){
    glcd_text57(15,53,Pgeneral5, 1, 0);
    sprintf(Pgeneral5,"%.1fCm", nivel);
    glcd_text57(15,53,Pgeneral5, 1, 1);
    oldnivel = nivel;
    }
    
//    sprintf(Pgeneral,"\%02d:\%02d:\%02d", hr,min,sec);
//    glcd_text57(70,53,Pgeneral, 1, 1);
//    hrswitch++;
//    cyclechange = 1;

//    sprintf(Pgeneral,"\%02d:\%02d:\%02d", hr,min,sec);
//    glcd_text57(70,53,Pgeneral, 1, 0);
//    sprintf(Pgeneral,"\%02d/\%02d/\%02d", dia,mes,yr);
//    glcd_text57(70,53,Pgeneral, 1, 1);
//    dateswitch++;
//    cyclechange = 1;
//   

      
 //=====================================================
  //  ESPACIO PARA LAS ESCUACIONES
  //  MAGNITUD DE VARIABLE Y ACUMULACI?N
  //  EN LITROS
  disable_interrupts(GLOBAL); 
  set_timer0(0);
  set_timer1(0);
  delay_ms(1000);//=========================================

  pulsos_ducha = get_timer0();
  pulsos_lavamanos = get_timer1();
//  t0count = get_timer0();
//  t1count = get_timer1();
  //========VARIABLES POR UNIDAD DE TIEMPO==================

//  t0count = (pulsos_ducha*0.00225)*2; // litros por segundo
//  t0count = t0count*60; // Litros por minuto
  
     t0count =  ((0.002564*pulsos_ducha) - 0.012821)*1000;
       if(t0count <= 0)
          t0count = 0;
     //t0count_continuous = t0count_continuous + t0count;

  

     if(oldt0count != t0count){
        glcd_text57(20,25,Pgeneral2, 1, 0);
        sprintf(Pgeneral2,"%.1fmL",t0count);
        glcd_text57(20,25,Pgeneral2, 1, 1);
        oldt0count = t0count;
      }
//
//      if(oldt1count != t1count){
//        glcd_text57(20,25,Pgeneral2, 1, 0);
//        sprintf(Pgeneral2,"%.2f",t1count);
//        glcd_text57(20,25,Pgeneral2, 1, 1);
//        oldt1count = t1count;
//      }
  litros_ducha = litros_ducha + t0count;
  sum_ducha = (t0count/1000) + sum_ducha ;
  set_timer0(0);
//  =====================================================
//    t1count = (pulsos_lavamanos*0.00225)*2; // litros por segundo
//    t1count = t1count*60; // Litros por minuto
  
  t1count =  ((0.002564*pulsos_lavamanos) - 0.012821)*1000;
  if(t1count <= 0)
      t1count = 0;

 // t1count_continuous = t1count_continuous + t1count;
  
        if(oldt1count != t1count){
        glcd_text57(20,39,Pgeneral3, 1, 0);
        sprintf(Pgeneral3,"%.1fmL",t1count);
        glcd_text57(20,39,Pgeneral3, 1, 1);
        oldt1count = t1count;
      }
    litros_lavamanos = litros_lavamanos + t1count;
    sum_lav = (t1count/1000) + sum_lav;
    set_timer1(0);
  //========CONSUMO EN LITROS Y SUMATORIA===================
  //======================================================
//if (sum_lav<0 && sum_ducha<0 ){
//     totalwater=0;
//    }
//    if (sum_lav>0 && sum_ducha<0 ){
//     totalwater=sum_lav;
//    }
//    if (sum_lav<0 && sum_ducha>0 ){
//     totalwater=sum_ducha;
//    }
//    if(sum_lav>0 && sum_ducha>0){
//     totalwater= sum_lav + sum_ducha;
//    }

     totalwater = sum_lav + sum_ducha;

     L_wasted = totalwater;

     if(oldtotalwater != totalwater){
        glcd_text57(76,25,Pgeneral4, 1, 0);
        sprintf(Pgeneral4,"%.2fL",totalwater);
        glcd_text57(76,25,Pgeneral4, 1, 1);
        oldtotalwater = totalwater;
      }
      enable_interrupts(GLOBAL); 


      if(lavamanos == 1){
          glcd_circle(115,42,3,1,1);
          output_high(PIN_C1);
          lavamanos = 0;
      }
      if (input_state(PIN_B0) == 0){
          glcd_circle(115,42,3,1,0);
          output_low(PIN_C1);
      }
//      if (input_state(PIN_A3) == 0 && input_state(PIN_B1) == 0){
//          leernivel();
//          if(nivel > 3)
//              output_high(PIN_C2);
//      }
      
      if(tanquelleno == 1){
          tanquelleno = 0;
          output_low(PIN_C2);
      }
      

       
          
      ds1307_get_time(hr,am_pm,min,sec);
   if ((sec == 59 || sec==0 ||sec == 1) && min==0  && (hr==2 || hr==4 || hr==6 || hr==8 || hr==10 || hr ==12)){
  //  if(sec==0 || sec==10 || sec==20  || sec==30  || sec==40  || sec==50){
          storeDATA();
          glcd_fillScreen(0);
          glcd_interfaz();
          sprintf(Pgeneral2,"%.1fmL",t0count);
          glcd_text57(20,25,Pgeneral2, 1, 1);
          sprintf(Pgeneral3,"%.1fmL",t1count);
          glcd_text57(20,39,Pgeneral3, 1, 1);

      }

    }

}
