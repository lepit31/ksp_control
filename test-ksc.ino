#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
LiquidCrystal_I2C lcd(0x27, 16, 2); // set the LCD address to 0x27 for a 16 chars and 2 line display


const byte ROWS = 4; // define row 4
const byte COLS = 4; // define column 4
char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

char val_maneuver ;
// connect row ports of the button to corresponding IO ports on the board
byte rowPins[ROWS] = {51, 49, 47, 45};
// connect column ports of the button to corresponding IO ports on the board
byte colPins[COLS] = {43, 41, 39, 37};
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

// nb display state
const int NB_DISPLAY_STATE = 7 ;
// current display State
int val_display_state = 0;

const int ref_joy_l_y = A0;
const int ref_joy_l_x = A1;
const int ref_joy_l_z = A2;

const int ref_slider  = A3;

const int ref_joy_r_x = A4;
const int ref_joy_r_y = A5;
const int ref_joy_r_z = A6;

const int ref_stage_led = 50;
const int ref_stage_btn = 48;
const int ref_sas_led   = 46;
const int ref_sas_btn   = 44;
const int ref_next_led  = 42;
const int ref_next_btn  = 40;

const int ref_arm_led    = 38;
const int ref_launch_led = 36;
const int ref_abort_led  = 34;
const int ref_rcs_led    = 32;

const int ref_arm_btn    = 13;
const int ref_launch_btn = 12;
const int ref_abort_btn  = 11;
const int ref_rcs_btn    = 10;

const int ref_cut_btn    = 9;
const int ref_nomi_btn   = 8;
const int ref_full_btn   = 7;

const int ref_light_btn  = 6;
const int ref_ladder_btn = 5;
const int ref_joy_r_btn  = 4;

const int ref_joy_l_btn  = 3;
const int ref_gears_btn  = 2;
const int ref_break_btn = 30;
const int ref_solar_btn  = 28;
const int ref_chute_btn  = 26;

float val_joy_l_y = 0;
float val_joy_l_x = 0;
float val_joy_l_z = 0;

float val_slider  = 0;

float val_joy_r_y = 0;
float val_joy_r_x = 0;
float val_joy_r_z = 0;

int val_stage_btn  = 0;
int val_sas_btn    = 0;
int val_next_btn   = 0;

int val_arm_btn    = 0;
int val_launch_btn = 0;
int val_abort_btn  = 0;
int val_rcs_btn    = 0;

int val_full_btn   = 0;
int val_nomi_btn   = 0;
int val_cut_btn    = 0;


int val_light_btn   = 0;
int val_ladder_btn   = 0;
int val_joy_r_btn    = 0;
int val_joy_l_btn   = 0;

int val_gears_btn   = 0;
int val_break_btn   = 0;
int val_solar_btn   = 0;
int val_chute_btn   = 0;

//for a btn add 1 seconde of latency
const unsigned long BTTIMELATENCY = 1000;

unsigned long now                 = 0 ;
unsigned long val_stage_btn_timer = 0;
unsigned long val_sas_btn_timer   = 0;
unsigned long val_next_btn_timer  = 0;

unsigned long val_abort_btn_timer = 0;
unsigned long val_rcs_btn_timer   = 0;
unsigned long val_joy_r_btn_timer = 0;
unsigned long val_joy_l_btn_timer = 0;


int val_stage_btn_state = 0;
int val_sas_btn_state = 0;
int val_next_btn_state = 0;

int val_arm_btn_state = 0;
int val_launch_btn_state = 0;
int val_abort_btn_state = 0;
int val_rcs_btn_state = 0;
int val_joy_r_btn_state = 0;
int val_joy_l_btn_state = 0;

float pitch = 0;
float yaw   = 0;
float roll  = 0;

float power = 0;

float x = 0;
float y = 0;
float z = 0;

void setup() {
  //analog input ref
  pinMode(ref_joy_l_y, INPUT);
  pinMode(ref_joy_l_x, INPUT);
  pinMode(ref_joy_l_z, INPUT);
  pinMode(ref_slider , INPUT);

  //digital input ref
  pinMode(ref_stage_btn, INPUT);
  pinMode(ref_sas_btn  , INPUT);
  pinMode(ref_next_btn , INPUT);

  pinMode(ref_arm_btn   , INPUT);
  pinMode(ref_launch_btn, INPUT);
  pinMode(ref_abort_btn , INPUT);
  pinMode(ref_rcs_btn   , INPUT);

  pinMode(ref_full_btn , INPUT);
  pinMode(ref_nomi_btn , INPUT);
  pinMode(ref_cut_btn  , INPUT);

  pinMode(ref_light_btn , INPUT);
  pinMode(ref_ladder_btn , INPUT);
  pinMode(ref_joy_r_btn  , INPUT);
  pinMode(ref_joy_l_btn  , INPUT);

  pinMode(ref_gears_btn , INPUT);
  pinMode(ref_break_btn , INPUT);
  pinMode(ref_solar_btn  , INPUT);
  pinMode(ref_chute_btn  , INPUT);

  //output ref
  pinMode(ref_stage_led, OUTPUT);
  pinMode(ref_sas_led  , OUTPUT);
  pinMode(ref_next_led , OUTPUT);

  pinMode(ref_arm_led   , OUTPUT);
  pinMode(ref_launch_led, OUTPUT);
  pinMode(ref_abort_led , OUTPUT);
  pinMode(ref_rcs_led   , OUTPUT);

  lcd.init(); // initialize the lcd
  lcd.backlight();

  Serial.begin(9600);
}

