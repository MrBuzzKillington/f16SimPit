

void setupWarning()
{
    pinMode(COL_1, OUTPUT);
    pinMode(COL_2, OUTPUT);
    pinMode(COL_3, OUTPUT);
    pinMode(COL_4, OUTPUT);

    pinMode(ROW_1, OUTPUT);
    pinMode(ROW_2, OUTPUT);
    pinMode(ROW_3, OUTPUT);
    pinMode(ROW_4, OUTPUT);
    pinMode(ROW_5, OUTPUT);
    pinMode(ROW_6, OUTPUT);
    pinMode(ROW_7, OUTPUT);
    pinMode(ROW_8, OUTPUT);



    digitalWrite(COL_1, LOW); //Light the Column
    digitalWrite(COL_2, LOW); //Light the Column
    digitalWrite(COL_3, LOW); //Light the Column
    digitalWrite(COL_4, LOW); //Light the Column

    digitalWrite(ROW_1, HIGH); //ROW
    digitalWrite(ROW_2, HIGH); //ROW
    digitalWrite(ROW_3, HIGH); //ROW
    digitalWrite(ROW_4, HIGH); //ROW
    digitalWrite(ROW_5, HIGH); //ROW
    digitalWrite(ROW_6, HIGH); //ROW
    digitalWrite(ROW_7, HIGH); //ROW
    digitalWrite(ROW_8, HIGH); //ROW    

    testPattern();
}

//Function to draw current state
void  drawScreen(byte buffer2[])
{
    // Turn on each Column in series
    for (byte i = 0; i < 4; i++) 
    {
        setRows(buffer2[i]); // Set columns for this specific row
        
        digitalWrite(ledCols_[i], HIGH); //Light the Column
        delay(5); // Set this to 50 or 100 if you want to see the multiplexing effect!
        digitalWrite(ledCols_[i], LOW);       
    }
}

//Cathod 0=ON
void setRows(byte b) 
{
    digitalWrite(ROW_1, (~b >> 0) & 0x01); // Get the 1st bit: 10000000
    digitalWrite(ROW_2, (~b >> 1) & 0x01); // Get the 2nd bit: 01000000
    digitalWrite(ROW_3, (~b >> 2) & 0x01); // Get the 3rd bit: 00100000
    digitalWrite(ROW_4, (~b >> 3) & 0x01); // Get the 4th bit: 00010000
    digitalWrite(ROW_5, (~b >> 4) & 0x01); // Get the 5th bit: 00001000
    digitalWrite(ROW_6, (~b >> 5) & 0x01); // Get the 6th bit: 00000100
    digitalWrite(ROW_7, (~b >> 6) & 0x01); // Get the 7th bit: 00000010
    digitalWrite(ROW_8, (~b >> 7) & 0x01); // Get the 8th bit: 00000001
   
    // If the polarity of your matrix is the opposite of mine
    // remove all the '~' above.
}

void testPattern()
{

  byte RowByte = B00000001;
  byte colByte = B00000001;
  //Walk a 1 across rows
  for (int i = 0; i<4; i++)
  {
    digitalWrite(ledCols_[i], HIGH); //Light the Column
    RowByte = B00000001;
    for (int ii =0; ii<8; ii++)
    {
        setRows(RowByte);
        RowByte = RowByte << 1;
        delay(100); // Set this to 50 or 100 if you want to see the multiplexing effect!
    }
    digitalWrite(ledCols_[i], LOW);        
    colByte = colByte << 1;
  }

  drawScreen(AllLedOff);
}


/*
 * 
 * Code to Parse the states ffrom DCS
 * 
 */


void onLightSeatNotChange(unsigned int newValue) {
    if (newValue == 1)
    {
      CurLedState_[3] = CurLedState_[3] | 0x01;
    } else {
      CurLedState_[3] = CurLedState_[3] & 0xFE;
    }
}
DcsBios::IntegerBuffer lightSeatNotBuffer(0x4476, 0x0008, 3, onLightSeatNotChange);

void onLightNwsFailChange(unsigned int newValue) {
    if (newValue == 1)
    {
      CurLedState_[3] = CurLedState_[3] | 0x02;
    } else {
      CurLedState_[3] = CurLedState_[3] & 0xFD;
    }
}
DcsBios::IntegerBuffer lightNwsFailBuffer(0x4476, 0x0080, 7, onLightNwsFailChange);


void onLightAntiSkidChange(unsigned int newValue) {
    if (newValue == 1)
    {
      CurLedState_[3] = CurLedState_[3] | 0x04;
    } else {
      CurLedState_[3] = CurLedState_[3] & 0xFB;
    }
}

