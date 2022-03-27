#include <Arduino.h>
//mini program na odladenie tónov pasívneho bzučiaka
// odoslaním prázdnych hodnôt sa zopakuje posledne zadanie pípnutie 

//definuj piny a premenné
int piezoPin = 8; // piezo element je pripojený na pine 8
int fr = 1000; // fr = frequency - základná frekvencia - 1kHz
int fr_alt = 1000; // pomocná premenná pre frekvenciu
int dr = 1000; // dr = duration - základná dĺžka tónu - 1 sekunda
int dr_alt = 1000; // pomocná pemenná pre dĺžku
String input; // reťazec znakov z inputu serial monitora

void setup()
{
  Serial.begin(9600);
  tone(piezoPin, fr, dr); // pípni pri štarte
  // Serial print pomocou F šetri RAM ku !!
  Serial.println(F("Toto je kód na odladenie <beep> funkcie pre pasívny buzzer."));
  delay(100); // po každom serial výpise mini pauza, inak sa to zasekávalo...
} 

void read_fr()
// podprogram na načítanie frekvencie
{
  input = "";
  while (Serial.available())
  {
    fr_alt = fr;
    input = Serial.readString(); // načítaj data do reťazca
    fr = (input).toInt(); // prekonvertuj reťazec na (celé) číslo
    if (fr > 5000 || fr < 100)
    // kontrola vhodného rozsahu, nad 5000 to už nezvláda ten bzučiak...
    {
      fr = fr_alt;
      Serial.println(F("Hodnota mimo rozsah, zostáva pôvodná vhodná."));
      delay(50);
    }
    Serial.print(F("Frekvencia tónu: "));
    Serial.println(fr);
    delay(100);
    return;
  }
  delay(100);
  read_fr(); // cykluj tento podprogram kým nedostanem hodnoty
}

void read_dr()
// podprogram na načítanie dĺzky tónu, princíp ako ten pre frekvenciu
{
  input = "";
  while (Serial.available())
  {
    dr_alt = dr;
    input = Serial.readString(); 
    dr = (input).toInt();
    if (dr == 0)
    {
      dr = dr_alt;
      Serial.println(F("Nulová dĺžka, zostáva pôvodná hodnota."));
      delay(100);
    }
    Serial.print(F("Dĺžka tónu: "));
    Serial.println(dr);
    delay(100);
    return;
  }
  delay(100);
  read_dr();
}

void loop()
/*Tone needs 2 arguments, but can take three
    1) Pin#
    2) Frequency - this is in hertz (cycles per second) which determines the pitch of the noise made
    3) Duration - how long teh tone plays
  */
{
  Serial.println(F("Zadaj frekvenciu - asi od 100, do reálne tak 5000:"));
  delay(100);
  read_fr(); // vykonaj podprogram na načítanie frekvenicie
  // z toho podprogramu sa vráti až po tom ako dostane hodnotu frekvencie !!
  Serial.println(F("Zadaj trvanie tónu - v milisekundách:"));
  delay(100);
  read_dr(); // rovnako pre dĺžku tónu
  Serial.println(F("Hodnoty sú načítané, pípnem..."));
  delay(100);
  tone(piezoPin, fr, dr); // samotný tón
  //  tone(piezoPin, 100, 500);
}