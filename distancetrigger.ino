/**
 * Trigger a wired remote (via opto-isolator) in response to distance change
 * (sensed with analog input from a hypersonic rangefinder).
 */

#define PIN_LED		13
#define PIN_DISTANCE	A0
#define THRESHOLD	6
#define SERIAL_BAUD	9600
#define TRIGGER_COUNT	6
#define TRIGGER_DELAY	20
#define SAMPLE_INTERVAL	500

int lastDist;
int sampleClock;

void setup() {
	sampleClock = 0;
	lastDist = analogRead(PIN_DISTANCE);
	pinMode(PIN_LED, OUTPUT);
	Serial.begin(SERIAL_BAUD);
}

void trigger() {
	for (int i = 0; i < TRIGGER_COUNT; i++) {
		digitalWrite(PIN_LED, HIGH);
		delay(TRIGGER_DELAY);
		digitalWrite(PIN_LED, LOW);
	}
}

void loop() {
	int dist = analogRead(PIN_DISTANCE);
	if (sampleClock++ % SAMPLE_INTERVAL == 0) {
		Serial.println(dist);
	}
	int delta = abs(dist - lastDist);
	if (delta >= THRESHOLD) {
		Serial.print("Triggered at ");
		Serial.println(dist);
		lastDist = dist;
		trigger();
	}
}
