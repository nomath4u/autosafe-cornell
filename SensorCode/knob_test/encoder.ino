#define NONE 0
#define LEFT 1
#define RIGHT 2

#define zero_zero 0
#define zero_one 1
#define one_zero 2
#define one_one 3

void setup(){

  Serial.begin(9600);

  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);
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
    
    //pin_two = 1;
    //pin_three = 1;
 
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
  Serial.print("Spin: ");
  Serial.println(spin);
  Serial.print("Switch: ");
  Serial.println(digitalRead(4));
  delay(100);
}

