
const int M1 = 7;
const int M2 = 4;
const int EN1 = 6; //pwm
const int EN2 = 5;
const int LSP = 9;
const int RSP = 8;
const int LV = 7; // sensor Volt
const int RV = 4;
const int RealyV = 13;
const int RealyG = 12;

byte state = 0;
int i = 0;
int count = 1;
int rightsensor = 0;
int leftsensor = 0;
byte place = 0;


//0 = Go straight, 1 = Left, 2 = Right, 3 = Stop, 4 = Stair Start,
int ones[3] = {1,4,3};
int scr[3] = {0,0,3};
int office[3] = {0,2,3};

void go()
{
  analogWrite(EN1,255);
  analogWrite(EN2,255);
  digitalWrite(M1, 1);
  digitalWrite(M2, 1);
}

void left()
{
  analogWrite(EN1,0);
  analogWrite(EN2,255);
  digitalWrite(M2, 1);
}

void right()
{
  analogWrite(EN1,255);
  analogWrite(EN2,0);
  digitalWrite(M1, 1);  
}

void pause()
{
  digitalWrite(EN1, LOW);
  digitalWrite(EN2, LOW); 
}

void stairstart()
{
  digitalWrite(RealyV, HIGH); 
}

void stairstop()
{
  digitalWrite(RealyV, LOW); 
}

void line()
{
  rightsensor = digitalRead(RSP);
  leftsensor = digitalRead(LSP);
   if(rightsensor == 0 && leftsensor == 0){
      while(rightsensor == 0 && leftsensor == 0 ){
          rightsensor = digitalRead(RSP);
          leftsensor = digitalRead(LSP);
          go();
        }//위 조건을 만족하는동안 켜야하므로 while문을 사용
    }else if(rightsensor == 1 && leftsensor == 0){
        while(rightsensor == 1 && leftsensor == 0 ){
            rightsensor = digitalRead(RSP);
            leftsensor = digitalRead(LSP);
            left();
          }
  }else if(rightsensor == 0 && leftsensor == 1){
        while(rightsensor == 0 && leftsensor == 1 ){
            rightsensor = digitalRead(RSP);
            leftsensor = digitalRead(LSP);
            right();
        }
    } 
}

void checkpoint(int maps[]){
  if(rightsensor == 1 && leftsensor == 1){
  switch(maps[i]){
   case 0: 
   while(rightsensor == 1 && leftsensor == 1 ){
          rightsensor = digitalRead(RSP);
          leftsensor = digitalRead(LSP);
          go();
        }
   i++;
   return;
   case 1: 
    while(rightsensor == 1 && leftsensor == 1 ){
          rightsensor = digitalRead(RSP);
          leftsensor = digitalRead(LSP);
          left();
          //delay(1000);
         }
    while(leftsensor == 0)
    {
          rightsensor = digitalRead(RSP);
          leftsensor = digitalRead(LSP);
          left();
    }
    while(leftsensor == 0)
    {
         rightsensor = digitalRead(RSP);
         leftsensor = digitalRead(LSP);
         left();
         delay(500);
    }
   i++;

   return; 
   case 2: 
   while(rightsensor == 1 && leftsensor == 1 ){
          rightsensor = digitalRead(RSP);
          leftsensor = digitalRead(LSP);
          right();
        }
   while(rightsensor == 0 ){
          rightsensor = digitalRead(RSP);
          leftsensor = digitalRead(LSP);
          right();
        }
   while(rightsensor == 0 ){
          rightsensor = digitalRead(RSP);
          leftsensor = digitalRead(LSP);
          right();
          delay(500);
        }
   i++;
   return;
   
   case 3: pause();
     i = 3;
     while(1);

   case 4: 
   while(1){
   go();
   stairstart();
   delay(500);
   rightsensor = digitalRead(RSP);
   leftsensor = digitalRead(LSP);
   if(rightsensor == 1 && leftsensor == 1){ stairstop; break;}
   }
     i++;
   return;
   
   default: return;
   
  }
  
 }
 return;
}
 
void controlbl()
{
    byte data;
    if(Serial.available() > 0){
    data = Serial.read();
    switch(data){
    case 'g' :
          digitalWrite(EN1, HIGH);
          digitalWrite(EN2, HIGH);
          digitalWrite(M1, HIGH);
          digitalWrite(M2, HIGH);
          return;
      
    case 'r' :
          digitalWrite(EN1, HIGH);
          digitalWrite(EN2, LOW);
          digitalWrite(M1, HIGH);
          digitalWrite(M2, LOW);
          return;
          
    case 's' :
          digitalWrite(EN1, LOW);
          digitalWrite(EN2, LOW); 
          return;
      
   case 'b' :
          digitalWrite(EN1, HIGH);
          digitalWrite(EN2, HIGH);
          digitalWrite(M1,LOW);
          digitalWrite(M2,LOW);
          return;
      
    case 'l':
          digitalWrite(EN1, LOW);
          digitalWrite(EN2, HIGH);
          digitalWrite(M1,LOW);
          digitalWrite(M2,HIGH);
          return;
      
  default : return;
  }
 }
}

void setup() {
  pinMode(M1,OUTPUT);
  pinMode(M2,OUTPUT);
  pinMode(LSP,INPUT);
  pinMode(RSP,INPUT);
  pinMode(13, OUTPUT);
  analogWrite(EN1,0);
  analogWrite(EN2,0);
  digitalWrite(LV, HIGH);
  digitalWrite(RV, HIGH);
  //digitalWrite(RealyG, LOW);
  Serial.begin(9600);
 
  // put your setup code here, to run once:

}
void set()
{
  while(Serial.available() == 0);
  state = Serial.read(); 
}
void loop() {
  if(count == 1)
  {
  set();
  count++;
  }
  if(state == 'z'){
    line();
    checkpoint(scr);
  }
    if(state == 'x'){
    line();
    checkpoint(ones);
  }
    if(state == 'c'){
    line();
    checkpoint(office);
  }
    if(state == 'v'){
    controlbl();
  }
}
