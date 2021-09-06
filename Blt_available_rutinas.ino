#include <SPI.h>

#include "I2Cdev.h"
#include "MPU6050.h"
#include "Wire.h"

#include <SoftwareSerial.h> // libreria que permite establecer pines digitales
        // para comunicacion serie

const int MPU2=0x69,MPU1=0x68;

MPU6050 sensor;

SoftwareSerial miBT(12, 13);  // pin 12 como RX, pin 13 como TX

int DATO=0;
int m1s=23;
int m1b=25;
int m2s=27;
int m2b=29;
int m3s=31;
int m3b=33;
int m4s=35;
int m4b=37;
int m5s=39;
int m5b=41;
int m6s=43;
int m6b=45;


int ax[4], ay[4], az[4];

int16_t ax2[4],ay2[4],az2[4];

float  accel_ang_x2[4], accel_ang_y2[4];
int s1=8, s2=9, s3=10, s4=11, i=0 ;
float dif13, dif14, dif23, dif24, dif12;
bool m1h=false,m2h=false,m3h=false,m4h=false,H=false;
bool b1s=false,b1b=false,b2s=false,b2b=false;
bool b3s=false,b3b=false,b4s=false,b4b=false;
bool b5s=false,b5b=false,b6s=false,b6b=false, H1=false,R1=false,R2=false;

bool r1s=false,r1b=false,r2s=false,r2b=false;
bool r3s=false,r3b=false,r4s=false,r4b=false;
bool r5s=false,r5b=false,r6s=false,r6b=false;

long treal,tdif1,tdif2,tdif3,tdif5s=0,tdif5b=0,tdif6s=0,tdif6b=0;
long cont5=0,cont6=0;


String cadenaCharEntrada = "";  
String cadenaCharEntrada1 = ""; 
char vectorChar[20];
bool finCadena = false;


void setup(){
   miBT.begin(38400);    // comunicacion serie entre Arduino y el modulo a 38400 bps
   
  pinMode(m1s,OUTPUT);
  pinMode(m1b,OUTPUT);
  pinMode(m2s,OUTPUT);
  pinMode(m2b,OUTPUT);
  pinMode(m3s,OUTPUT);
  pinMode(m3b,OUTPUT);
  pinMode(m4s,OUTPUT);
  pinMode(m4b,OUTPUT);
  pinMode(m5s,OUTPUT);
  pinMode(m5b,OUTPUT);
  pinMode(m6s,OUTPUT);
  pinMode(m6b,OUTPUT);

      Wire.begin(); 
      Wire.beginTransmission(MPU1);
      Wire.write(0x6B);// PWR_MGMT_1 register 
      Wire.write(0); // set to zero (wakes up the MPU-6050)
      Wire.endTransmission(true);
     
      Wire.begin(); 
      Wire.beginTransmission(MPU2);
      Wire.write(0x6B);// PWR_MGMT_1 register 
      Wire.write(0); // set to zero (wakes up the MPU-6050)
      Wire.endTransmission(true);
      
      Wire.begin(); 
      
      Serial.begin(9600); 
      sensor.initialize();
      
      pinMode(s1,OUTPUT);
      pinMode(s2,OUTPUT);
      pinMode(s3,OUTPUT);
      pinMode(s4,OUTPUT);

    cadenaCharEntrada.reserve(20);
    
}


