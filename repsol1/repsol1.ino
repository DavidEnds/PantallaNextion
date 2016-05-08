
#include "NexButton.h" //libreria para botones
#include "NexText.h"  //libreria para texto
#include "Arduino.h" 
#include "Wire.h"  //libreria para i2c del RTC
#include "RTCLib.h"  //libreria para RTC
#include "NexPage.h"  //libreria para paginas

//page id numero de la pagina
//component id es un numero asignado por el editor
//lo mandas al presionar si tienes la casilla send component id marcada en pressed

NexButton b0 = NexButton(0, 2, "b0");  // (page id, component id, nombre puntero a un unico nombre )
NexButton b1 = NexButton(0, 3, "b1");  // (page id, component id, nombre puntero a un unico nombre )
NexButton b2 = NexButton(0, 4, "b2");  // (page id, component id, nombre puntero a un unico nombre )
NexButton b3 = NexButton(0, 5, "b3");  // (page id, component id, nombre puntero a un unico nombre )
NexButton b4 = NexButton(0, 17, "b4"); // (page id, component id, nombre puntero a un unico nombre )

NexText t5 = NexText(0, 10, "t5");     // (page id, component id, nombre puntero a un unico nombre )
NexText t6 = NexText(0, 11, "t6");     // (page id, component id, nombre puntero a un unico nombre )
NexText t7 = NexText(0, 12, "t7");     // (page id, component id, nombre puntero a un unico nombre )
NexText t8 = NexText(0, 13, "t8");     // t8 nombre dentro del codigo segundo t8 dentro de la pantalla
NexText t9 = NexText(0, 14, "t9");
NexText t13 = NexText(0, 19, "t13");
NexText t16 = NexText(0, 22, "t16");
NexText t18 = NexText(0, 24, "t18");

NexButton d6 = NexButton(1, 15, "d6");  //en otra pagina (pantalla 1) los botones pasan a llamarse "d"
NexText d7 = NexText(1, 14, "d7");      //los de navegacion 

NexPage page0 = NexPage(0, 0, "page0");   // (page id, component id, nombre puntero a un unico nombre )
NexPage page1 = NexPage(1, 0, "page1");

NexButton k1 = NexButton(1, 2, "k1");   // los botones de la otra pantalla que no son de navegacion los llama k
NexButton k2 = NexButton(1, 3, "k2");   //al cambiar de pagina tienes diferentes ids puedes usar los mismos numeros que en otra
NexButton k3 = NexButton(1, 4, "k3");   
NexButton k4 = NexButton(1, 5, "k4");
NexButton k5 = NexButton(1, 6, "k5");
NexButton k6 = NexButton(1, 7, "k6");
NexButton k7 = NexButton(1, 8, "k7");
NexButton k8 = NexButton(1, 9, "k8");

char buffer[10] = {0};
char buffer_temp[10] = {0};
char buffer_year[10] = {0};
char buffer_month[10] = {0};
char buffer_day[10] = {0};
char buffer_hour[10] = {0};
char buffer_minute[10] = {0};
char buffer_second[10] = {0};
char buffer_week[10] = {0};

int8_t settime_type = -1;     //variables de seteo de la hora, pantalla de ajustes
int8_t settime_up = -1;
int8_t number_settime_year = 0;
int8_t number_settime_month = 0;
int8_t number_settime_day = 0;
int8_t number_settime_hour = 0;
int8_t number_settime_minute = 0;
int8_t number_settime_week = 0;

char buffer_temperature[10] = {0};

RTCLib rtc;
/*SENSORES Y RELES*/
uint8_t number = 0;          
uint16_t sensor_pin = A0;   /*select the input pin for the potentiometer*/
uint16_t sensor_value = 0;  /* variable to store the value coming from the sensor*/
uint16_t temperature = 0;   
uint16_t temp = 0;

NexTouch *nex_Listen_List[] = 
{
    &b0, &b1, &b2, &b3, &b4,
    &t5, &t6, &t7, &t9, &t8,
    &t13, &t16, &t18, &d7,
    &k1, &k2, &k3, &k4, &k5,
    &k6, &k7, &k8, &d6, &page0,
    &page1,
    NULL
};

