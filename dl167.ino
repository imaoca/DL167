#include "LedControl.h"
LedControl lc=LedControl(19,17,18,1);
unsigned char mem[255],inst,c_flag,dip,cnt,stat,op2;
int reg[8];
//unsigned char pin[]={12,11,2,6,8,14,15,16,17,18,19};
unsigned char pin[]={2,3,4,5,6,7,8,9,10,11,12};
//unsigned char prg[16]={0xa0,0x55,0x61,0x90,0x02,0,0,0,0,0,0,0,0,0,0,0};
unsigned char prg[16]={0xa0,0xf0,0x60,0x80,0x02,0x90,5,0,0,0,0,0,0,0,0,0};
void setup() {
  lc.shutdown(0,false);lc.setIntensity(0,8);lc.clearDisplay(0);// led diplay setup
  for (int i=0;i<11;i++) pinMode(pin[i],INPUT_PULLUP);
  pinMode(13, OUTPUT);      // clock blink
  pinMode(14, OUTPUT);      // output
  (reg[0],reg[1],reg[2],reg[3],reg[4],reg[5],reg[6],reg[7],c_flag,op2)=0;
  if (digitalRead(11))for (int i=0;i<16;i++) mem[i]=prg[i];
}
void cpu(){
  if ((cnt++)>60) {
    digitalWrite(13, !digitalRead(13));cnt=0;
    digitalWrite(14,reg[6]&1);
    reg[5]=dip &0x0f;
    
    inst = mem[reg[7]];
    if (op2==0){
/*MOV*/    if (0x00==(inst&0xc0)) reg[(inst&0x38)>>3]=reg[inst&7]; 
/*add*/    if (0x40==(inst&0xf8)) if ((reg[0]+=reg[inst&7])>255) c_flag=1;
/*or*/     if (0x48==(inst&0xf8)) reg[0]|=reg[inst&7];
/*and*/    if (0x50==(inst&0xf8)) reg[0]&=reg[inst&7];
/*xor*/    if (0x58==(inst&0xf8)) reg[0]=reg[0]^reg[inst&7];
/*inc*/    if (0x60==(inst&0xf8)) if ((++reg[inst&7])>255) c_flag=1;
/*not*/    if (0x68==(inst&0xf8)) reg[inst&7]=!reg[inst&7];
/*lrotate*/if (0x70==(inst&0xf8)) reg[inst&7]=((reg[inst&7])>>1)|((reg[inst&7]&0x01) << 3);
/*lrotate*/if (0x78==(inst&0xf8)) reg[inst&7]=((reg[inst&7])<<1)|((reg[inst&7]&0x08) >> 3);
/*jnc      if (0x80==(inst&0xf0)) {reg[7]=(c_flag)?reg[7]+1:inst&0x0f;c_flag=0;} */
/*jmp      if (0x90==(inst&0xf0)) reg[7]=inst&0x0f; */
/*mvi      if (0xa0==(inst&0xf0)) reg[0]=inst&0x0f; */
/*jnc*/      if (0x80==(inst&0xf0)) op2=1;
/*jmp*/      if (0x90==(inst&0xf0)) op2=2;
/*mvi*/      if (0xa0==(inst&0xf0)) op2=3; 
//    if ((0x90!=(inst&0xf0)&&(0x80!=(inst&0xf0)))) reg[7]++;
            reg[7]++;
      }
    else {
      if (op2==1) {reg[7]=(c_flag)?reg[7]+1:inst;c_flag=0;}
      if (op2==2) reg[7]=inst;
      if (op2==3) {reg[0]=inst;reg[7]++;}
      op2=0;
      }  
    }
  }
void loop() {
  dip=!digitalRead(4)+(!digitalRead(5))*2+(!digitalRead(6))*4+(!digitalRead(7))*8
  +(!digitalRead(8))*16+(!digitalRead(9))*32+(!digitalRead(10))*64+(!digitalRead(11))*128;
  if ((digitalRead(12)==0) || ((stat&0x80) == 128)){ // run mode
    stat = dip;
    cpu();  
    for (int i=0;i<8;i++)lc.setColumn(0,i,reg[i]);
    lc.setColumn(0,5,(reg[5]|(c_flag<<7)));  
  }
  else {                  // programing mode 
    for (int i=0;i<8;i++) lc.setColumn(0,i,(mem[i+(8*(reg[7]>7))])^dip*(i==(reg[7]%8)));delay(100);  
    for (int i=0;i<8;i++) lc.setColumn(0,i,(mem[i+(8*(reg[7]>7))]));delay(100);  
    if (!digitalRead(2)){reg[7]=(reg[7]<15)?reg[7]+1:0;delay(200);}
    if (!digitalRead(3)){mem[reg[7]]=dip;delay(1000);}
  }
}
