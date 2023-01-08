/********************************************************************************
* serial.c: Inneh�ller funktionsdefinitioner f�r implementering av seriell
*           �verf�ring med USART.
********************************************************************************/
#include "header.h"

/********************************************************************************
* serial_init: Aktiverar seriell �verf�ring f�r transmission av data med
*              angiven baud rate (�verhastighet) i kbps (kilobits per sekund).
*              Vi st�ller in asynkron �verf�ring med �tta bitar i taget med en
*              stoppbit. Om seriell �verf�ring redan har aktiverats s� sker
*              ingen ny initiering.
*
*              1. Vi aktiverar seriell �verf�ring (skrivning) genom att ettst�lla
*                 biten TXEN0 (Transmitter Enable 0) i kontroll- och status-
*                 registret UCSR0B (USART Control and Status Register 0 B).
*
*              2. Vi st�ller in att �tta bitar skickas i taget via ettst�llning
*                 av bitar USCZ0[1:0] (USART Character Size 0 bit [1:0]) i
*                 kontroll- och statusregistret UCSR0C (USART Control and Status 
*                 Register 0 C).
*
*              3. Baud rate s�tts via skrivning till registret UBRR0 (USART Baud
*                 Rate Register 0), d�r f�ljande formel fr�n databladet anv�nds:
*
*                 UBRR0 = F_CPU / (16 * baud_rate) - 1,
*
*                 vilket avrundas till n�rmsta heltal.

*              4. Vi skriver ut ett vagnreturstecken s� att f�rsta utskriften
*                 hamnar l�ngst till v�nster p� f�rsta raden.
*
*              - baud_rate_kbps: Baud rate i kilobits per sekund.
********************************************************************************/
void serial_init(const uint32_t baud_rate_kbps)
{
   static bool serial_initialized = false;
   if (serial_initialized) return;

   UCSR0B = (1 << TXEN0);
   UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
   UBRR0 = (uint16_t)(F_CPU / (16.0 * baud_rate_kbps) - 1 + 0.5); 
   UDR0 = '\r';

   serial_initialized = true;
   return;
}

/********************************************************************************
* serial_print_string: Skriver ut angivet textstycke till en seriell terminal.
*
*                      1. Vi itererar genom str�ngen fr�n f�rsta tecknet tills
*                         ett nolltecken '\0' n�s via en pekare i.
*
*                      2. Varje tecken skickas en efter en via anrop av 
*                         funktionen serial_print_char. Det tecken som i pekar
*                         p�, dvs. dereferensen av i, skickas d�rf�r.
*
*                      3. Om ett nyradstecken '\n' skickas, s� skickar vi ett
*                         vagnreturstecken '\r' efter f�r att n�sta utskrift
*                         ska hamna l�ngst till v�nster p� n�sta rad.
*
*                      - s: Pekare till textstycken som ska skrivas ut.
********************************************************************************/
void serial_print_string(const char* s) 
{
   for (const char* i = s; *i; ++i)
   {
      serial_print_char(*i);

      if (*i == '\n')
      {
         serial_print_char('\r');
      }
   }
   return;
}

/********************************************************************************
* serial_print_integer: Skriver ut ett signerat heltal till en seriell terminal.
*
*                       1. Vi deklarerar och initierar en ny str�ng som rymmer 
*                          20 tecken. Str�ngen �r tom vid start, vi tilldelar
*                          d�rmed ett nolltecken vid start.
*
*                       2. Vi omvandlar heltalet till text och lagrar i
*                          str�ngen via anrop av funktionen sprintf fr�n
*                          stdio.h. Vi anv�nder formatspecificerare %ld, d�
*                          talet utg�r ett 32-bitars signerat heltal.
*
*                       3. Vi skriver ut talet som text via anrop av funktionen
*                          serial_print_string.
*
*                       - num: Heltalet som ska skrivas ut.
********************************************************************************/
void serial_print_integer(const int32_t num)
{
   char s[20] = { '\0' };
   sprintf(s, "%ld", num);
   serial_print_string(s);
   return;
}

/********************************************************************************
* serial_print_unsigned: Skriver ut ett osignerat heltal till en seriell terminal.
*
*                        1. Vi deklarerar och initierar en ny str�ng som rymmer
*                           20 tecken. Str�ngen �r tom vid start, vi tilldelar
*                           d�rmed ett nolltecken vid start.
*
*                        2. Vi omvandlar heltalet till text och lagrar i
*                           str�ngen via anrop av funktionen sprintf fr�n
*                           stdio.h. Vi anv�nder formatspecificerare %lu, d�
*                           talet utg�r ett 32-bitars osignerat heltal.
*
*                        3. Vi skriver ut talet som text via anrop av funktionen
*                           serial_print_string.
*
*                        - num: Heltalet som ska skrivas ut.
********************************************************************************/
void serial_print_unsigned(const uint32_t num)
{
   char s[20] = { '\0' };
   sprintf(s, "%lu", num);
   serial_print_string(s);
   return;
}