void b4PopCallback(void *ptr) //boton de cambiar de pagina (b4)
{
    page1.show();               //mostramos pantalla 2
    k3.setText(buffer_year);    //ponemos en los campos de boton el texto del buffer del año
    k4.setText(buffer_month);   //ponemos en los campos de boton el texto del buffer del mes
    k5.setText(buffer_day);     //ponemos en los campos de boton el texto del buffer del dia
    k6.setText(buffer_hour);    //ponemos en los campos de boton el texto del buffer de la hora
    k7.setText(buffer_minute);  //ponemos en los campos de boton el texto del buffer del minuto
    k8.setText(buffer_week);    //ponemos en los campos de boton el texto del buffer de la semana

    number_settime_year = atoi(buffer_year); //convertimos las cadenas de los buffer del reloj a enteros
    number_settime_month = atoi(buffer_month);
    number_settime_day = atoi(buffer_day);
    number_settime_hour = atoi(buffer_hour);
    number_settime_minute = atoi(buffer_minute);
//para el seteo de la pantalla comprobamos si hemos selectionado domingo por ejemplo le pasamos el entero reloj
    if(strcmp(buffer_week, "Sunday") == 0) 
    {
        number_settime_week = 1;
    }
    if(strcmp(buffer_week, "Monday") == 0)
    {
        number_settime_week = 2;
    }
    if(strcmp(buffer_week, "Tuesday") == 0)
    {
        number_settime_week = 3;
    }
    if(strcmp(buffer_week, "Wednesday") == 0)
    {
        number_settime_week = 4;
    }
    if(strcmp(buffer_week, "Thursday") == 0)
    {
        number_settime_week = 5;
    }
    if(strcmp(buffer_week, "Friday") == 0)
    {
        number_settime_week = 6;
    }
    if(strcmp(buffer_week, "Saturday") == 0)
    {
        number_settime_week = 7;
    }
    if(strcmp(buffer_week, "Fail") == 0)
    {
        number_settime_week = -1;
    }
    
    
}

void d6PopCallback(void *ptr)  //si pulsamos el boton OK de la pantalloa de la hora
{                              //establecemos como hora la seteada en la pantalla 2
    rtc.set(0, number_settime_minute, number_settime_hour, 
            number_settime_week, number_settime_day, 
            number_settime_month, number_settime_year);
}

void d7PopCallback(void *ptr)  //volver para atras 
{
    page0.show();           //mostramos pantalla 1
    t5.setText(buffer_year);     //refrescamos los datos que cambian 
    t16.setText(buffer_month);
    t18.setText(buffer_day);
    t6.setText(buffer_hour);
    t13.setText(buffer_minute);
    t7.setText(buffer_week);
}


void k1PopCallback(void *ptr)  //tecla +1
{
    settime_up = 1;
    setTime();
}

void k2PopCallback(void *ptr)  //tecla -1
{
    settime_up = 0;      //No tendria que ser menos?
    setTime();
}

void k3PopCallback(void *ptr)
{
    settime_type = 1;   
}

void k4PopCallback(void *ptr)
{
    settime_type = 2;
}

void k5PopCallback(void *ptr)
{
    settime_type = 3;
}

void k6PopCallback(void *ptr)
{
    settime_type = 4;
}

void k7PopCallback(void *ptr)
{
    settime_type = 5;
}

void k8PopCallback(void *ptr)
{
    settime_type = 6;

}