void loop(){
  
  miBT.flush();

    while(finCadena == false){

    
    //Serial.print("  SRAM Libre: "); 
    //Serial.println(freeRam());
    
       
    sensarangulos();

    rutinas();

    restricciones();
    
    ejecutar();
    
    sensar5y6();

    //leer dato

      if(miBT.available()) {              //Si existen datos seriales, leer a todos
                        //Serial.println("Comienza lectura");
                        char CaracterEntrada = miBT.read();   //Leer 1 byte serial recibido
                        //Serial.println("Se lee dato");
                        //Serial.println(CaracterEntrada);
                        cadenaCharEntrada += CaracterEntrada;   //Agregar el nuevo char a una cadena String 
                        if (CaracterEntrada == ' ') {          //Si el char o byte recibido es un fin de linea, activa la bandera
                        cadenaCharEntrada1=cadenaCharEntrada;
                        cadenaCharEntrada="";
                        // Serial.println(cadenaCharEntrada1);
                        finCadena = true;   //Si la bandera finCadena = 1, entonces la transmision esta completa
                        //Serial.println("Se leyo la cadena de caracteres");
                                                     }
                             }
         
    
    }

     finCadena = false;                                  //Permitimos volver a recibir otra cadena
    cadenaCharEntrada1.toCharArray(vectorChar,20);   //Convertimos el string a un vectorChar
    int numeroEntero = atoi(vectorChar);
    //Serial.print(numeroEntero);//Convertimos un vectorChar a un entero
    //Serial.print("Se recibio ");
    //Serial.println(numeroEntero);
    cadenaCharEntrada = "";
    DATO=numeroEntero;
    
    asignarbanderas();
}





void asignarbanderas(){

  if (DATO==1)
  b1s=true;
  
   if(DATO==2)
  b1s=false;

   if (DATO==3)
  b1b=true;
  
   if(DATO==4)
  b1b=false;
   
 if (DATO==5)
  b2s=true;
  
  if(DATO==6)
  b2s=false;
   
  if (DATO==7)
  b2b=true;
  
   if(DATO==8)
  b2b=false;

  if (DATO==9)
  b3s=true;
  
   if(DATO==10)
  b3s=false;   
  
  if (DATO==11)
  b3b=true;
  
   if(DATO==12)
  b3b=false; 
  
  if (DATO==13)
  b4s=true;
  
   if(DATO==14)
  b4s=false; 
  
  if (DATO==15)
  b4b=true;
  
   if(DATO==16)
  b4b=false; 
  
  if (DATO==17)
  b5s=true;
  
   if(DATO==18)
  b5s=false; 
   
  if (DATO==19)
  b5b=true;
  
   if(DATO==20)
  b5b=false; 
   
  if (DATO==21)
  b6s=true;
  
   if(DATO==22)
  b6s=false;

  if (DATO==23)
  b6b=true;
  
   if(DATO==24)
  b6b=false;

   if(DATO==25){
  b1s=true;
  b2s=true;
   }
   
   if(DATO==26){
  b1s=false;
  b2s=false;
   }
    
    if(DATO==27){
  b1b=true;
  b2b=true;
   }

   if(DATO==28){
  b1b=false;
  b2b=false;
   }
   
    if(DATO==29){
  b3s=true;
  b4s=true;
   }

   if(DATO==30){
  b3s=false;
  b4s=false;
   }

    if(DATO==31){
  b3b=true;
  b4b=true;
   }

   if(DATO==32){
  b3b=false;
  b4b=false;
   }

    if(DATO==33){
  b5s=true;
  b6s=true;
   }

   if(DATO==34){
  b5s=false;
  b6s=false;
   }

    if(DATO==35){
  b5b=true;
  b6b=true;
   }

   if(DATO==36){
  b5b=false;
  b6b=false;
   }

    if(DATO==37){
  b1s=true;
  b3s=true;
   }

   if(DATO==38){
  b1s=false;
  b3s=false;
 
   }
   
   if(DATO==39){
  b1b=true;
  b3b=true;
  
   }

   if(DATO==40){
  b1b=false;
  b3b=false;
   }

  if(DATO==41){
  b2s=true;
  b4s=true;
     }
   
   if(DATO==42){
  b2s=false;
  b4s=false;
   }

   if(DATO==43){
   b2b=true;
   b4b=true;
  
   }
   
   if(DATO==44){
   b2b=false;
   b4b=false;
   }


   if(DATO==45){
  b1s=true;
  b2s=true;
  b3s=true;
  b4s=true;
  b5s=true;
  b6s=true;
   }

     if(DATO==46){
  b1s=false;
  b2s=false;
  b3s=false;
  b4s=false;
  b5s=false;
  b6s=false;
   }

   if(DATO==47){
  b1b=true;
  b2b=true;
  b3b=true;
  b4b=true;
  b5b=true;
  b6b=true;
   }

     if(DATO==48){
  b1b=false;
  b2b=false;
  b3b=false;
  b4b=false;
  b5b=false;
  b6b=false;
   } 

   if(DATO==49){
  //Rutina 1
  R1=true;
  tdif2=millis();
  r1b=true;
  r2b=true;
  r3b=true;
  r4b=true;
 
   }

   if(DATO==50){
  //Rutina 1
   } 

   if(DATO==51){
  //Rutina 2
  R2=true;
  tdif3=millis();
   }

   if(DATO==52){
  //Rutina 2
   } 

   //HOME
   if(DATO==53){
    H=true;
    tdif1=millis();
    r1b=true;
    r2b=true;
    r3b=true;
    r4b=true;
    r5b=true;
    r6b=true;
    H1=true;
   } 
  //HOME
   if(DATO==54){
   
   } 

  if(DATO==55){
  b1s=false;
  b2s=false;
  b3s=false;
  b4s=false;
  b5s=false;
  b6s=false;
  b1b=false;
  b2b=false;
  b3b=false;
  b4b=false;
  b5b=false;
  b6b=false;
  H=false;
  R1=false;
  R2=false;
  
   } 

   if(DATO==56){

   } 




  
}

