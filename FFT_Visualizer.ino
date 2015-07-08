#define LOG_OUT 1 // use the log output function
#define FFT_N 256 // set to 256 point fft creates fft_log_out[256]
#include <FFT.h> // include the library
#define d_start 1
#define d_end 66

void setup() {  
  Serial.begin(115200); // use the serial port
//TIMSK0 = 0; // turn off timer0 for lower jitter
  ADCSRA = 0xe5; // set the adc to free running mode
  ADMUX = 0x40; // use adc0
  DIDR0 = 0x01; // turn off the digital input for adc0
Serial.write(65); //start byte for visualizer
}

//--------------------------------------------------------------
char display[d_end-d_start]={0};
inline void FFTread(char threshold){    // reduces jitter
    cli();  // UDRE interrupt slows this way down on arduino1.0
    for (int i = 0 ; i < 512 ; i += 2) { // save 256 samples
      while(!(ADCSRA & 0x10)); // wait for adc to be ready
      ADCSRA = 0xf5; // restart adc
      byte m = ADCL; // fetch adc data
      byte j = ADCH;
      int k = (j << 8) | m; // form into an int
      k -= 0x0200; // form into a signed int
      k <<= 6; // form into a 16b signed int
      fft_input[i] = k; // put real data into even bins
      fft_input[i+1] = 0; // set odd bins to 0
    }
    fft_window(); // window the data for better frequency response
    fft_reorder(); // reorder the data before doing the fft
    fft_run(); // process the data in the fft
    fft_mag_log(); // take the output of the fft
    sei();
for(byte i=d_start;i<d_end;i++){
     if(fft_log_out[i]<threshold){display[i]=0;}
      else{display[i]=fft_log_out[i];}
  }
    
    
}
//--------------------------------------------------------------
/*

void peak(){  
byte error=0;
int D=0,Dprev=0;

  for(byte i=d_start;i<d_end;i++){
  D=fft_log_out[i+1]-fft_log_out[i];
 if(D<0&&Dprev>0){display[i-1]=fft_log_out[i-1];}
Dprev=D;
}
FFTread();


  for(byte i=d_start;i<d_end;i++){
   if((fft_log_out[i]>(display[i]*(1+(error/100))))&&(fft_log_out[i]<(display[i]*(1-(error/100))))){
display[i]=0;
}
  }

}
//--------------------------------------------------------------
//--------------------------------------------------------------
*/
void loop() {
while(1) { 
tone(11,5000);
FFTread(40);
//peak();
 for(byte i=d_start;i<d_end-5;i++){
Serial.write(display[i]);
 }
//noTone(3);tone(11,2000);
//FFTread(40);
/*
 for(byte i=d_start+3;i<d_end-2;i++){
Serial.write(display[i]);
 
 }*/
}


}
   

