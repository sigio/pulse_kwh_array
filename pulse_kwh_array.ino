#define PULSES_PER_KWH 2000
#define MS_PER_HOUR 3.6e6

#define NUMREADERS 4
static short inpin[NUMREADERS] = { 12, 10, 8, 6 };
static short outpin[NUMREADERS] = { 11, 9, 7, 5 };
//unsigned long howlong;
//unsigned int loopcount = 0;

void setup () {
   Serial.begin(115200);
   Serial.println("init");
   
   for( short i = 0; i < NUMREADERS; i++ )
   {
     pinMode(inpin[i], INPUT);
     pinMode(outpin[i], OUTPUT);
     digitalWrite(inpin[i], HIGH); // internal pull-up
   }
}

void loop ()
{
  static int laststate[NUMREADERS];
  static unsigned long laststatechangetime[NUMREADERS];
  static unsigned int cycle[NUMREADERS];
  static unsigned long lastpulsetime[NUMREADERS];

//  if ( loopcount == 0 )
//  {
//    howlong = millis();
//  }
//  loopcount++;
//  if ( (loopcount % 100000) == 0 )
//  {
//    unsigned long now = millis();
//    Serial.print( "100000 Loops took: " );
//    Serial.println( now - howlong );
//    howlong = now;
//  }
   
  for( short i = 0; i < NUMREADERS; i++ )
  {
    unsigned long now = millis();
    
    if( laststatechangetime[i] > now )
    {
//      Serial.print( "Sensor " );
//      Serial.print( i, DEC );
//      Serial.println( " still in debounce" );
    }
    else
    {   
      int current = digitalRead(inpin[i]);

      if (laststate[i] != current)
      {
        unsigned long delta = now - laststatechangetime[i];
        if (delta > 30)
        {
          /* pulse! */
          if (lastpulsetime[i])
          {
            unsigned long elapsed = now - lastpulsetime[i];
            double W = 1000 * ((double) MS_PER_HOUR / elapsed) / 
PULSES_PER_KWH;
            Serial.print("Reader " );
            Serial.print( i, DEC );
            Serial.print(": Cycle ");
            Serial.print(++cycle[i], DEC);
            Serial.print(": ");
            Serial.print(elapsed, DEC);
            Serial.print(" ms, ");
            Serial.print(W, 2);
            Serial.println(" W");
          }
          lastpulsetime[i] = now;

          // debounce
          laststatechangetime[i] = millis() + 200;
          laststate[i] = current;
        } // lastpulsetime
        else
        {
          laststate[i] = current;
          laststatechangetime[i] = now;
        } // not lastpulsetime
      } // laststate == current
    } // debounce
  } // for-sensorloop
} // loop()

