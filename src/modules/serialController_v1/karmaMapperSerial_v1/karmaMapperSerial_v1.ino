// Made by Daan de Lange on 09-06-2016

// Serial communication
#include <PacketSerial.h>
PacketSerial serial;

// LEDs (PWM)
int leds1Pin = 9;
int leds2Pin = 10;
int leds3Pin = 11;
int leds1Brightness = 0;
int leds2Brightness = 0;
int leds3Brightness = 0;

// LED STRIPS
int ledStrip1Pin = 5;
int ledStrip2Pin = 6;
int ledStrip1PotPin = A0;
int ledStrip2PotPin = A1;
int ledStrip1Intensity;
int ledStrip2Intensity;
bool ledStrip1IsOscControlled;
bool ledStrip2IsOscControlled;

// Flow Meter
int flowMeter1Pin = 2;
int flowMeter2Pin = 3;
// The hall-effect flow sensor outputs approximately 4.5 pulses per second per litre/minute of flow.
float calibrationFactor = 4.5;
volatile byte flowMeter1PulseCount;
volatile byte flowMeter2PulseCount;
//float flowRate;
//unsigned int flowMilliLitres;
//unsigned long totalMilliLitres;
//unsigned long oldTime;

void setup(){
  
  // setup LEDs
  pinMode(leds1Pin, OUTPUT);
  pinMode(leds2Pin, OUTPUT);
  pinMode(leds3Pin, OUTPUT);

  // setup 12v LED strips
  pinMode(ledStrip1Pin, OUTPUT);
  digitalWrite(ledStrip1Pin, LOW); // turn off
  pinMode(ledStrip2Pin, OUTPUT);
  digitalWrite(ledStrip2Pin, LOW); // turn it off
  pinMode(ledStrip1PotPin, INPUT);
  pinMode(ledStrip2PotPin, INPUT);
  ledStrip1Intensity = 0;
  ledStrip2Intensity = 0;
  ledStrip1IsOscControlled;
  ledStrip2IsOscControlled;
  
  // Flow meters
  pinMode(flowMeter1Pin, INPUT_PULLUP);
  pinMode(flowMeter2Pin, INPUT_PULLUP);
  digitalWrite(flowMeter1Pin, HIGH); // pullup
  digitalWrite(flowMeter2Pin, HIGH); // pullup
  attachInterrupt( digitalPinToInterrupt(flowMeter1Pin), flowMeter1PulseCounter, FALLING);
  attachInterrupt( digitalPinToInterrupt(flowMeter2Pin), flowMeter2PulseCounter, FALLING);
  flowMeter1PulseCount = 0;
  flowMeter2PulseCount = 0;
//  flowRate          = 0.0;
//  flowMilliLitres   = 0;
//  totalMilliLitres  = 0;
//  oldTime           = 0;

  // setup serial communication
  serial.setPacketHandler(&onPacket);
  serial.begin(115200);
  //Serial.begin(115200);

//  while (!serial) {
//    ; // wait for serial port to connect. Needed for Leonardo only
//  }
}


