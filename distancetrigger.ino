/**
 * Trigger a wired remote (via opto-isolator) in response to breaking a beam
 * (sensed with analog input from an ambient light sensor).
 */

#define PIN_LED			13
#define PIN_TRIGGER		12
#define PIN_BEAM		11
#define PIN_BEAM_SENSE		A0
#define THRESHOLD		650	// Brightness less than this triggers.
#define SERIAL_BAUD		9600
//#define TRIGGER_DELAY		0
#define TRIGGER_DURATION	80
#define SAMPLE_INTERVAL		1500

int sampleClock;

void setup() {
	sampleClock = 0;
	pinMode(PIN_LED, OUTPUT);
	pinMode(PIN_BEAM, OUTPUT);
	digitalWrite(PIN_BEAM, HIGH);
	pinMode(PIN_TRIGGER, OUTPUT);
	digitalWrite(PIN_TRIGGER, LOW);
	Serial.begin(SERIAL_BAUD);
}

void trigger() {
	#ifdef TRIGGER_DELAY
	delay(TRIGGER_DELAY);
	#endif
	digitalWrite(PIN_TRIGGER, HIGH);
	digitalWrite(PIN_BEAM, LOW);
	digitalWrite(PIN_LED, HIGH);
	delay(TRIGGER_DURATION);
	digitalWrite(PIN_LED, LOW);
	digitalWrite(PIN_BEAM, HIGH);
	digitalWrite(PIN_TRIGGER, LOW);
}

void loop() {
	int level = analogRead(PIN_BEAM_SENSE);
	if (sampleClock++ % SAMPLE_INTERVAL == 0) {
		Serial.println(level);
	}
	if (level < THRESHOLD) {
		Serial.print(level);
		Serial.println("\ttriggered");
		trigger();
	}
}
