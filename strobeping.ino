// ---------------------------------------------------------------------------
// Example NewPing library sketch that does a ping about 20 times per second.
// ---------------------------------------------------------------------------

#include <NewPing.h>

#define TRIGGER_PIN  12  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     11  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 200 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.
#define strobepin 3 
#define MS_ON 40
#define MS_OFF  22

//#define  DEBUG_SERIAL

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.
unsigned long last_trig = 0;
boolean strobe = false;
unsigned long strobe_start = 0;
unsigned long strobe_on = 0;
 
void setup() {
#ifdef DEBUG_SERIAL
	Serial.begin(115200); // Open serial monitor at 115200 baud to see ping results.
#endif
	pinMode(13, OUTPUT);
	digitalWrite(13, LOW);
	pinMode(strobepin, OUTPUT);
	digitalWrite(strobepin, LOW);
}

void loop() {

	unsigned long  curtime = millis();
	if (strobe) {
		if (curtime > (strobe_start + 2000)){
			strobe = false;
			last_trig = curtime;
			digitalWrite(strobepin, LOW);
			last_trig = curtime;

		}
		else
		{
			if (curtime > (strobe_on + MS_ON)){
				digitalWrite(strobepin, LOW);
				delay(MS_OFF);
 				digitalWrite(strobepin, HIGH);
				strobe_on = millis();
			}
	
		}

	}
	int val = analogRead(A0);            // reads the value of the potentiometer (value between 0 and 1023) 
	val = map(val, 0, 1023, 0, 200);     // scale it to use it with the servo (value between 0 and 180) 


	delay(50);                     // Wait 50ms between pings (about 20 pings/sec). 29ms should be the shortest delay between pings.

	int dist = sonar.ping_cm();

#ifdef DEBUG_SERIAL
 	Serial.print(val);
 	Serial.print(" Ping: ");
 	Serial.print(dist); // Send ping, get distance in cm and print result (0 = outside set distance range)
 	Serial.println("cm");
#endif

	if (dist < val) {
		digitalWrite(13, HIGH);

		if (curtime > (last_trig + 10000)){
			strobe_start = curtime;
			last_trig = curtime;
			strobe = true;
			digitalWrite(strobepin, HIGH);
			strobe_on = curtime;
	 

		}



	}
	else
	{
		digitalWrite(13, LOW);
	}


}