void loop() {

  now = millis();

  val_joy_l_y = analogRead(ref_joy_l_y);
  val_joy_l_x = analogRead(ref_joy_l_x);
  val_joy_l_z = analogRead(ref_joy_l_z);

  val_slider  = analogRead(ref_slider);

  val_joy_r_y = analogRead(ref_joy_r_y);
  val_joy_r_x = analogRead(ref_joy_r_x);
  val_joy_r_z = analogRead(ref_joy_r_z);

  pitch = map( val_joy_l_y, 0, 1023, 100, -100);
  yaw   = map( val_joy_l_x, 0, 1023, 100, -100);
  roll  = map( val_joy_l_z, 0, 1023, 100, -100);

  power = map( val_slider, 373, 1023, 100, 0);

  x = map( val_joy_r_x, 0, 1023, 100, -100);
  y = map( val_joy_r_y, 0, 1023, 100, -100);
  z = map( val_joy_r_z, 0, 1023, 100, -100);

  val_stage_btn = digitalRead(ref_stage_btn);
  val_sas_btn   = digitalRead(ref_sas_btn);
  val_next_btn  = digitalRead(ref_next_btn);

  val_arm_btn    = digitalRead(ref_arm_btn);
  val_launch_btn = digitalRead(ref_launch_btn);
  val_abort_btn  = digitalRead(ref_abort_btn);
  val_rcs_btn    = digitalRead(ref_rcs_btn);

  val_full_btn = digitalRead(ref_full_btn);
  val_nomi_btn = digitalRead(ref_nomi_btn);
  val_cut_btn  = digitalRead(ref_cut_btn);

  val_light_btn = digitalRead(ref_light_btn);
  val_ladder_btn = digitalRead(ref_ladder_btn);
  //erro cablage on light and ladder so inverse reading
  val_light_btn = val_light_btn == HIGH ? LOW : HIGH;
  val_ladder_btn = val_ladder_btn == HIGH ? LOW : HIGH;

  val_joy_r_btn = digitalRead(ref_joy_r_btn);
  val_joy_l_btn = digitalRead(ref_joy_l_btn);

  val_gears_btn = digitalRead(ref_gears_btn);
  val_break_btn = digitalRead(ref_break_btn);
  val_solar_btn = digitalRead(ref_solar_btn);
  val_chute_btn = digitalRead(ref_chute_btn);

  char tmp_key = keypad.getKey();
  if (NO_KEY != tmp_key) {
    val_maneuver = tmp_key;
  }

  /*
    Serial.print("pitch  = " );
    Serial.print(pitch);
    Serial.print("  yaw = " );
    Serial.print(yaw);
    Serial.print("  roll = " );
    Serial.print(roll);
    Serial.print("  power = " );
    Serial.print(power);
    Serial.print("  stage = " );
    Serial.print(val_stage_btn);
    Serial.print("  sas = " );
    Serial.print(val_sas_btn);
    Serial.print("  next = " );
    Serial.print(val_next_btn);

    Serial.print(" x  = " );
    Serial.print(x);
    Serial.print("  y = " );
    Serial.print(y);
    Serial.print("  z = " );
    Serial.print(z);
    Serial.print("  key = " );

    Serial.print(" val_arm_btn  = " );
    Serial.print(val_arm_btn);
    Serial.print("  val_launch_btn = " );
    Serial.print(val_launch_btn);
    Serial.print("  val_abort_btn = " );
    Serial.print(val_abort_btn);
    Serial.print("  val_rcs_btn = " );
    Serial.print(val_rcs_btn);
    Serial.print("  val_full_btn = " );
    Serial.print(val_full_btn);
    Serial.print("  val_nomi_btn = " );
    Serial.print(val_nomi_btn);
    Serial.print("  val_cut_btn = " );
    Serial.println(val_cut_btn);

  */

  Serial.print(" val_gears_btn  = " );
  Serial.print(val_gears_btn);
  Serial.print("  val_break_btn = " );
  Serial.print(val_break_btn);
  Serial.print("  val_solar_btn = " );
  Serial.print(val_solar_btn);
  Serial.print("  val_chute_btn = " );
  Serial.print(val_chute_btn);
  Serial.print("  val_joy_l_btn_state = " );
  Serial.println(val_joy_l_btn_state);

  if (val_stage_btn == 1 && now - val_stage_btn_timer > BTTIMELATENCY ) {
    val_stage_btn_timer = now;
    val_stage_btn_state = val_stage_btn_state == 1 ? 0 : 1;
  }

  if (val_sas_btn == 1 && now - val_sas_btn_timer > BTTIMELATENCY ) {
    val_sas_btn_timer = now;
    val_sas_btn_state = val_sas_btn_state == 1 ? 0 : 1;
  }

  if (val_next_btn == 1 && now - val_next_btn_timer > BTTIMELATENCY ) {
    val_next_btn_timer = now;
    val_next_btn_state = val_next_btn_state == 1 ? 0 : 1;
    val_display_state = (val_display_state + 1) % NB_DISPLAY_STATE;
    clearLCD();
  }

  if (val_abort_btn == 1 && now - val_abort_btn_timer > BTTIMELATENCY ) {
    val_abort_btn_timer = now;
    val_abort_btn_state = val_abort_btn_state == 1 ? 0 : 1;
  }

  if (val_rcs_btn == 1 && now - val_rcs_btn_timer > BTTIMELATENCY ) {
    val_rcs_btn_timer = now;
    val_rcs_btn_state = val_rcs_btn_state == 1 ? 0 : 1;
  }

  if (val_joy_r_btn == 1 && now - val_joy_r_btn_timer > BTTIMELATENCY ) {
    val_joy_r_btn_timer = now;
    val_joy_r_btn_state = val_joy_r_btn_state == 1 ? 0 : 1;
  }

  if (val_joy_l_btn == 1 && now - val_joy_l_btn_timer > BTTIMELATENCY ) {
    val_joy_l_btn_timer = now;
    val_joy_l_btn_state = val_joy_l_btn_state == 1 ? 0 : 1;
  }

  digitalWrite(ref_stage_led, val_stage_btn_state);
  digitalWrite(ref_sas_led  , val_sas_btn_state);
  digitalWrite(ref_next_led , val_next_btn_state);

  digitalWrite(ref_abort_led , val_abort_btn_state);
  digitalWrite(ref_rcs_led , val_rcs_btn_state);

  digitalWrite(ref_arm_led , val_arm_btn);
  digitalWrite(ref_launch_led , val_launch_btn);

  displayLCD();

  delay(100);
}

