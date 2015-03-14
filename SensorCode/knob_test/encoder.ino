#define NONE 0
#define LEFT 1
#define RIGHT 2

#define zero_zero 0
#define zero_one 1
#define one_zero 2
#define one_one 3

const int buttonPin = 4;    // the number of the pushbutton pin
int buttonState;             // the current reading from the input pin
int lastButtonState = LOW;   // the previous reading from the input pin
long lastDebounceTime = 0;  // the last time the output pin was toggled
long debounceDelay = 50;    // the debounce time; increase if the output flickers

void setup(){

  Serial.begin(9600);

  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  pinMode(buttonPin, INPUT_PULLUP);
}

int concatenate(int x, int y){ 
    int pow = 2;
    return x * pow + y;
}

//*********************************************************************** 
//                        deal_with_encoder 	 
//This function masks to get the value of the encoders to determines if  
//the encoder was turned left or right. It saves the value of the 
//previous value to compare to the new value 
//***********************************************************************
int deal_with_encoder(void){
    int pin_two = 0;
    int pin_three = 0;
    int encoder = 0;
    int spin = NONE;
    static int old_encoder = 0;
    
    pin_two = digitalRead(2);
    pin_three = digitalRead(3);
    
    encoder = concatenate(pin_two,pin_three); 

    switch(encoder){
      case 0:
        if (old_encoder == zero_one){
          spin = RIGHT;
        } else if (old_encoder == one_zero){
          spin = LEFT;
        } else{
          spin= NONE;
        }
        break;
      
      case 1:
        if (old_encoder == zero_zero){
          spin = LEFT;
        } else if (old_encoder == one_one){
          spin = RIGHT;
        } else{
          spin= NONE;
        }
        break;
      
      case 2:
        if (old_encoder == zero_zero){
          spin = RIGHT;
        } else if (old_encoder == one_one){
          spin = LEFT;
        } else{
          spin= NONE;
        }
        break;
        
        case 3:
        if (old_encoder == one_zero){
          spin = RIGHT;
        } else if (old_encoder == zero_one){
          spin = LEFT;
        } else{
          spin= NONE;
        }
        break;
    }
        
    //Save the encoder values to compare to the new one 
    old_encoder = encoder;
    return spin;
} 

void loop()
{
  int spin = deal_with_encoder();
  int pressed = digitalRead(buttonPin);
  
  //Serial.print("Spin: ");
//  if(spin == 0){
//    Serial.println("No Turn");
//  }else if(spin == 1){
//    Serial.println("Left Turn");
//  }else{ 
//    Serial.println("Right Turn");
//  }
  //Serial.println(spin);
  Serial.print("Switch: ");
  if(pressed == 1);{
        Serial.println("Not Pressed");
  }
  
  // check to see if you just pressed the button 
  // (i.e. the input went from LOW to HIGH),  and you've waited 
  // long enough since the last press to ignore any noise:  

  // If the switch changed, due to noise or pressing:
  if (pressed != lastButtonState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  } 
  
  if ((millis() - lastDebounceTime) > debounceDelay) {
    // whatever the reading is at, it's been there for longer
    // than the debounce delay, so take it as the actual current state:

    // if the button state has changed:
    if (pressed != buttonState) {
      buttonState = pressed;
      if(pressed == 0){
        Serial.println("Pressed");
      }
    }
  }
  delay(100);
  // save the reading.  Next time through the loop,
  // it'll be the lastButtonState:
  lastButtonState = pressed;
}