void setTime(void)
{
    switch (settime_type) //argoritmo de cambio de hora
    {
        case 1: 
            if (settime_up == 1) //si pulsamos la tecla de arriba, subimos año
            {
               number_settime_year++;
               if (number_settime_year > 99)
               {
                    number_settime_year = 10;
               }
               memset(buffer, 0, sizeof(buffer));  
               itoa(number_settime_year, buffer, 10); 
               k3.setText(buffer);   //ponemos un texto en un boton 
            }   
            else if (settime_up == 0)  //si pulsamos la tecla menos bajamos año
            {
                number_settime_year--;
                if (number_settime_year < 10)
                {
                    number_settime_year = 99;
                }
                memset(buffer, 0, sizeof(buffer));  
                itoa(number_settime_year, buffer, 10);
                k3.setText(buffer);
            }
            break;
        case 2:
            if (settime_up == 1)
            {
                number_settime_month++;
                if (number_settime_month > 12)
                {
                    number_settime_month = 1;
                }
                memset(buffer, 0, sizeof(buffer));  
                itoa(number_settime_month, buffer, 10);
                k4.setText(buffer);
            }
            else if (settime_up == 0)
            {
                number_settime_month--;
                if (number_settime_month < 1)
                {
                    number_settime_month= 12;
                }
                memset(buffer, 0, sizeof(buffer));  
                itoa(number_settime_month, buffer, 10);
                k4.setText(buffer);
            }
            break;
        case 3:
        {
            memset(buffer, 0, sizeof(buffer)); 
            k4.getText(buffer,sizeof(buffer));
            uint8_t dayofmonth = atoi(buffer);
            if (settime_up == 1)
            {
                number_settime_day++;
                if (dayofmonth == 1 || dayofmonth == 3 || dayofmonth == 5 
                    || dayofmonth == 7 || dayofmonth == 8 || dayofmonth == 10
                    || dayofmonth == 12 )
                {
                    if (number_settime_day > 31)
                    {
                        number_settime_day = 1;
                    }
                }
                if (dayofmonth == 4 || dayofmonth == 6 || dayofmonth == 9 
                    || dayofmonth == 11)
                {
                    if (number_settime_day > 30)
                    {
                        number_settime_day = 1;
                    }
                }
                if (dayofmonth == 2)
                {
                    if(number_settime_day > 28)
                    {
                        number_settime_day = 1;
                    }
                }
                memset(buffer, 0, sizeof(buffer));  
                itoa(number_settime_day, buffer, 10);
                k5.setText(buffer);
            }
            else if (settime_up == 0)
            {
                number_settime_day--;
                if (dayofmonth == 1 || dayofmonth == 3 || dayofmonth == 5 
                    || dayofmonth == 7 || dayofmonth == 8 || dayofmonth == 10
                    || dayofmonth == 12 )
                {
                    if (number_settime_day < 1)
                    {
                        number_settime_day = 31;
                    }
                }
                if (dayofmonth == 4 || dayofmonth == 6 || dayofmonth == 9 
                    || dayofmonth == 11)
                {
                    if (number_settime_day < 1)
                    {
                        number_settime_day = 30;
                    }
                }
                if (dayofmonth == 2)
                {
                    if(number_settime_day < 1)
                    {
                        number_settime_day = 28;
                    }
                }
                memset(buffer, 0, sizeof(buffer));  
                itoa(number_settime_day, buffer, 10);
                k5.setText(buffer);
            }
            break;
        }
        case 4:
            if (settime_up == 1)
            {
                number_settime_hour++;
                if (number_settime_hour > 23)
                {
                    number_settime_hour = 0;
                }
                memset(buffer, 0, sizeof(buffer));  
                itoa(number_settime_hour, buffer, 10);
                k6.setText(buffer);
            }
            else if (settime_up == 0)
            {
                number_settime_hour--;
                if (number_settime_hour < 0)
                {
                    number_settime_hour = 23;
                }
                memset(buffer, 0, sizeof(buffer));  
                itoa(number_settime_hour, buffer, 10);
                k6.setText(buffer);
            }
            break;
        case 5:
            if (settime_up == 1)
            {
                number_settime_minute++;
                if (number_settime_minute > 59)
                {
                    number_settime_minute = 0;
                }
                memset(buffer, 0, sizeof(buffer));  
                itoa(number_settime_minute, buffer, 10);
                k7.setText(buffer);
            }
            else if (settime_up == 0)
            {
                number_settime_minute--;
                if (number_settime_minute < 0)
                {
                    number_settime_minute = 59;
                }
                memset(buffer, 0, sizeof(buffer));  
                itoa(number_settime_minute, buffer, 10);
                k7.setText(buffer);
            }
            break;                
        case 6:
            if (settime_up == 1)
            {
                number_settime_week++;
                if (number_settime_week > 7)
                {
                    number_settime_week = 1;
                }  
            }
            else if (settime_up == 0)
            {
                number_settime_week--;
                if (number_settime_week < 1)
                {
                    number_settime_week = 7;
                }
            }
            switch (number_settime_week)
            {
                case 1:
                        strcpy(buffer, "Sunday");
                        break;
                case 2: 
                        strcpy(buffer, "Monday");
                        break;
                case 3:
                        strcpy(buffer, "Tuesday");
                        break;
                case 4:
                        strcpy(buffer, "Wednesday");
                        break;
                case 5:
                        strcpy(buffer, "Thursday");
                        break;
                case 6:
                        strcpy(buffer, "Friday");
                        break;
                case 7:
                        strcpy(buffer, "Saturday");
                        break;
                default:
                        strcpy(buffer, "fail");
                        break;
            }
            k8.setText(buffer);
            break;
    }
}