/********************************************************************************
* serial_print_double: Skriver ut ett flyttal med upp till tv� decimaler till 
*                      en seriell terminal. Eftersom formatspecificerare %lg
*                      inte fungerar i detta milj� extraheras heltalsdelen samt
*                      decimaldelen och skrivs sedan ut som signerade heltal.
*
*                      1. Vi deklarerar och intitierar en tom str�ng som rymmer
*                         40 tecken.
*
*                      2. Heltalsdelen tas ut och lagras i en konstant genom
*                         att flyttalsdelen av flyttalet trunkeras.
* 
*                      3. Decimaldelen extraheras och avrundas till tv� 
*                         decimaler. Vi tar i �tanke om heltalsdelen �r
*                         positiv eller negativ. 
*
*                         a) Om heltalsdelen �r st�rre eller lika med noll s�
*                            ber�knas decimaltalen som (flyttalet - heltalet) * 100, 
*                            exempelvis om flyttalet �r lika med 3.467:
*
*                            (3.467 - 3) * 100 = 46.7, 
*
*                            vilket avrundas till 47 genom att addera 0.5 och
*                            typomvandla till ett heltal.
*
*                         b) Annars om heltalsdelen �r mindre �n noll s� ber�knas
*                            decimaltalen som (heltalet - flyttalet) * 100,
*                            exempelvis om flyttalet �r lika med -3.467:
*
*                            (-3 - (-3.467)) * 100 = 46.7,
*
*                            vilket avrundas till 47 genom att addera 0.5 och 
*                            typomvandla till ett heltal.
*  
*                      4. Vi typomvandlar flyttalet till text och lagrar i
*                         str�ngen s. Vi anv�nder som punkt som separator
*                         mellan heltalsdelen och flyttalsdelen. Vi anropar
*                         �terigen funktionen sprintf fr�n stdio.h. Vi anv�nder 
*                         formatspecificerare %ld f�r b�de heltalsdelen samt
*                         flyttalsdelen, d� b�da delar har lagrats som 32-bitars
*                         signerade heltal. 
*
*                      5. Vi skickar iv�g textstycket via anrop av funktionen
*                         serial_print_string.
*
*                      - num: Flyttalet som ska skrivas ut.
********************************************************************************/
void serial_print_double(const double num) 
{
   char s[40] = { '\0' };
   const int32_t integer = (int32_t)num;
   int32_t decimal = 0;

   if (integer >= 0)
   {
      decimal = (int32_t)((num - integer) * 100 + 0.5);
   }
   else
   {
      decimal = (int32_t)((integer - num) * 100 + 0.5);
   }

   sprintf(s, "%ld.%ld", integer, decimal);
   serial_print_string(s);
   return;
}

/********************************************************************************
* serial_print_char: Skriver ut ett tecken till en seriell terminal. Innan
*                    angivet tecken skrivs ut sker v�ntan p� att eventuellt
*                    f�reg�ende tecken skickas.
*
*                    1. Vi l�ser av biten UDRE0 (USART Data Register Empty 0)
*                       i kontroll- och statusregistret USCR0A (USART Control
*                       and Status Register A) f�r att kontrollera ifall buffern 
*                       UDR0 (USART Data Register 0) �r tom och d�rmed redo att 
*                       ta emot n�sta tecken. S� l�nge UDRE0 �r lika med 0 har 
*                       f�reg�ende tecken inte skickats och d� v�ntar vi via 
*                       en while-sats.
*
*                    2. S� fort buffern blir tom (f�reg�ende tecken har skickats)
*                       s� ettst�lls biten UDRE0 och d� avslutas f�reg�ende
*                       while-sats. Vi l�gger d� det nya tecknet i buffern
*                       och avslutar sedan funktionen.
*
*                    - c: Tecknet som ska skrivas ut.
********************************************************************************/
void serial_print_char(const char c)
{
   while ((UCSR0A & (1 << UDRE0)) == 0);
   UDR0 = c;
   return;
}

/********************************************************************************
* serial_print_new_line: Genererar en ny rad i en seriell terminal med nyrads-
*                        tecknet \n. Ett vagnreturstecken \r skrivs ocks� ut f�r
*                        att n�sta utskrift ska hamna l�ngst till v�nster p�
*                        n�sta rad.
********************************************************************************/
void serial_print_new_line(void)
{
   serial_print_string("\n\r");
   return;
}