void loop(){
//  while (Serial.available() > 0) {
//    
//    Serial.write(Serial.read());
//  }

  // tmp
  //byte buf[41];
  //char* myString = "serial Test from Arduino loop()\n";
  //serial.send( (uint8_t *)myString, 40);
  //Serial.println("Arduino Serial\n");

  // LED Strips
  {
    ledStrip1Intensity = analogRead(ledStrip1PotPin)/1023.0*255;
    ledStrip2Intensity = analogRead(ledStrip2PotPin)/1023.0*255;

    analogWrite(ledStrip1Pin, ledStrip1Intensity);
    analogWrite(ledStrip2Pin, ledStrip2Intensity);
  }

  // led stuff
  {
    analogWrite(leds1Pin, leds1Brightness);

    // change the brightness for next time through the loop:
    //brightness = brightness + fadeAmount;

    // reverse the direction of the fading at the ends of the fade:
//    if (brightness == 0 || brightness == 255) {
//      fadeAmount = -fadeAmount ;
//    }

    // Flow meter
    {
      /*/ calc flow variables
      if ((millis() - oldTime) > 1000)   // Only process counters once per second
      {
        // Disable the interrupt while calculating flow rate and sending the value to
        // the host
        //detachInterrupt(digitalPinToInterrupt(flowMeterPin));

        // Because this loop may not complete in exactly 1 second intervals we calculate
        // the number of milliseconds that have passed since the last execution and use
        // that to scale the output. We also apply the calibrationFactor to scale the output
        // based on the number of pulses per second per units of measure (litres/minute in
        // this case) coming from the sensor.
        flowRate = ((1000.0 / (millis() - oldTime)) * pulseCount) / calibrationFactor;

        // Note the time this processing pass was executed. Note that because we've
        // disabled interrupts the millis() function won't actually be incrementing right
        // at this point, but it will still return the value it was set to just before
        // interrupts went away.
        oldTime = millis();

        // Divide the flow rate in litres/minute by 60 to determine how many litres have
        // passed through the sensor in this 1 second interval, then multiply by 1000 to
        // convert to millilitres.
        flowMilliLitres = (flowRate / 60) * 1000;

        // Add the millilitres passed in this second to the cumulative total
        totalMilliLitres += flowMilliLitres;

        unsigned int frac;

        // Print the flow rate for this second in litres / minute
        Serial.print("Flow rate: ");
        Serial.print(int(flowRate));  // Print the integer part of the variable
        Serial.print(".");             // Print the decimal point
        // Determine the fractional part. The 10 multiplier gives us 1 decimal place.
        frac = (flowRate - int(flowRate)) * 10;
        Serial.print(frac, DEC) ;      // Print the fractional part of the variable
        Serial.print("L/min");
        // Print the number of litres flowed in this second
        Serial.print("  Current Liquid Flowing: ");             // Output separator
        Serial.print(flowMilliLitres);
        Serial.print("mL/Sec");

        // Print the cumulative total of litres flowed since starting
        Serial.print("  Output Liquid Quantity: ");             // Output separator
        Serial.print(totalMilliLitres);
        Serial.println("mL");

        // Reset the pulse counter so we can start incrementing again
        pulseCount = 0;

        // Enable the interrupt again now that we've finished sending output
        //attachInterrupt( digitalPinToInterrupt(flowMeterPin), pulseCounter, FALLING);
      } // */


    }
  }

  // update serial communication
  serial.update();

  // tmp ?
  //delay(10);
}