DcsBios::IntegerBuffer lightAntiSkidBuffer(0x4476, 0x0800, 11, onLightAntiSkidChange);

void onLightHookChange(unsigned int newValue) {
    if (newValue == 1)
    {
      CurLedState_[3] = CurLedState_[3] | 0x08;
    } else {
      CurLedState_[3] = CurLedState_[3] & 0xF7;
    }
}

DcsBios::IntegerBuffer lightHookBuffer(0x4476, 0x8000, 15, onLightHookChange);


void onLightObogsChange(unsigned int newValue) {
    if (newValue == 1)
    {
      CurLedState_[3] = CurLedState_[3] | 0x10;
    } else {
      CurLedState_[3] = CurLedState_[3] & 0xEF;
    }
}
DcsBios::IntegerBuffer lightObogsBuffer(0x4478, 0x0008, 3, onLightObogsChange);

void onLightCabinPressChange(unsigned int newValue) {
    if (newValue == 1)
    {
      CurLedState_[3] = CurLedState_[3] | 0x20;
    } else {
      CurLedState_[3] = CurLedState_[3] & 0xDF;
    }
}
DcsBios::IntegerBuffer lightCabinPressBuffer(0x4478, 0x0080, 7, onLightCabinPressChange);

//Col2
void onLightAvionicsFaultChange(unsigned int newValue) {
    if (newValue == 1)
    {
      CurLedState_[2] = CurLedState_[2] | 0x01;
    } else {
      CurLedState_[2] = CurLedState_[2] & 0xFE;
    }
}
DcsBios::IntegerBuffer lightAvionicsFaultBuffer(0x4476, 0x0004, 2, onLightAvionicsFaultChange);


void onLightEquipHotChange(unsigned int newValue) {
    if (newValue == 1)
    {
      CurLedState_[2] = CurLedState_[2] | 0x02;
    } else {
      CurLedState_[2] = CurLedState_[2] & 0xFD;
    }
}
DcsBios::IntegerBuffer lightEquipHotBuffer(0x4476, 0x0040, 6, onLightEquipHotChange);

void onLightRadarAltChange(unsigned int newValue) {
    if (newValue == 1)
    {
      CurLedState_[2] = CurLedState_[2] | 0x04;
    } else {
      CurLedState_[2] = CurLedState_[2] & 0xFB;
    }
}
DcsBios::IntegerBuffer lightRadarAltBuffer(0x4476, 0x0400, 10, onLightRadarAltChange);

void onLightIffChange(unsigned int newValue) {
    if (newValue == 1)
    {
      CurLedState_[2] = CurLedState_[2] | 0x08;
    } else {
      CurLedState_[2] = CurLedState_[2] & 0xF7;
    }
}
DcsBios::IntegerBuffer lightIffBuffer(0x4476, 0x4000, 14, onLightIffChange);

void onLightNuclearChange(unsigned int newValue) {
    if (newValue == 1)
    {
      CurLedState_[2] = CurLedState_[2] | 0x10;
    } else {
      CurLedState_[2] = CurLedState_[2] & 0xEF;
    }
}
DcsBios::IntegerBuffer lightNuclearBuffer(0x4478, 0x0004, 2, onLightNuclearChange);

//COL1
void onLightEngineFaultChange(unsigned int newValue) {
    if (newValue == 1)
    {
      CurLedState_[1] = CurLedState_[1] | 0x01;
    } else {
      CurLedState_[1] = CurLedState_[1] & 0xFE;
    }
}
DcsBios::IntegerBuffer lightEngineFaultBuffer(0x4476, 0x0002, 1, onLightEngineFaultChange);

void onLightSecChange(unsigned int newValue) {
    if (newValue == 1)
    {
      CurLedState_[1] = CurLedState_[1] | 0x02;
    } else {
      CurLedState_[1] = CurLedState_[1] & 0xFD;
    }
}
DcsBios::IntegerBuffer lightSecBuffer(0x4476, 0x0020, 5, onLightSecChange);

void onLightFuelOilHotChange(unsigned int newValue) {
    if (newValue == 1)
    {
      CurLedState_[1] = CurLedState_[1] | 0x04;
    } else {
      CurLedState_[1] = CurLedState_[1] & 0xFB;
    }
}
DcsBios::IntegerBuffer lightFuelOilHotBuffer(0x4476, 0x0200, 9, onLightFuelOilHotChange);