/**
 * Button to return the response.
 * 
 * @param ptr - the parameter was transmitted to pop event function pointer. 
 * 
 */
void b0PopCallback(void *ptr)
{
    NexButton *btn = (NexButton *)ptr;
    memset(buffer, 0, sizeof(buffer));
    btn->getText(buffer, sizeof(buffer));
    if (strcmp(buffer,"ON"))
    {
        digitalWrite(9, HIGH);
        strcpy(buffer, "ON");   //cambiar el texto de la pantalla
    }
    else
    {
        digitalWrite(9, LOW);
        strcpy(buffer, "OFF");
    }
    btn->setText(buffer);
}

/**
 * Button to return the response.
 * 
 * @param ptr - the parameter was transmitted to pop event function pointer. 
 * 
 */
void b1PopCallback(void *ptr)
{
    NexButton *btn = (NexButton *)ptr;
    memset(buffer, 0, sizeof(buffer));
    btn->getText(buffer, sizeof(buffer));
    if (strcmp(buffer,"ON"))
    {
         digitalWrite(10, HIGH);
         strcpy(buffer, "ON");
    }
    else
    {
        digitalWrite(10, LOW);
        strcpy(buffer, "OFF");
    }
    btn->setText(buffer);
    
}
/**
 * Button to return the response.
 * 
 * @param ptr - the parameter was transmitted to pop event function pointer. 
 * 
 */
void b2PopCallback(void *ptr)
{
    NexButton *btn = (NexButton *)ptr;
    memset(buffer, 0, sizeof(buffer));
    btn->getText(buffer, sizeof(buffer));
    if (strcmp(buffer,"ON"))
    {
        digitalWrite(11, HIGH);
        strcpy(buffer, "ON");
     
    }
    else
    {
        digitalWrite(11, LOW);
        strcpy(buffer, "OFF");
    }
    btn->setText(buffer);
    
}
/**
 * Button to return the response.
 * 
 * @param ptr - the parameter was transmitted to pop event function pointer. 
 * 
 */
void b3PopCallback(void *ptr)
{
    NexButton *btn = (NexButton *)ptr;
    memset(buffer, 0, sizeof(buffer));
    btn->getText(buffer, sizeof(buffer));
    if (strcmp(buffer,"ON"))
    {
        digitalWrite(12, HIGH);
        strcpy(buffer, "ON");
    }
    else
    {
        digitalWrite(12, LOW);
        strcpy(buffer, "OFF");
    }
    btn->setText(buffer);
    
}