// Serial packet callback
void onPacket(const uint8_t* buffer, size_t size){
  if(size==0){
    return;
  }
  //Serial.print("Got Packet Serial!");
  //serial.send((uint8_t *)"Arduino Got Packet serial!", 26);

  // try to keep this small in execution time so the callback doesn't block the arduino for long

  // convert to non-const char
  //uint8_t nonConstBuf[size];//f = buffer;// = "-----";
  //memcpy(nonConstBuf, buffer, size);

  //String matchAddress = "ping";
  const uint8_t matchAddr[] = "ping";
  uint8_t separatorChar = ':'; // addr:value
  if( (buffer[sizeof(matchAddr)-1]) == separatorChar){ // skips quickly if simple condition not met
    int cursorPos = 0;
    //serial.send((uint8_t *)"Entering while!", 16);
    while(cursorPos < sizeof(matchAddr)-1 && cursorPos < size){
      
//      char charReadableSize[20];//5];
//      String str;
//
//      str=String( buffer[cursorPos] );
//      str.concat('-');
//      str.concat((int)cursorPos);
//      str.concat('-');
//      str.concat(matchAddr[cursorPos]);
//      str.concat('-');
//      str.concat( sizeof(matchAddr) );
//  
//      str.toCharArray(charReadableSize, 20);
//      serial.send( ((uint8_t*)charReadableSize), 20 );
      //serial.send((uint8_t *)"Doing letter"+((char)buffer[cursorPos]), 30);
      
      // end search ? ?
      if(cursorPos==(int)sizeof(matchAddr)){
        break;
      }
      else if(buffer[cursorPos]==matchAddr[cursorPos]){
        cursorPos++;
      }
      else {
        //serial.send((uint8_t *)"Breaking here!", 14);
        break;
      }
      //serial.send((uint8_t *)"Damn!"+((char)cursorPos), 44);
    }

    // match address match? (with or without value)
    if(cursorPos==sizeof(matchAddr)-1 && (buffer[cursorPos]==separatorChar || (sizeof(matchAddr)-1)==size ) ){
      // send ping back!
      serial.send((uint8_t *)"Arduino Pings you back!", 23);

      // get value ?
      if(buffer[cursorPos] == separatorChar && size > cursorPos){
        String str;
        str=String( (char*) buffer );
        str = str.substring(cursorPos+1);
        char value[str.length()];
        str.toCharArray(value, (int)sizeof(value));
        //serial.send((uint8_t *)value, sizeof(value) );
      }
      else {
        //serial.send((uint8_t *)"No Value!", 10);
        
//        char charReadableSize[20];
//        String str;
//  
//        str=String( buffer[cursorPos] );
//        str.concat('-');
//        str.concat((int)cursorPos);
//        str.concat('-');
//        str.concat(matchAddr[cursorPos]);
//        str.concat('-');
//        str.concat( size );
//        str.concat('-');
//        str.concat(separatorChar);
//    
//        str.toCharArray(charReadableSize, 20);
//        serial.send( ((uint8_t*)charReadableSize), 20 );
      }
    }
  }
  else {
    //uint8_t * tmp = (buffer[sizeof(matchAddr)]+((uint8_t *)""));
    //serial.send(tmp, sizeof(tmp));
    //serial.send(buffer, size);

    //uint8_t tmp = (uint8_t) buffer[sizeof(matchAddr)-1];
    
    /*/ convert 
    int tmpSize = sizeof(matchAddr);
    char charReadableSize[20];//5];
    String str;

    str=String( tmpSize);
    str.concat('-');
    str.concat(separatorChar);
    str.concat('-');
    str.concat(buffer[sizeof(matchAddr)-1]);
    str.concat('-');
    str.concat( (int)( (buffer[sizeof(matchAddr)-1]) == separatorChar ) );

    str.toCharArray(charReadableSize, 20);
    serial.send( ((uint8_t*)charReadableSize), 20 );
    // */



    
//    serial.send( (uint8_t*) tmpSize, 2 );
//    serial.send( (uint8_t*) separatorChar, 2 );
//    serial.send( (uint8_t*) (char) buffer[sizeof(matchAddr)-1], 1);
    
//    str=String( tmpSize);
//    str.toCharArray(charReadableSize,5);
//    serial.send( ((uint8_t*)charReadableSize), 1 );
    
//    uint8_t tmp = (uint8_t) buffer[sizeof(matchAddr)-1];
//    //uint8_t tmp2 = strcasestr(nonConstBuf,separatorChar);
//    char tmpS = buffer[tmpSize-1];
//    str=String(tmpS);
//    str.concat(separatorChar);
//    str.toCharArray(charReadableSize,5);
//    serial.send( ((uint8_t*)charReadableSize), 2 );
    
    //serial.send((uint8_t*) tmpS, 1);
    //Serial.println( buffer[sizeof(matchAddr)-1] );
  }

  //if(curPos == sizeof(matchAddr) && strcmp()==0){
    
  //}
    
  
  
  //if(matchAddress.equals());
 // const uint8_t matchAddr[] = "ping";// = (unsigned char*)"ping";
  
  //int separatorPos = buffer.indexOf(separatorChar);
  //char* separatorPos =
  //strcasestr(buffer,separatorChar);
  //uint8_t char* = strstr(nonConstBuf,separatorChar);
  //if(separatorPos != null ){
    //if(matchAddr.equals(buffer.substring(0,separatorPos)) == 0 ){
    // if( strcmp(nonConstBuf, matchAddr)==0){
    
    //char* value = buffer.substring(separatorPos);
    
  //}
  // free memory
  //free(matchAddr);
  
//  // Make a temporary buffer.
//  uint8_t tmp[size]; 
//  
//  // Copy the packet into our temporary buffer.
//  memcpy(tmp, buffer, size); 
//  
//  // Reverse our temporaray buffer.
//  //reverse(tmp, size);
//  
//  // Send the reversed buffer back.
//  // The send() method will encode the buffer
//  // as a packet, set packet markers, etc.
//  serial.send(tmp, size);
//  {
//    char buffer2[50];
//    sprintf(buffer2, "%s", tmp);
//    Serial.println(buffer2);
//  }
}

/*
Insterrupt Service Routine (for flow meters)
 */
void flowMeter1PulseCounter() {
  // Increment the pulse counter
  flowMeter1PulseCount++;
  //Serial.println("Aruino Interrupt!");
}

void flowMeter2PulseCounter() {
  flowMeter2PulseCount++;
}

