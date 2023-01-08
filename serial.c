/********************************************************************************
* serial.c: Innehåller funktionsdefinitioner för implementering av seriell
*           överföring med USART.
********************************************************************************/
#include "header.h"

/********************************************************************************
* serial_init: Aktiverar seriell överföring för transmission av data med
*              angiven baud rate (överhastighet) i kbps (kilobits per sekund).
*              Vi ställer in asynkron överföring med åtta bitar i taget med en
*              stoppbit. Om seriell överföring redan har aktiverats så sker
*              ingen ny initiering.
*
*              1. Vi aktiverar seriell överföring (skrivning) genom att ettställa
*                 biten TXEN0 (Transmitter Enable 0) i kontroll- och status-
*                 registret UCSR0B (USART Control and Status Register 0 B).
*
*              2. Vi ställer in att åtta bitar skickas i taget via ettställning
*                 av bitar USCZ0[1:0] (USART Character Size 0 bit [1:0]) i
*                 kontroll- och statusregistret UCSR0C (USART Control and Status 
*                 Register 0 C).
*
*              3. Baud rate sätts via skrivning till registret UBRR0 (USART Baud
*                 Rate Register 0), där följande formel från databladet används:
*
*                 UBRR0 = F_CPU / (16 * baud_rate) - 1,
*
*                 vilket avrundas till närmsta heltal.

*              4. Vi skriver ut ett vagnreturstecken så att första utskriften
*                 hamnar längst till vänster på första raden.
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
*                      1. Vi itererar genom strängen från första tecknet tills
*                         ett nolltecken '\0' nås via en pekare i.
*
*                      2. Varje tecken skickas en efter en via anrop av 
*                         funktionen serial_print_char. Det tecken som i pekar
*                         på, dvs. dereferensen av i, skickas därför.
*
*                      3. Om ett nyradstecken '\n' skickas, så skickar vi ett
*                         vagnreturstecken '\r' efter för att nästa utskrift
*                         ska hamna längst till vänster på nästa rad.
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
*                       1. Vi deklarerar och initierar en ny sträng som rymmer 
*                          20 tecken. Strängen är tom vid start, vi tilldelar
*                          därmed ett nolltecken vid start.
*
*                       2. Vi omvandlar heltalet till text och lagrar i
*                          strängen via anrop av funktionen sprintf från
*                          stdio.h. Vi använder formatspecificerare %ld, då
*                          talet utgör ett 32-bitars signerat heltal.
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
*                        1. Vi deklarerar och initierar en ny sträng som rymmer
*                           20 tecken. Strängen är tom vid start, vi tilldelar
*                           därmed ett nolltecken vid start.
*
*                        2. Vi omvandlar heltalet till text och lagrar i
*                           strängen via anrop av funktionen sprintf från
*                           stdio.h. Vi använder formatspecificerare %lu, då
*                           talet utgör ett 32-bitars osignerat heltal.
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
* serial_print_double: Skriver ut ett flyttal med upp till två decimaler till 
*                      en seriell terminal. Eftersom formatspecificerare %lg
*                      inte fungerar i detta miljö extraheras heltalsdelen samt
*                      decimaldelen och skrivs sedan ut som signerade heltal.
*
*                      1. Vi deklarerar och intitierar en tom sträng som rymmer
*                         40 tecken.
*
*                      2. Heltalsdelen tas ut och lagras i en konstant genom
*                         att flyttalsdelen av flyttalet trunkeras.
* 
*                      3. Decimaldelen extraheras och avrundas till två 
*                         decimaler. Vi tar i åtanke om heltalsdelen är
*                         positiv eller negativ. 
*
*                         a) Om heltalsdelen är större eller lika med noll så
*                            beräknas decimaltalen som (flyttalet - heltalet) * 100, 
*                            exempelvis om flyttalet är lika med 3.467:
*
*                            (3.467 - 3) * 100 = 46.7, 
*
*                            vilket avrundas till 47 genom att addera 0.5 och
*                            typomvandla till ett heltal.
*
*                         b) Annars om heltalsdelen är mindre än noll så beräknas
*                            decimaltalen som (heltalet - flyttalet) * 100,
*                            exempelvis om flyttalet är lika med -3.467:
*
*                            (-3 - (-3.467)) * 100 = 46.7,
*
*                            vilket avrundas till 47 genom att addera 0.5 och 
*                            typomvandla till ett heltal.
*  
*                      4. Vi typomvandlar flyttalet till text och lagrar i
*                         strängen s. Vi använder som punkt som separator
*                         mellan heltalsdelen och flyttalsdelen. Vi anropar
*                         återigen funktionen sprintf från stdio.h. Vi använder 
*                         formatspecificerare %ld för både heltalsdelen samt
*                         flyttalsdelen, då båda delar har lagrats som 32-bitars
*                         signerade heltal. 
*
*                      5. Vi skickar iväg textstycket via anrop av funktionen
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
*                    angivet tecken skrivs ut sker väntan på att eventuellt
*                    föregående tecken skickas.
*
*                    1. Vi läser av biten UDRE0 (USART Data Register Empty 0)
*                       i kontroll- och statusregistret USCR0A (USART Control
*                       and Status Register A) för att kontrollera ifall buffern 
*                       UDR0 (USART Data Register 0) är tom och därmed redo att 
*                       ta emot nästa tecken. Så länge UDRE0 är lika med 0 har 
*                       föregående tecken inte skickats och då väntar vi via 
*                       en while-sats.
*
*                    2. Så fort buffern blir tom (föregående tecken har skickats)
*                       så ettställs biten UDRE0 och då avslutas föregående
*                       while-sats. Vi lägger då det nya tecknet i buffern
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
*                        tecknet \n. Ett vagnreturstecken \r skrivs också ut för
*                        att nästa utskrift ska hamna längst till vänster på
*                        nästa rad.
********************************************************************************/
void serial_print_new_line(void)
{
   serial_print_string("\n\r");
   return;
}