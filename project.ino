volatile boolean headlights = false; //bool var for headlights button
volatile boolean wipers = false; //bool var for wipers button
volatile boolean parking = false; //bool var for parking sensor button


const int buzzer = 7; //pin for the buzzer
const int ledPin = 8;
const int trigPin = 6;
const int echoPin = 5;


//TO DO implement these pins into the go -- make sure it works fine
const int autoHeadlightsPin = 12;
const int ledHeadlightsPin = 13;
const int ledHeadlightsPinSecond = 10;
const int buttonHeadlightsPin = 2;

const int autoWipersPin = 11;
const int ledWipersPin = 14;
const int buttonWipersPin = 3;

const int buttonParkingPin = 18;


long duration;
int distance, rain, light, safetyDistance;

void setup()
{
  Serial.begin(9600);
  pinMode(ledHeadlightsPin, OUTPUT); //led for headlight
  pinMode(ledHeadlightsPinSecond, OUTPUT); //led for second headlight
  pinMode(buttonHeadlightsPin, INPUT); //button for headlights
  attachInterrupt(digitalPinToInterrupt(buttonHeadlightsPin), headlightsButton, RISING); //interrupt for headlights button

  pinMode(ledWipersPin, OUTPUT); //led for wipers
  pinMode(buttonWipersPin, INPUT); // button for wipers
  attachInterrupt(digitalPinToInterrupt(buttonWipersPin), wipersButton, RISING); //interrupt for wipers button

//  pinMode(trigPin, OUTPUT);
//  pinMode(echoPin, INPUT);
//  pinMode(buzzer, OUTPUT);
  pinMode(ledPin, OUTPUT); //led for parking sensor
  pinMode(buttonParkingPin, INPUT); //button for parking sensor
  attachInterrupt(digitalPinToInterrupt(buttonParkingPin), parkingSensor, RISING); // interrupt for parking sensor
}

void loop()
{
  light = analogRead(A0);
  rain = analogRead(A1);

  //check if auto headlights is on
  if (headlights == true)
  {
    Serial.println(light);
    //check if there is enough light
    if (light < 90)
    {
      digitalWrite(ledHeadlightsPin, HIGH); 
      digitalWrite(ledHeadlightsPinSecond, HIGH);
      Serial.println("Headlights activated.");
    }
    else
    {
      digitalWrite(ledHeadlightsPin, LOW);
      digitalWrite(ledHeadlightsPinSecond, LOW);
    }
  }


  //check if auto wipers is on
  if (wipers == true)
  {
    Serial.println(rain);
    //check if the is water on the sensor
    if (rain > 400)
    {
      digitalWrite(ledWipersPin, LOW);
    }
    else
    {
      digitalWrite(ledWipersPin, HIGH);
      Serial.println("Wipers activated.");
    }
  }

  if (parking == true)
  {
    distance = calculateDistance();
    
    Serial.print("Distance : ");
    Serial.println(distance);

    safetyDistance = distance;
    
    if (safetyDistance <= 5)
    {
      digitalWrite(ledPin, HIGH);
      Serial.println("BUZZZZZ");
      tone(buzzer, 450);
      delay(100);
      noTone(buzzer);
      delay(100);
      safetyDistance = calculateDistance();
    }
  }
  delay(300);
}

int calculateDistance()
{
  //make sure the trig pin is empty
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  //generate ultrasound wave
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;

  return distance;
}

//interrupt function for headlights button
void headlightsButton()
{
  if (headlights == true)
  {
    headlights = false;
    digitalWrite(autoHeadlightsPin, LOW);
    digitalWrite(ledHeadlightsPin, LOW);
    digitalWrite(ledHeadlightsPinSecond, LOW);
  }
  else
  {
    headlights = true;
    digitalWrite(autoHeadlightsPin, HIGH);
  }
}

//interrupt function for wipers button
void wipersButton()
{
  if (wipers == true)
  {
    wipers = false;
    digitalWrite(autoWipersPin, LOW);
  }
  else
  {
    wipers = true;
    digitalWrite(autoWipersPin, HIGH);
  }
}

//interrupt function for parking sensor
void parkingSensor()
{
  if (parking == true)
  {
    parking = false;
    //digitalWrite(ledPin, LOW);
  }
  else
  {
    parking = true;
    //digitalWrite(ledPin, HIGH);
  }
}
