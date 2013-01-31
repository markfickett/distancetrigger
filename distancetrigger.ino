/**
 * Trigger a wired remote (via opto-isolator) in response to distance change
 * (sensed with analog input from a hypersonic rangefinder).
 */

#define PIN_LED		13
#define PIN_TRIGGER	12
#define PIN_DISTANCE	A0
#define THRESHOLD_F	0.15	// Percent change in distance that will trigger.
#define DISTANCE_MAX	300	// Maximum distance to register.
#define SERIAL_BAUD	9600
#define TRIGGER_DELAY	80
#define SAMPLE_INTERVAL	500

int lastDist;
int sampleClock;

void setup() {
	sampleClock = 0;
	lastDist = min(DISTANCE_MAX, analogRead(PIN_DISTANCE));
	pinMode(PIN_LED, OUTPUT);
	pinMode(PIN_TRIGGER, OUTPUT);
	digitalWrite(PIN_TRIGGER, LOW);
	Serial.begin(SERIAL_BAUD);
}

void trigger() {
	digitalWrite(PIN_LED, HIGH);
	digitalWrite(PIN_TRIGGER, HIGH);
	delay(TRIGGER_DELAY);
	digitalWrite(PIN_TRIGGER, LOW);
	digitalWrite(PIN_LED, LOW);
}

void loop() {
	int dist = min(DISTANCE_MAX, analogRead(PIN_DISTANCE));
	float delta = float(abs(dist - lastDist))/lastDist;
	if (sampleClock++ % SAMPLE_INTERVAL == 0) {
		Serial.print(dist);
		Serial.print("\t");
		Serial.println(delta);
	}
	if (delta >= THRESHOLD_F) {
		Serial.print("Triggered at ");
		Serial.print(dist);
		Serial.print(" (");
		Serial.print(delta);
		Serial.println(").");
		lastDist = dist;
		trigger();
	}
}
