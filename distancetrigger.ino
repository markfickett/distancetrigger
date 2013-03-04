/**
 * Trigger a wired remote (via opto-isolator) in response to breaking a beam
 * (sensed with analog input from an ambient light sensor).
 */

#define PIN_LED			13
#define PIN_TRIGGER		12
#define PIN_BEAM		A0
#define THRESHOLD		250	// Brightness less than this triggers.
#define SERIAL_BAUD		9600
#define TRIGGER_DELAY		0
#define TRIGGER_DURATION	80
#define SAMPLE_INTERVAL		1500
#define SMOOTHING_WINDOW	1

int sampleClock;
int averages[SMOOTHING_WINDOW];

void setup() {
	for (int i = 0; i < SMOOTHING_WINDOW; i++) {
		averages[i] = 0;
	}
	sampleClock = 0;
	pinMode(PIN_LED, OUTPUT);
	pinMode(PIN_TRIGGER, OUTPUT);
	digitalWrite(PIN_TRIGGER, LOW);
	Serial.begin(SERIAL_BAUD);
}

void trigger() {
	delay(TRIGGER_DELAY);
	digitalWrite(PIN_LED, HIGH);
	digitalWrite(PIN_TRIGGER, HIGH);
	delay(TRIGGER_DURATION);
	digitalWrite(PIN_TRIGGER, LOW);
	digitalWrite(PIN_LED, LOW);
}

void loop() {
	int rawLevel = analogRead(PIN_BEAM);
	averages[sampleClock % SMOOTHING_WINDOW] = rawLevel;
	int level = 0;
	for (int i = 0; i < SMOOTHING_WINDOW; i++) {
		level += averages[i];
	}
	level /= SMOOTHING_WINDOW;
	if (sampleClock++ % SAMPLE_INTERVAL == 0) {
		Serial.print(rawLevel);
		Serial.print("\t");
		Serial.println(level);
	}
	if (level < THRESHOLD) {
		Serial.print(rawLevel);
		Serial.print("\t");
		Serial.print(level);
		Serial.println("\ttriggered");
		trigger();
	}
}
