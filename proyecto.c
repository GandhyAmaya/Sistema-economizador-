

#include <DS1307.c>
#include <24c321.c>

#define trig pin_c1
#define echo pin_c0

//====================================DEFINICIÓN DE VARIABLES===============================
//==========================================================================================
int16 litros_float,litros_int,pulses,CONTADOR,duc_pulsos,inod_pulsos,nivel_ducha,nivel_inodoro,nivel_bajoducha,FREC;
float flujo,lav_pulsos,lav_litros,duc_litros,inod_litros,distance,time;
int  init_eeprom,i=0,t_overflow,sec,min,hrs,day,month,yr,id,dow,lavamanos,ducha,inodoro;
char fecha[3]; 
//==========================================================================================
//==========================================================================================
#INT_TIMER0 
void  TIMER0_isr(void)  
{
t_overflow=t_overflow + 1;
}
//================================FUNCIÓN DE MENU DEL SISTEMA===============================
//==========================================================================================
void menu(){
     lcd_init();
     lcd_putc("\f1.Calendario");
     lcd_putc("\n2.Sensores");
     delay_ms(500);
     while(input_state(pin_b5) == 0 && input_state(pin_b6) == 0 && input_state(pin_b7) == 0)
     {}

if(input_state(pin_b5) == 1)
   id=1;
else 
   if(input_state(pin_b6) == 1)
   id=5;   
   return;
}
//==========================================================================================
//==========================================================================================

//==============================FUNCIÓN DE LECTURA DE SENSORES==============================
//==========================================================================================
void sensores(){

lcd_init();
printf(LCD_PUTC, "\f TEST \n SENSORS");
delay_ms(100);

set_tris_d(0x1f);

while(true){

//============ULTRASONICO=================
//!output_high(trig);
//!delay_us(20);
//!output_low(trig);
//!while(!input(ECHO))
//!{}
//!set_timer1(0);
//!while(input(ECHO))
//!{}
//!time=get_timer1();
//!if(time<180)  {
//!output_high(pin_D1);
//!}
//!ELSE if (time>180){
//!output_LOW(pin_D1);
//!}     
//===========================================================================================
//================CALCULO DE LITROS CADA 2 HORAS Y GUARDADO EN EEPROM========================

ds1307_get_time (hrs, min, sec);
delay_ms(300);

//if(sec==0 && min==0  && (hrs==2 || hrs==4 || hrs==6 || hrs==8 || hrs==10)) -------------> ESTA ESCRIBE CADA DOS HORAS
if(sec==0 || sec==10 || sec==20  || sec==30  || sec==40  || sec==50)  //------------> ESTA ESCRIBE CADA 10 SEGUNDOS, 
                                                                      //              PARA PROBAR
{

disable_interrupts(int_timer0);
pulses = t_overflow*256 + get_timer0();
lav_litros= ((0.0657*t_overflow*t_overflow-4.17*t_overflow+78)/ 60.0);

set_timer0(0);
enable_interrupts(int_timer0);

litros_int = lav_litros;
litros_float = (lav_litros - litros_int)*10; 

//======================================================
//Escritura de bloques de 5 casillas en la EEPROM=======
//|    0    |    1   |     2    | 3  |  4  |  5  |
//------------------------------------------------------
//|Reservada| LT int | LT Float | HR | MIN | DIA |
//| para i  | LT int | LT Float | HR | MIN | DIA |
//======================================================
//======================================================
lcd_init();
printf(LCD_PUTC, "   ESCRIBIENDO");
lcd_gotoxy(1,2);
printf(LCD_PUTC, "      DATOS");

i = read_ext_eeprom(0);
delay_ms(1);
i++;
write_ext_eeprom(i,litros_int);
delay_ms(1);
i++;
write_ext_eeprom(i,litros_float);
delay_ms(1);
i++;
write_ext_eeprom(i,hrs);
delay_ms(1);
i++;
write_ext_eeprom(i,min);
delay_ms(1);
i++;
write_ext_eeprom(i,day);
delay_ms(1);

write_ext_eeprom(0,i);
delay_ms(2000);
lcd_init();
printf(LCD_PUTC, "DATOS ESCRITOS!");
delay_ms(2000);
}
//==========================================================================================
//==========================================================================================
//! Printf(lcd_putc,"\f p%lu  T:%3f \nlv:%3.1f l/min",FREC,TIME,lav_litros);
//! if(input_state(pin_b7) == 1)
//!      menu();
//!   }    
//!}
//==========================================================================================
//==========================================================================================
}
}
void main()
{

lcd_init();

enable_interrupts(global);
enable_interrupts(int_ext);
enable_interrupts(int_rb);
enable_interrupts(int_timer0);
setup_timer_1(T1_INTERNAL|T1_DIV_BY_2);
setup_timer_0 (RTCC_EXT_L_TO_H|RTCC_DIV_1|RTCC_8_BIT);
set_timer0(0);

ds1307_init(DS1307_ALL_DISABLED);
ds1307_init(DS1307_OUT_ENABLED | DS1307_OUT_4_KHZ);

    //ds1307_set_date_time(07,05,16,5,01,33,0);
    //delay_ms(20);
    
//============INICIALIZACIÓN DE LA EEPROM EXTERNA========================   
   init_eeprom = read_ext_eeprom(0);
   if(init_eeprom == 0xFF){
      write_ext_eeprom(0,i);
   }
//========================================================================
sensores();
menu();

if(id==1)
  goto CALENDARIO;
  
if(id==5)
  sensores();

while (true){

      CALENDARIO:
      lcd_init();
      
      ds1307_get_date (day, month, yr, dow);
      if(input_state(pin_b6) == 1)
      menu(); 
      ds1307_get_time (hrs, min, sec);
      if(input_state(pin_b6) == 1)
      menu(); 
      ds1307_get_day_of_week(fecha);
      if(input_state(pin_b6) == 1)
      menu(); 
      lcd_gotoxy(1,1);
      printf(lcd_putc,"%s %02u/%02u/20%02u",fecha,day,month,yr);
      
      
      
      lcd_gotoxy(2,2);
      printf(lcd_putc, "HORA: %02u:%02u:%02u",hrs, min, sec);
      
      
      delay_ms(1000);
      if(input_state(pin_b7) == 1)

      menu(); 

  }

}



  
