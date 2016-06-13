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
int ledStrip1IntensityManu = 0; // 0-255
int ledStrip2IntensityManu = 0; // 0-255
int ledStrip1IntensityAuto = 0; // 0-255
int ledStrip2IntensityAuto = 0; // 0-255
bool ledStrip1IsSerialControlled = false;
bool ledStrip2IsSerialControlled = false;

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

// ElectroValves
int EV1Pin = 4;
int EV2Pin = 7;
int EV3Pin = 8;
int EV4Pin = 12;

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
  ledStrip1IntensityManu = 0;
  ledStrip2IntensityManu = 0;
  ledStrip1IntensityAuto = 0;
  ledStrip2IntensityAuto = 0;
  ledStrip1IsSerialControlled = false;
  ledStrip2IsSerialControlled = false;
  
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

  uint8_t tmp[42];
  memcpy(tmp, "karmaMapper reporting for duty! (Online)\n", 42);
  serial.send(tmp, 42);
  // Make an array.
//uint8_t myPacket[] { 255, 10 };


  //Serial.begin(115200);

#if ARDUINO >= 100
    while(!Serial)
      ;   // Leonardo bug
#endif

  //serial.send(myPacket, 2);
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
    // store value for later comparison
    int ledStrip1IntensityManuTmp = ledStrip1IntensityManu;
    int ledStrip2IntensityManuTmp = ledStrip2IntensityManu;

    // get latest manu value
    ledStrip1IntensityManu = analogRead(ledStrip1PotPin)/1023.0*255;
    ledStrip2IntensityManu = analogRead(ledStrip2PotPin)/1023.0*255;

    // write the right luminosities to the LED strips
    if(ledStrip1IsSerialControlled){
      analogWrite(ledStrip1Pin, ledStrip1IntensityAuto*((float)ledStrip1IntensityManu/255.0) );
    }
    else {
      analogWrite(ledStrip1Pin, ledStrip1IntensityManu);
    }
    
    if(ledStrip2IsSerialControlled){
      analogWrite(ledStrip2Pin, ledStrip2IntensityAuto*((float)ledStrip2IntensityManu/255.0) );
    }
    else {
      analogWrite(ledStrip2Pin, ledStrip2IntensityManu);
    }

    // notify LED strip intensity changes over serial
    if(ledStrip1IntensityManu != ledStrip1IntensityManuTmp){
      String str=String("ledStripIntensityManu:");
      str.concat(0);
      str.concat('-');
      str.concat(ledStrip1IntensityManu);
      char charReadableSize[str.length()+1];
      str.toCharArray(charReadableSize, str.length()+1);
      serial.send( ((uint8_t*)charReadableSize), str.length()+1 );
    }

    // changed ?
    //if(ledStrip2IntensityManu != ledStrip2IntensityManuTmp){
      String str=String("ledStripIntensityManu:");
      str.concat(1);
      str.concat('-');
      str.concat(ledStrip1IntensityManu);
      str.concat('-');
      char charReadableSize[str.length()];
      str.toCharArray(charReadableSize, str.length());
      serial.send( ((uint8_t*)charReadableSize), str.length() );
    //}
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

#define arr_len( x )  ( sizeof( x ) / sizeof( *x ) )