void restricciones(){


      dif13=abs(accel_ang_x2[0])+abs(accel_ang_x2[2]);
      dif14=abs(accel_ang_x2[0])+abs(accel_ang_x2[3]);
      dif23=abs(accel_ang_x2[1])+abs(accel_ang_x2[2]);
      dif24=abs(accel_ang_x2[1])+abs(accel_ang_x2[3]);
      dif12=abs(accel_ang_x2[0])+abs(accel_ang_x2[1]);

        
//Restriccciones por si motor delante y atras
   
   if ( b1s&&b1b==true){  
      b1s=false;
      b1b=false;
    }
    if ( b2s&&b2b==true){  
      b2s=false;
      b2b=false;
    }
     if ( b3s&&b3b==true){  
      b3s=false;
      b3b=false;
    }
     if ( b4s&&b4b==true){  
      b4s=false;
      b4b=false;
    }
     if ( b5s&&b5b==true){  
      b5s=false;
      b5b=false;
    }
     if ( b6s&&b6b==true){  
      b6s=false;
      b6b=false;
    }

    //Restricciones respaldo mayor o igual a 55
    if(abs(accel_ang_x2[0])>48){
      b1s=false;
    }
    if(abs(accel_ang_x2[1])>50.3){
      b2s=false;
    }
    
    //Restricciones respaldo
   /* if((abs(accel_ang_x2[0])-abs(accel_ang_x2[1]))>=3){
      b1s=false;
      b2b=false;
    }

     if((abs(accel_ang_x2[1])-abs(accel_ang_x2[0]))>=8){
      b2s=false;
      b1b=false;
    }*/

    //Restricciones sandwich
    if(dif13>=95){
      b1s=false;
      b3s=false;
    }
    if(dif14>=95){
      b1s=false;
      b4s=false;
    }
    if(dif23>=96){
      b2s=false;
      b3s=false;
    }
    if(dif24>=96){
      b2s=false;
      b4s=false;
    }  
}


void sensar5y6(){
  
  //sensar 5 y 6
    if(b5s==true){
   cont5=cont5+(millis()-tdif5s);  
   tdif5s=millis();
    }
    if(b5b==true){
   cont5=cont5-(millis()-tdif5b);  
   tdif5b=millis();
    }
    if(b6s==true){
   cont6=cont6+(millis()-tdif6s);  
   tdif6s=millis();
    }
     if(b6b==true){
   cont6=cont6-(millis()-tdif6b);  
   tdif6b=millis();
    }

    
      Serial.println("5");
      Serial.println(cont5);
      Serial.println("6");
      Serial.println(cont6);
      
}