void onLightInletIcingChange(unsigned int newValue) {
    if (newValue == 1)
    {
      CurLedState_[1] = CurLedState_[1] | 0x08;
    } else {
      CurLedState_[1] = CurLedState_[1] & 0xF7;
    }
}
DcsBios::IntegerBuffer lightInletIcingBuffer(0x4476, 0x2000, 13, onLightInletIcingChange);

void onLightOverheatChange(unsigned int newValue) {
    if (newValue == 1)
    {
      CurLedState_[1] = CurLedState_[1] | 0x10;
    } else {
      CurLedState_[1] = CurLedState_[1] & 0xEF;
    }
}
DcsBios::IntegerBuffer lightOverheatBuffer(0x4478, 0x0002, 1, onLightOverheatChange);

void onLightEecChange(unsigned int newValue) {
    if (newValue == 1)
    {
      CurLedState_[1] = CurLedState_[1] | 0x20;
    } else {
      CurLedState_[1] = CurLedState_[1] & 0xDF;
    }
}
DcsBios::IntegerBuffer lightEecBuffer(0x4478, 0x0020, 5, onLightEecChange);


void onLightBucChange(unsigned int newValue) {
    if (newValue == 1)
    {
      CurLedState_[1] = CurLedState_[1] | 0x40;
    } else {
      CurLedState_[1] = CurLedState_[1] & 0xBF;
    }
}
DcsBios::IntegerBuffer lightBucBuffer(0x4478, 0x0200, 9, onLightBucChange);

//Column 0

void onLightFlcsFaultChange(unsigned int newValue) {
    if (newValue == 1)
    {
      CurLedState_[0] = CurLedState_[0] | 0x01;
    } else {
      CurLedState_[0] = CurLedState_[0] & 0xFE;
    }
}
DcsBios::IntegerBuffer lightFlcsFaultBuffer(0x4476, 0x0001, 0, onLightFlcsFaultChange);


void onLightElecSysChange(unsigned int newValue) {
    if (newValue == 1)
    {
      CurLedState_[0] = CurLedState_[0] | 0x02;
    } else {
      CurLedState_[0] = CurLedState_[0] & 0xFD;
    }
}
DcsBios::IntegerBuffer lightElecSysBuffer(0x4476, 0x0010, 4, onLightElecSysChange);

void onLightProbeHeatChange(unsigned int newValue) {
    if (newValue == 1)
    {
      CurLedState_[0] = CurLedState_[0] | 0x04;
    } else {
      CurLedState_[0] = CurLedState_[0] & 0xFB;
    }
}
DcsBios::IntegerBuffer lightProbeHeatBuffer(0x4476, 0x0100, 8, onLightProbeHeatChange);

void onLightCadcChange(unsigned int newValue) {
    if (newValue == 1)
    {
      CurLedState_[0] = CurLedState_[0] | 0x08;
    } else {
      CurLedState_[0] = CurLedState_[0] & 0xF7;
    }
}
DcsBios::IntegerBuffer lightCadcBuffer(0x4476, 0x1000, 12, onLightCadcChange);

void onLightStoresConfigChange(unsigned int newValue) {
    if (newValue == 1)
    {
      CurLedState_[0] = CurLedState_[0] | 0x10;
    } else {
      CurLedState_[0] = CurLedState_[0] & 0xEF;
    }
}
DcsBios::IntegerBuffer lightStoresConfigBuffer(0x4478, 0x0001, 0, onLightStoresConfigChange);

void onLightAtfNotChange(unsigned int newValue) {
    if (newValue == 1)
    {
      CurLedState_[0] = CurLedState_[0] | 0x20;
    } else {
      CurLedState_[0] = CurLedState_[0] & 0xDF;
    }
}
DcsBios::IntegerBuffer lightAtfNotBuffer(0x4478, 0x0010, 4, onLightAtfNotChange);

void onLightFwdFuelLowChange(unsigned int newValue) {
    if (newValue == 1)
    {
      CurLedState_[0] = CurLedState_[0] | 0x40;
    } else {
      CurLedState_[0] = CurLedState_[0] & 0xBF;
    }
}
DcsBios::IntegerBuffer lightFwdFuelLowBuffer(0x4478, 0x0100, 8, onLightFwdFuelLowChange);


void onLightAftFuelLowChange(unsigned int newValue) {
    if (newValue == 1)
    {
      CurLedState_[0] = CurLedState_[0] | 0x80;
    } else {
      CurLedState_[0] = CurLedState_[0] & 0x7F;
    }
}
DcsBios::IntegerBuffer lightAftFuelLowBuffer(0x4478, 0x1000, 12, onLightAftFuelLowChange);