//return a string on 4 char
//use to display a value between -100 to 100 on lcd
String displayFloatOn3Char ( float val ) {
  String res = String(val, 0);

  if (res.length() == 1 ) {
    res = res + " ";
  }

  if (res.length() == 2 ) {
    res = res + " ";
  }
  if (res.length() == 3 ) {
    res = res + " ";
  }
  return res;
}

void clearLCD() {
  lcd.setCursor(0, 0);
  lcd.print("                ");
  lcd.setCursor(0, 1);
  lcd.print("                ");
}

void displayLCD() {
  // display function
  if (val_display_state == 0) {

  }


  switch (val_display_state) {
    case 0:
      lcd.setCursor(0, 0);
      lcd.print("Kerbal Space");
      lcd.setCursor(0, 1);
      lcd.print("Controler  v 0.1");
      break;
    case 1:
      lcd.setCursor(0, 0);
      lcd.print("pi ");
      lcd.print(displayFloatOn3Char(pitch));
      lcd.print(" ya ");
      lcd.print(displayFloatOn3Char(yaw));
      lcd.setCursor(0, 1);
      lcd.print("ro ");
      lcd.print(displayFloatOn3Char(roll));
      lcd.print(" po ");
      lcd.print(displayFloatOn3Char(power));
      break;
    case 2:
      lcd.setCursor(0, 0);
      lcd.print("x ");
      lcd.print(displayFloatOn3Char(x));
      lcd.print(" y ");
      lcd.print(displayFloatOn3Char(y));
      lcd.setCursor(0, 1);
      lcd.print("z ");
      lcd.print(displayFloatOn3Char(z));
      break;
    case 3:
      lcd.setCursor(0, 0);
      lcd.print("maneuver :");
      lcd.setCursor(0, 1);
      lcd.print(val_maneuver);
      break;
    case 4:
      lcd.setCursor(0, 0);
      lcd.print("Power :");
      lcd.setCursor(0, 1);
      if (val_full_btn == 1) {
        lcd.print("Full   ");
      } else if (val_nomi_btn == 1) {
        lcd.print("Nominal");
      } else if (val_cut_btn == 1) {
        lcd.print("Cut    ");
      } else {
        lcd.print("ERROR");
      }
      break;
    case 5:
      lcd.setCursor(0, 0);
      lcd.print("Btn joy left :");
      lcd.print(val_joy_l_btn_state);
      lcd.setCursor(0, 1);
      lcd.print("Btn joy right :");
      lcd.print(val_joy_r_btn_state);
      break;
    case 6:
      lcd.setCursor(0, 0);
      lcd.print("li ");
      lcd.print(val_light_btn);
      lcd.print(" la ");
      lcd.print(val_ladder_btn);
      lcd.print(" ge ");
      lcd.print(val_gears_btn);
      lcd.setCursor(0, 1);
      lcd.print("br ");
      lcd.print(val_break_btn);
      lcd.print(" so ");
      lcd.print(val_solar_btn);
      lcd.print(" ge ");
      lcd.print(val_chute_btn);
      break;
    default:
      lcd.setCursor(0, 0);
      lcd.print("ERROR STATE");
  }


}