void sensarangulos(){
  if(i==4){ //reiniciar contador
       i=0;
  }
      if(i==0){
      digitalWrite(s1,true);
      digitalWrite(s2,false);
      digitalWrite(s3,false);
      digitalWrite(s4,false);
      }
      if(i==1){
      digitalWrite(s1,false); 
      digitalWrite(s2,true);
      digitalWrite(s3,false);
      digitalWrite(s4,false);
      }
      if(i==2){
      digitalWrite(s1,false);
      digitalWrite(s2,false);
      digitalWrite(s3,true);
      digitalWrite(s4,false);
      }
      if(i==3){
      digitalWrite(s1,false);
      digitalWrite(s2,false);
      digitalWrite(s3,false);
      digitalWrite(s4,true);
      }
       
       //get values for second mpu having address of 0x69
       GetMpuValue2(MPU2);

      Serial.println(i);
      Serial.println(accel_ang_x2[i]);
       
       i++;
}


void rutinas(){
  //Rutina H
  if(H==true){
   HOME();
  }

//Rutina 1
 if(R1==true){
 Rutina1();
 }
//Rutina 2
 if(R2==true){
  Rutina2();
 }
}


void GetMpuValue2(const int MPU){ 

//if (Wire.available())    //Esperando a que termine
//{



   
      Wire.beginTransmission(MPU); 

      Serial.println("empieza transmision");
      
      Wire.write(0x3B); // starting with register 0x3B (ACCEL_XOUT_H) 

      Serial.println("wire write");
      Serial.println("wire write");
      
      Wire.endTransmission();

      Serial.println("end transmission");
      
      Wire.requestFrom(MPU, 14, true); // request a total of 14 registers 

      Serial.println("14 regsitros");
      
      ax2[i]=Wire.read()<<8| Wire.read(); // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L) 

       Serial.println("read ax");
      
      ay2[i]=Wire.read()<<8|  Wire.read(); // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)

      Serial.println("read ay");
      
      az2[i]=Wire.read()<<8| Wire.read(); // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)

       Serial.println("read az");

      accel_ang_x2[i]=atan(ay2[i]/sqrt(pow(ax2[i],2) + pow(az2[i],2)))*(180.0/3.14);
       Serial.println("formula mamalona");
//}

     }

 void ejecutar(){

  if(b1s==true){
    digitalWrite(m1s,true);  
  }else{
    digitalWrite(m1s,false);
    
  }
  
  if(b1b==true){
    digitalWrite(m1b,true);  
  }else{
    digitalWrite(m1b,false);
    
  }
  
  if(b2s==true){
    digitalWrite(m2s,true); 
  }else{
    digitalWrite(m2s,false);
    
  }
  
  if(b2b==true){
    digitalWrite(m2b,true); 
  }else{
    digitalWrite(m2b,false);
    
  }

   if(b3s==true){
    digitalWrite(m3s,true);  
  }else{
    digitalWrite(m3s,false);
   
  }
  
  if(b3b==true){
    digitalWrite(m3b,true); 
  }else{
    digitalWrite(m3b,false);
    
  }

   if(b4s==true){
    digitalWrite(m4s,true);
  }else{
    digitalWrite(m4s,false);
    
  }
  
  if(b4b==true){
    digitalWrite(m4b,true); 

  }else{
    digitalWrite(m4b,false);
    
  }

   if(b5s==true){
    digitalWrite(m5s,true);  
  }else{
    digitalWrite(m5s,false);
  }
  
  if(b5b==true){
    digitalWrite(m5b,true); 
  }else{
    digitalWrite(m5b,false);
  }

   if(b6s==true){
    digitalWrite(m6s,true);  
  }else{
    digitalWrite(m6s,false);
  }
  
  if(b6b==true){
    digitalWrite(m6b,true); 
  }else{
    digitalWrite(m6b,false);
  }

   if(b5s==true) 
   tdif5s=millis();

   if(b5b==true) 
   tdif5b=millis();

   if(b6s==true) 
   tdif6s=millis();

   if(b6b==true) 
   tdif6b=millis();
   
 }