void timeDisplay()
{
    memset(buffer, 0, sizeof(buffer)); 
    number = rtc.hour();
    itoa(number, buffer, 10);
    if (strcmp(buffer_hour, buffer))
    {
        t6.setText(buffer);
        strcpy(buffer_hour, buffer); 
    }

    memset(buffer, 0, sizeof(buffer));
    memset(buffer_temp, 0, sizeof(buffer_temp));
    number = rtc.minute();
    itoa(number, buffer_temp, 10);
    if (rtc.minute() < 10)
    {
        strcat(buffer, "0");
    }
    strcat(buffer, buffer_temp);
    if (strcmp(buffer_minute, buffer))
    {
        t13.setText(buffer);
        strcpy(buffer_minute, buffer); 
    }
    
    memset(buffer, 0, sizeof(buffer)); 
    memset(buffer_temp, 0, sizeof(buffer_temp)); 
    number = rtc.second();
    itoa(number, buffer_temp, 10);
    if (number < 10)
    {
        strcpy(buffer, "0");
        strcat(buffer, buffer_temp);
        if (strcmp(buffer, buffer_second))
        {
            t9.setText(buffer);
            strcpy(buffer_second, buffer);
         }
    } 
    else
    {
        if (strcmp(buffer_temp, buffer_second))
        {
            t9.setText(buffer_temp);
            strcpy(buffer_second, buffer_temp);
        }
    }
        
 
    switch (rtc.dayOfWeek())
    {
        case 1:
                strcpy(buffer, "Sunday");
                break;
        case 2: 
                strcpy(buffer, "Monday");
                break;
        case 3:
                strcpy(buffer, "Tuesday");
                break;
        case 4:
                strcpy(buffer, "Wednesday");
                break;
        case 5:
                strcpy(buffer, "Thursday");
                break;
        case 6:
                strcpy(buffer, "Friday");
                break;
        case 7:
                strcpy(buffer, "Saturday");
                break;
        default:
                strcpy(buffer, "fail");
                break;
    }

    if (strcmp(buffer_week, buffer))
    {
        t7.setText(buffer);
        strcpy(buffer_week, buffer);
    }

    memset(buffer, 0, sizeof(buffer));
    memset(buffer_temp, 0, sizeof(buffer_temp));
    number = rtc.year();
    itoa(number, buffer, 10);
    if (strcmp(buffer_year, buffer))
    {
        t5.setText(buffer);
        strcpy(buffer_year, buffer);
    }
    
    memset(buffer, 0, sizeof(buffer));     
    number = rtc.month();
    itoa(number, buffer, 10);
    if (strcmp(buffer_month, buffer))
    {
        t16.setText(buffer);
        strcpy(buffer_month, buffer);
    }
    
    memset(buffer, 0, sizeof(buffer));
    number = rtc.day();
    itoa(number, buffer, 10);
    if (strcmp(buffer_day, buffer))
    {
        t18.setText(buffer);
        strcpy(buffer_day, buffer);
    }
}

void ntcTemperature()
{
    sensor_value = analogRead(sensor_pin);
    temp = sensor_value-185;
    temperature = 100-temp/5.4;

    memset(buffer, 0, sizeof(buffer));
    memset(buffer_temp, 0, sizeof(buffer_temp));

    itoa(temperature, buffer, 10);
    
    if (strcmp(buffer, buffer_temperature))
    {
        t8.setText(buffer);
        strcpy(buffer_temperature, buffer);
    }
    delay(100);
}

void setup(void)
{
    nexInit();  //inicializacion de la pantalla 
    b0.attachPop(b0PopCallback, &b0);  //botones que pueden cambiar el nombre
    b1.attachPop(b1PopCallback, &b1);
    b2.attachPop(b2PopCallback, &b2);
    b3.attachPop(b3PopCallback, &b3);
    b4.attachPop(b4PopCallback);      //Los botones que no pueden no se le pasa ocmo parametro el puntero
    d6.attachPop(d6PopCallback);
    d7.attachPop(d7PopCallback);
    k1.attachPop(k1PopCallback);
    k2.attachPop(k2PopCallback);
    k3.attachPop(k3PopCallback);
    k4.attachPop(k4PopCallback);
    k5.attachPop(k5PopCallback);
    k6.attachPop(k6PopCallback);
    k7.attachPop(k7PopCallback);
    k8.attachPop(k8PopCallback);
    //pines usados 
    pinMode(9, OUTPUT);
    pinMode(10, OUTPUT);
    pinMode(11, OUTPUT);
    pinMode(12, OUTPUT); 
    //inicializacion de los pines en estado bajo 
    digitalWrite(9, LOW);
    digitalWrite(10, LOW);
    digitalWrite(11, LOW);
    digitalWrite(12, LOW);
    /* RTCLib::set(byte second, byte minute, byte hour, 
     * byte dayOfWeek, byte dayOfMonth, byte month, byte year);
     */
    rtc.set(0, 5, 16, 3, 15, 2, 16); 
}

void loop(void)
{
    unsigned long nowtime = millis(); 
    unsigned long update = 0;
    if (nowtime > update)
    {
        ntcTemperature();
        update += 10000;
    }
    nexLoop(nex_Listen_List);
    rtc.refresh();
    timeDisplay();
    
}