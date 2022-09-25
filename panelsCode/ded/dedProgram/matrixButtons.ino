/*
 * 
 *   Keyboard stuff
 * 
 */
bool keyPressedLast_ = false;
char lastKey_ = 0x00;
const byte ROWS = 5; //five rows
const byte COLS = 5; //five columns
char keys[ROWS][COLS] = 
{
  {11,12,13,14,15},
  {21,22,23,24,25},
  {31,32,33,34,35},
  {41,42,43,44,45},
  {51,52,53,54,55}
};

//byte rowPins[ROWS] = {53, 23,51,25,49}; //connect to the row pinouts of the keypad  
byte rowPins[ROWS] = {37, 23,39,25,49}; //connect to the row pinouts of the keypad  39was 51
byte colPins[COLS] = {27,47,29,45,31}; //connect to the column pinouts of the keypad
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );



void processMatrixKeys()
{
  char key = keypad.getKey();

  if (key != NO_KEY)
  {
          keyPressedLast_ = true;
          processButton(key, 11, "ICP_COM1_BTN", "1");
          processButton(key, 12, "ICP_COM2_BTN", "1");
          processButton(key, 13, "ICP_IFF_BTN", "1");
          processButton(key, 14, "ICP_LIST_BTN", "1");
          processButton(key, 15, "ICP_AA_MODE_BTN", "1");
          processButton(key, 21, "ICP_BTN_1", "1");
          processButton(key, 22, "ICP_BTN_2", "1");
          processButton(key, 23, "ICP_BTN_3", "1");
          processButton(key, 24, "ICP_RCL_BTN", "1");
          processButton(key, 25, "ICP_AG_MODE_BTN", "1");
          processButton(key, 31, "ICP_BTN_4", "1");
          processButton(key, 32, "ICP_BTN_5", "1");
          processButton(key, 33, "ICP_BTN_6", "1");
          processButton(key, 34, "ICP_ENTR_BTN", "1");
          processButton(key, 35, "ICP_WX_BTN", "1");
          processButton(key, 41, "ICP_BTN_7", "1");
          processButton(key, 42, "ICP_BTN_8", "1");
          processButton(key, 43, "ICP_BTN_9", "1");
          processButton(key, 44, "ICP_BTN_0", "1");
          processButton(key, 45, "ICP_FLIR_SW", "2");
          processButton(key, 51, "ICP_DED_SW", "2");
          processButton(key, 52, "ICP_DED_SW", "0");
          processButton(key, 53, "ICP_BTN_9", "1");
          processButton(key, 54, "ICP_BTN_0", "1");
          processButton(key, 55, "ICP_FLIR_SW", "0");         
     
  } else {
     if (keyPressedLast_ == true)
     {
        if (keypad.isPressed(lastKey_) || keypad.isHeld(lastKey_) )
        {
          //do something
        } else {
           clearButtons();
           keyPressedLast_ = false;        
        }

     }
  }
}

//This function checks if the button is pressed and send bios the comand
void processButton(char keySeen, char check, const char* biosSignal, const char* biosCmd)
{
  if (keySeen == check)
  {
       sendDcsBiosMessage(biosSignal, biosCmd);
       lastKey_ = check;
  }
}



void clearButtons()
{
     sendDcsBiosMessage("ICP_COM1_BTN", "0");
     sendDcsBiosMessage("ICP_COM2_BTN", "0");
     sendDcsBiosMessage("ICP_IFF_BTN", "0");
     sendDcsBiosMessage("ICP_LIST_BTN", "0");
     sendDcsBiosMessage("ICP_AA_MODE_BTN", "0");
     sendDcsBiosMessage("ICP_AG_MODE_BTN","0");
     sendDcsBiosMessage("ICP_ENTR_BTN","0");
     sendDcsBiosMessage("ICP_RCL_BTN","0");
     sendDcsBiosMessage("ICP_WX_BTN","0");
     sendDcsBiosMessage("ICP_BTN_1","0");
     sendDcsBiosMessage("ICP_BTN_2","0");
     sendDcsBiosMessage("ICP_BTN_3","0");
     sendDcsBiosMessage("ICP_BTN_4","0");
     sendDcsBiosMessage("ICP_BTN_5","0");
     sendDcsBiosMessage("ICP_BTN_6","0");
     sendDcsBiosMessage("ICP_BTN_7","0");
     sendDcsBiosMessage("ICP_BTN_8","0");
     sendDcsBiosMessage("ICP_BTN_9","0");
     sendDcsBiosMessage("ICP_BTN_0","0");
     sendDcsBiosMessage("ICP_FLIR_SW","1");
     sendDcsBiosMessage("ICP_DED_SW","1");   
}
