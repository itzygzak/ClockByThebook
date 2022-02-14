#include "Wire.h"
#include <LiquidCrystal_I2C.h> 

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Ustawienie adresu ukladu na 0x27

#define DS3231_I2C_ADDRESS 0x68

// Konwersja liczby dziesiętnej do postaci binarnej
byte decToBcd(byte val){
  return( (val/10*16) + (val%10) );
}

// Konwersja liczby binarnej do postaci dziesiętnej
byte bcdToDec(byte val){
  return( (val/16*10) + (val%16) );
}

void setup(){
  Wire.begin();
  Serial.begin(9600);
  // Ustawienie czasu w momencie inicjacji układu:
  // DS3231 seconds, minutes, hours, day, date, month, year
  // setDS3231time(00,01,11,7,11,03,17);

  lcd.begin(20,4);   
  lcd.backlight(); // załączenie podświetlenia 
  lcd.setCursor(0,0);
  lcd.print("Prosty zegar");
  delay(500);  
}

void setDS3231time(byte second, byte minute, byte hour, byte dayOfWeek, byte
dayOfMonth, byte month, byte year)
{
  // ustawienie czasu i daty w DS3231
  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(0);                    // określa, że następna wartość ma trafić do rejestru sekund
  Wire.write(decToBcd(second));     // ustawienie sekund
  Wire.write(decToBcd(minute));     // ustawienie minuty
  Wire.write(decToBcd(hour));       // ustawienie godziny
  Wire.write(decToBcd(dayOfWeek));  // ustawienie dnia tygodnia (1=niedziela, 7=sobota)
  Wire.write(decToBcd(dayOfMonth)); // ustawienie dnia (1-31)
  Wire.write(decToBcd(month));      // ustawienie miesiąca
  Wire.write(decToBcd(year));       // ustawienie roku (0-99)
  Wire.endTransmission();
}

void readDS3231time(byte *second,
byte *minute,
byte *hour,
byte *dayOfWeek,
byte *dayOfMonth,
byte *month,
byte *year)
{
  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(0); // ustawia rejestr DS3231 na 00h
  Wire.endTransmission();
  Wire.requestFrom(DS3231_I2C_ADDRESS, 7);
  // żąda 7 bajtów danych od modułu DS3231 począwszy od rejestru 00h
  *second = bcdToDec(Wire.read() & 0x7f);
  *minute = bcdToDec(Wire.read());
  *hour = bcdToDec(Wire.read() & 0x3f);
  *dayOfWeek = bcdToDec(Wire.read());
  *dayOfMonth = bcdToDec(Wire.read());
  *month = bcdToDec(Wire.read());
  *year = bcdToDec(Wire.read());
}

void displayTime(){
  byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
  // retrieve data from DS3231
  readDS3231time(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month,
  &year);
  
// Przekazanie danych do wyświetlenie w serial monitorze i do LCD

  // CZAS
  // Godziny
    // podczas wyświetlania konwertuje wartość zmiennej typu bitowego do postaci dziesiętnej
  Serial.print(hour, DEC);
  if (hour<10) {
    lcd.setCursor(0,1); 
    lcd.print("0");
    lcd.print(hour, DEC); 
  } else {
    lcd.setCursor(0,1); 
    lcd.print(hour, DEC); 
    }
  Serial.print(":");
 // lcd.setCursor(2,1); 
  lcd.print(":");
  
  // Minuty
  Serial.print(minute, DEC);
  if (minute<10) {
    Serial.print("0");
    lcd.setCursor(3,1); 
    lcd.print("0");
    lcd.print(minute, DEC); 
  } else {
    lcd.setCursor(3,1); 
    lcd.print(minute, DEC); 
    }
  Serial.print(":");
  lcd.print(":");
  
  //Sekundy
  if (second<10){
    Serial.print("0");
    lcd.setCursor(6,1); 
    lcd.print("0");
    lcd.print(second, DEC); 
  } else {
    Serial.print(second, DEC);
    lcd.setCursor(6,1); 
    lcd.print(second, DEC); 
    }

// DATA
  Serial.print(" ");
  Serial.print(dayOfMonth, DEC);
  Serial.print("/");
  Serial.print(month, DEC);
  Serial.print("/");
  Serial.print(year, DEC);
  lcd.setCursor(0,2); 
  lcd.print("0");

  // Dzień
  if (dayOfMonth<10){
    lcd.setCursor(0,2); 
    lcd.print("0");
    lcd.print(dayOfMonth, DEC); 
  } else {
    lcd.setCursor(0,2); 
    lcd.print(dayOfMonth, DEC); 
    }
    lcd.print(".");
    
  // Miesiąc
  if (month<10){
    lcd.setCursor(3,2); 
    lcd.print("0");
    lcd.print(month, DEC); 
  } else {
    lcd.setCursor(3,2); 
    lcd.print(month, DEC); 
    }
    lcd.print(".");

   //Rok
    lcd.print("20");
    if (year<10){
    lcd.setCursor(8,2); 
    lcd.print("0");
    lcd.print(year, DEC); 
  } else {
    lcd.setCursor(8,2); 
    lcd.print(year, DEC); 
    }
  
  Serial.print(" ");
  lcd.setCursor(0,3); 
  switch(dayOfWeek){
  case 1:
    Serial.println("Niedziela");
    lcd.print("Niedziela");
    break;
  case 2:
    Serial.println("Poniedzialek");
    lcd.print("Poniedzialek");
    break;
  case 3:
    Serial.println("Wtorek");
    lcd.print("Wtorek");
    break;
  case 4:
    Serial.println("Środa");
    lcd.print("Środa");
    break;
  case 5:
    Serial.println("Czwartek");
    lcd.print("Czwartek");
    break;
  case 6:
    Serial.println("Piątek");
    lcd.print("Piątek");
    break;
  case 7:
    Serial.println("Sobota");
    lcd.print("Sobota");
    break;
  }
}
void loop()
{
  displayTime(); // wyświetlanie czasu rzeczywistego
  delay(1000);   // odśwież co sekundę
}
