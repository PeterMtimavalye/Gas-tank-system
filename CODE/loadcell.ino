#include <LiquidCrystal.h>
LiquidCrystal lcd(8,9,10,11,12,13);
 #define ledgreen 2
 #define ledred 3
#define DT A0
#define SCK A1
#define sw 1
 
long sample=0;
float val=0;
long count=0;      
 
unsigned long readCount(void)
{
unsigned long Count;
unsigned char i;
pinMode(DT, OUTPUT);
digitalWrite(DT,HIGH);
digitalWrite(SCK,LOW);
Count=0;
pinMode(DT, INPUT);
while(digitalRead(DT));
for (i=0;i<24;i++)
{
    digitalWrite(SCK,HIGH);
    Count=Count<<1;
    digitalWrite(SCK,LOW);
    if(digitalRead(DT))
    Count++;
}
    digitalWrite(SCK,HIGH);
    Count=Count^0x800000;
    digitalWrite(SCK,LOW);
    return(Count);
}

 void calibrate()
{
    lcd.clear();
    lcd.print("Calibrating...");
    lcd.setCursor(0,1);
    lcd.print("Please Wait...");
    for(int i=0;i<100;i++)
{
  count=readCount();
  sample+=count;
}
    sample/=100;
    lcd.clear();
    lcd.print("Put 15Kg & wait");
    count=0;


while(count<100)
{
    count=readCount();
    count=sample-count;
}
    lcd.clear();
    lcd.print("Please Wait....");
    delay(2000);
    for(int i=0;i<100;i++)
{
    count=readCount();
    val+=sample-count;
}
    val=val/100.0;
    val=val/15.0; // put here your calibrating weight
    lcd.clear();
}


void setup()
{
  
      pinMode(ledred,OUTPUT);
      pinMode(ledgreen,OUTPUT);
      pinMode(SCK, OUTPUT);
      pinMode(sw, INPUT_PULLUP);

        lcd.begin(16, 2);
        lcd.print(" Gas tank ");
        lcd.setCursor(0,1);
        lcd.print(" Measurement ");
        delay(1000);
        lcd.clear();
        
calibrate();

}
 
void loop()
{
      count= readCount();
      int w=(((count-sample)/val)-2*((count-sample)/val));
      lcd.setCursor(0,0);
      lcd.print("Measured Weight");
      lcd.setCursor(0,1);
      lcd.print(w);
      lcd.print("kg");
  
  
if(w>=2){
      digitalWrite(ledgreen,HIGH);
      delay(50);
      digitalWrite(ledred,LOW);
}
 else{
        digitalWrite(ledred,HIGH);
        delay(50);
        digitalWrite(ledgreen,LOW);
      
        }


if(digitalRead(sw)==0)
        {
        val=0;
        sample=0;
        w=0;
        count=0;
        calibrate();
          }
}
