/**
 * Trigger a wired remote (via opto-isolator) in response to distance change
 * (sensed with analog input from a hypersonic rangefinder).
 */

#define PIN_LED			13
#define PIN_TRIGGER		12
#define PIN_DISTANCE		A0
#define THRESHOLD_F		0.55	// %Delta-distance that will trigger.
#define THRESHOLD_DIST		180	// Distances greater than this trigger.
#define DISTANCE_MAX		300	// Maximum distance to register.
#define SERIAL_BAUD		9600
#define TRIGGER_DELAY		0
#define TRIGGER_DURATION	80
#define SAMPLE_INTERVAL		200
#define SMOOTHING_WINDOW	2

int lastDist;
int sampleClock;
int averages[SMOOTHING_WINDOW];

void setup() {
	for (int i = 0; i < SMOOTHING_WINDOW; i++) {
		averages[i] = 0;
	}
	sampleClock = 0;
	lastDist = min(DISTANCE_MAX, analogRead(PIN_DISTANCE));
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
	int rawDist = min(DISTANCE_MAX, analogRead(PIN_DISTANCE));
	averages[sampleClock % SMOOTHING_WINDOW] = rawDist;
	int dist = 0;
	for (int i = 0; i < SMOOTHING_WINDOW; i++) {
		dist += averages[i];
	}
	dist /= SMOOTHING_WINDOW;
	if (sampleClock++ % SAMPLE_INTERVAL == 0) {
		Serial.print(rawDist);
		Serial.print("\t");
		Serial.println(dist);
	}
	if (dist > THRESHOLD_DIST) {
		Serial.print(rawDist);
		Serial.print("\t");
		Serial.print(dist);
		Serial.println("\ttriggered");
		lastDist = dist;
		trigger();
	}
}