void HOME(){

   
    if(abs(accel_ang_x2[0])>=4){
      b1b=true;
    }else{
      b1b=false;
      
    }
    
    if(abs(accel_ang_x2[1])>=4){
      b2b=true;
      
    }else{
      digitalWrite(m2b,false);
      b2b=false;
    }
    
    if(accel_ang_x2[2]>=4){
     b3b=true;
      
    }else{
      b3b=false;
    }
    if(accel_ang_x2[3]>=4){
      b4b=true;     
    }else{
      b4b=false;
    }
  

  if((millis()-tdif1)<=52500){
    
    if(millis()-tdif1<=49000){
      b5b=true;
      b6b=true;
    }else{
      b5b=false;
      b6b=false;
    }
    
    if(((millis()-tdif1)>49000)&&((millis()-tdif1)<=52000)){
      b6s=true;    
    }else{
      b6s=false;
    }
    
  }
    
    else{
     b6s=false;
      H=false; 
      cont5=0;
      cont6=0; 
    }
}


void HOME1(){

if(r1b||r2b||r3b||r4b||r5b||r6b==true){   
    
    if(abs(accel_ang_x2[0])>=4){
      b1b=true;
    }else{
      b1b=false;
      r1b=false;
    }
    
    if(abs(accel_ang_x2[1])>4){
      b2b=true;
    }else{
      digitalWrite(m2b,false);
      b2b=false;
      r2b=false;
    }
    
    if(accel_ang_x2[2]>4){
     b3b=true;
    }else{
      b3b=false;
      r3b=false;
    }
    if(accel_ang_x2[3]>4){
      b4b=true;     
    }else{
      b4b=false;
      r4b=false;
    }
  

  if(cont5>3000){
      b5b=true;
      } else if(cont5<2000){
      b5b=false;
      b5s=true;    
    }else{
     b5s=false; 
      cont5=2000;
      r5b=false;
    }


if(cont6>3000){
      b6b=true;
      } else if(cont6<2000){
      b6b=false;
      b6s=true;    
    }else{
     b6s=false; 
     cont6=2000;
     r6b=false;
    }
}
else{
  H1=false;
}
}


void Rutina1(){

if(r1b||r2b||r3b==true){   

if(r1b||r2b==true){
    if(abs(accel_ang_x2[0])>38){
      b1b=true;
    }else if(abs(accel_ang_x2[0])<31){
      b1b=false;
      b1s=true;
    }else{
      b1s=false;
      b1b=false;
      r1b=false;
    }

    if(abs(accel_ang_x2[1])>37){
      b2b=true;
    }else if(abs(accel_ang_x2[1])<32){
      b2b=false;
      b2s=true;
    }else{
      b2s=false;
      b2b=false;
      r2b=false;
    }
}


  if ((millis()-tdif2)<41000){

 if (((millis()-tdif2)>5000)&&((millis()-tdif2)<10000)){
    b3s=true;
 }else{
    b3s=false;
 }
 if (((millis()-tdif2)>15000)&&((millis()-tdif2)<20000)){
    b3b=true;
 }else{
    b3b=false;
 }

  if (((millis()-tdif2)>25000)&&((millis()-tdif2)<30000)){
    b4s=true;
 }else{
    b4s=false;
 }
 if (((millis()-tdif2)>35000)&&((millis()-tdif2)<40000)){
    b4b=true;
 }else{
    b4b=false;
 }
     
  }else{
     r3b=false;
  }

  }
  else{
    R1=false;
  }
   
}
  


void Rutina2(){

if(r1b||r2b||r3b||r4b==true){   


    if(abs(accel_ang_x2[0])>38){
      b1b=true;
    }else if(abs(accel_ang_x2[0])<33){
      b1b=false;
      b1s=true;
    }else{
      b1s=false;
      r1b=false;
    }

    if(abs(accel_ang_x2[1])>39){
      b2b=true;
    }else if(abs(accel_ang_x2[1])<34){
      b2b=false;
      b2s=true;
    }else{
      b2s=false;
      r2b=false;
    }



  if (((millis()-tdif2)>2000)&&((millis()-tdif2)<=4000)){
     b4s=true;
  }else{
     b4s=false;
     r3b=false;
  }

  if (((millis()-tdif2)>5000)&&((millis()-tdif2)<=6000)){
     b4b=true;
  }else{
     b4b=false;
     r4b=false;
  }
   
}else{
  R2=false;
}
  
}
