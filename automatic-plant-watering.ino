/* Automatic gardener control program.
 *  
 * The automatic gardened is controlled by the water level 
 * on the reservoir and by the gauges on the front of the case.
 * 
 * The front gauges control the time between the pumping and the
 * time with the pump on. It also has the on/off lever and a
 * reset button.
 * 
 * Notes:
 *   Using the ATTiny85 programmer shield developed using the
 *   description from: 
 *   https://create.arduino.cc/projecthub/arjun/programming-attiny85-with-arduino-uno-afb829
 * Joao Aguirre
 * 2018-07-01
 */

const unsigned int RESERVOIR = 0; // Liquid level sensor on the reservoir.
const unsigned int ALERT = 1; // Red LED used to indicate the low water level.
const unsigned int RELAY = 2; // Relay used to activate the water pump.
const unsigned int TIMER = 3; // Potentiometer controlling the time between pumping.
const unsigned int PUMP = 4;  // Potentiometer controlling the pumping time.

const bool EXCLUDE_EXTREMES = true;
const unsigned int NUMBER_MEASURES = 10;
const unsigned long HOUR = 3600000UL;
const unsigned long SECOND = 1000UL;

bool system_start;
int level_alert;
int timer_read;
int pump_read;
unsigned long previous_millis;
unsigned long current_millis;
unsigned long time_passed;
unsigned long waiting_time;
unsigned long pumping_time;

unsigned long get_waiting_time(int timer_read);
unsigned long get_pumping_time(int pump_read);
int read_average(int pin, int number_reads, bool exclude_extremes);
int compare(int a, int b);

void setup()
{
    // Initialize system variables;
    system_start = true;
    level_alert = LOW;
    timer_read = 0;
    pump_read = 0;
    previous_millis = 0UL;
    current_millis = 0UL;
    time_passed = 0UL;
    waiting_time = 0UL;
    pumping_time = 0UL;

    // Initialize pins and communication.
    pinMode(RESERVOIR, INPUT);
    pinMode(ALERT, OUTPUT);
    pinMode(RELAY, OUTPUT);

    // Turn the reservoir alert LED off and, turn off the relay.
    digitalWrite(ALERT, LOW);
    digitalWrite(RELAY, LOW);
}

void loop()
{
    // Read the water level sensor:
    // (LOW = water level ok, HIGH = water level too low).
    level_alert = digitalRead(RESERVOIR);
    if (level_alert == LOW)
    {
        // Make sure that the water level alert LED is off.
        digitalWrite(ALERT, LOW);
        // Read the wating interval from the potentiometer.
        timer_read = read_average(TIMER, NUMBER_MEASURES, EXCLUDE_EXTREMES);
        waiting_time = get_waiting_time(timer_read);
        current_millis = millis();
        // Using the tip of account for the rollover with subtraction from:
        // https://www.baldengineer.com/arduino-how-do-you-reset-millis.html
        time_passed = (unsigned long)(current_millis - previous_millis);
        if (time_passed >= waiting_time || system_start)
        {
            pump_read = read_average(PUMP, NUMBER_MEASURES, EXCLUDE_EXTREMES);
            pumping_time = get_pumping_time(pump_read);
            digitalWrite(RELAY, HIGH);
            delay(pumping_time);
            digitalWrite(RELAY, LOW);
            previous_millis = current_millis;
            system_start = false;
        }
    }
    else
    {
        digitalWrite(ALERT, HIGH);
    }
}

unsigned long get_waiting_time(int timer_read)
{
    if (timer_read < 426)
    {
        return 3UL * HOUR;
    }
    else if (timer_read < 830)
    {
        return 6UL * HOUR;
    }
    else if (timer_read < 955)
    {
        return 12UL * HOUR;
    }
    return 24UL * HOUR;
}

unsigned long get_pumping_time(int pump_read)
{
    if (pump_read < 445)
    {
        return 30UL * SECOND;
    }
    else if (pump_read < 840)
    {
        return 60UL * SECOND;
    }
    else if (pump_read < 950)
    {
        return 90UL * SECOND;
    }
    return 120UL * SECOND;
}

int read_average(int pin, int number_reads, bool exclude_extremes)
{
    const int DELAY = 100;
    int readings[number_reads];
    int average = 0;

    if (number_reads < 3)
    {
        number_reads = 3;
    }
    
    for (int i = 0; i < number_reads; ++i)
    {
        readings[i] = analogRead(pin);
        delay(DELAY);
    }
    
    if (exclude_extremes)
    {
        qsort(readings, number_reads, sizeof(int), compare);
        for (int i = 1; i < number_reads - 1; ++i)
        {
            average += readings[i]; 
        }
        return average / (number_reads - 2);
    }
    else
    {
        for (int i = 0; i < number_reads; ++i)
        {
            average += readings[i]; 
        }
        return average / number_reads;
    }
}

int compare(int a, int b)
{
    return (a - b);
}