// Checks if a string matches an address pattern
// returns -1 if not matching
// returns position offset to value if matching
int matchAddress(const uint8_t* buffer, size_t size, const uint8_t matchAddr[], size_t matchAddrSize ){

  static uint8_t separatorChar = ':'; // addr:value
  //int matchLength = sizeof(matchAddr)/sizeof(uint8_t);
  //size_t matchLength = arr_len( matchAddr);

//  String str( (char*) buffer );
//  //str = str.substring(cursorPos+1);
//  str.concat(" .vs. ");
//  str.concat( ((char*)matchAddr) );
//  str.concat(" - lastChar=");
//  str += (char) buffer[size-1];
//  str.concat(" - MASize=");
//  str += matchAddrSize;
//  str.concat(" - size=");
//  str += size;
//  str.concat('-');
//  char value[str.length()];
//  str.toCharArray(value, (int)sizeof(value));
//  serial.send((uint8_t *)value, sizeof(value) );
  
  if( size >= matchAddrSize && (buffer[matchAddrSize]) == separatorChar){ // skips quickly if simple condition not met
    int cursorPos = 0;
    //serial.send((uint8_t *)"Entering while!", 16);
    
    while(cursorPos < matchAddrSize && cursorPos < size){
      
//      char myConcatenation[80];
//      sprintf(myConcatenation,"%i = %c",cursorPos, (char) buffer[cursorPos]);
//      serial.send((uint8_t *) myConcatenation, 80);
      
      // end search ? ?
      if(cursorPos==matchAddrSize){
        //serial.send((uint8_t *)"Breaking found!", 15);
        break;
      }
      else if(buffer[cursorPos]==matchAddr[cursorPos]){
        cursorPos++;
      }
      else {
        // not matching
        //serial.send((uint8_t *)"Breaking here!", 14);
        break;
      }
      
    }

    // match address match? (with or without value)
    if(cursorPos==matchAddrSize && (buffer[cursorPos]==separatorChar || (matchAddrSize-1)==size ) ){
      //serial.send((uint8_t *)("Result: "), 8);
      return cursorPos;
    }
    else {
      //serial.send((uint8_t *)("Failed: "), 8);
      return -1;
    }
  }

  return -1;
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

  // answer pings
  #define ping "ping"
  int cursorPos = matchAddress(buffer, size, (uint8_t *) ping, sizeof(ping)-1);
  if( cursorPos > 0 ){
    // send ping back!
    serial.send((uint8_t *)"pingback: Arduino Response", 22);

    // get value ?
//    if(size-1 > cursorPos){
//      String str( (char*) buffer);//, cursorPos+1, (int)size-(cursorPos+1));
//      str = str.substring(cursorPos+1); // skip separator
//      char value[str.length()];
//      str.toCharArray(value, (int)sizeof(value));
//      //serial.send((uint8_t *)value, sizeof(value) );
//      
////      char myConcatenation[80];
////      sprintf(myConcatenation,"%i = %s - %i",cursorPos+1, value, str.length() );
////      serial.send((uint8_t *) myConcatenation, 80);
//    }
//    else {
//      // empty message, just the address
//      //serial.send((uint8_t *)"No Value!", 10);
//    }
    return;
  }
  else {
    // no match
    //serial.send((uint8_t *)("Not a Ping! "+cursorPos ), 15);
  }
  
  // check to set water flow ?
  #define setEV "setEV"
  cursorPos = matchAddress(buffer, size, (uint8_t *) setEV, sizeof(setEV)-1);
  if( cursorPos > 0 ) {
//    char myConcatenation[80];
//    sprintf(myConcatenation,"setEV: %i / %i (%s) = %i %c %i", cursorPos, size, (char*)buffer, (int) buffer[cursorPos+1], buffer[cursorPos+2], (int) buffer[cursorPos+3] );
//    serial.send((uint8_t *) myConcatenation, 80);

    // get value ?
    if(size >= cursorPos+4){ // value of 3 long + 1 separator char
//      String str;
//      str=String( (char*) buffer );
//      str = str.substring(cursorPos+1);
//      char value[str.length()];
//      str.toCharArray(value, (int)sizeof(value));

      if(buffer[cursorPos+2]==(uint8_t)'-'){
        
        int valveID = buffer[cursorPos+1];
        int valveValue = buffer[cursorPos+3];

        // todo: allow easier setting of EVs (limited to 4 here)
        if(valveID>=0 && valveID<3){

          bool tmpValue = LOW;
          if(valveValue > 0){
            tmpValue = HIGH;
            //serial.send((uint8_t*)"EV_VALUE=HIGH", 13);
          }
//          else {
//            serial.send((uint8_t*)"EV_VALUE=LOW", 12);
//          }
          
          if(valveID==0){
            digitalWrite(EV1Pin, tmpValue);
            //serial.send((uint8_t*)"EV0=SET", 7);
          }
          else if(valveID==1){
            digitalWrite(EV2Pin, tmpValue);
            //serial.send((uint8_t*)"EV1=SET", 7);
          }
          else if(valveID==2){
            digitalWrite(EV3Pin, tmpValue);
            //serial.send((uint8_t*)"EV2=SET", 7);
          }
          else if(valveID==3){
            digitalWrite(EV4Pin, tmpValue);
            //serial.send((uint8_t*)"EV3=SET", 7);
          }
//          else {
//            serial.send((uint8_t*)"EVx=NOT_SET", 11);
//          }
  
          // send response containing new value (optional)
          String str=String("EVStatus:");
          str.concat(valveID);
          str.concat('-');
          str.concat(valveValue);
          char charReadableSize[str.length()+1];
          str.toCharArray(charReadableSize, str.length()+1);
          serial.send( ((uint8_t*)charReadableSize), str.length()+1 );

        }
      }
      else {
        //serial.send((uint8_t *)"failEV:", 6);
      }
      
      //serial.send((uint8_t *)"setEV:", 5);
    }
    else {
      // empty message, just the address
      //serial.send((uint8_t *)"No Value!", 10);
    }
    return;
  }
  else {
    // no match
    //serial.send((uint8_t *)("Not a setEV! "), 12);
  }

  // check to set LED strips ?
  #define setLEDstrips "setLEDstrips"
  cursorPos = matchAddress(buffer, size, (uint8_t *) setLEDstrips, sizeof(setLEDstrips)-1);
  if( cursorPos > 0 ) {
    if(size >= cursorPos+4){ // value of 3 long + 1 separator char
      if(buffer[cursorPos+2]==(uint8_t)'-'){
        int stripID = buffer[cursorPos+1];
        int stripValue = buffer[cursorPos+3];
  
        // todo: allow easier setting of EVs (limited to 2 here)
        if(stripID>=0 && stripID<2){

          if(stripID==0){
            ledStrip1IntensityAuto = stripValue;
          }
          else {
            ledStrip2IntensityAuto = stripValue;
          }

          String str=String("LEDstripAutoStatus:");
          str.concat(stripID);
          str.concat('-');
          str.concat(stripValue);
          char charReadableSize[str.length()+1];
          str.toCharArray(charReadableSize, str.length()+1);
          serial.send( ((uint8_t*)charReadableSize), str.length()+1 );
          
        }
      }
    }
  }

  // THERES ONLY UNUSED CODE BELOW THIS POINT
  
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
  //}

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

