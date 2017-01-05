#include <MemoryFree.h>

void showMem(){
  Serial.println(F(""));
  Serial.print(F("freeMemory()="));
  Serial.println(freeMemory());
  Serial.println(F(""));
}

char* char2ptr(char letter) {
  char* res = new char[2];
        res[0] = letter;
        res[1] = char(0);
  return res;
}

char* str2ptr(char* str) {
  unsigned int len = strlen(str);
  char* res = new char[len+1];
  for (int i=0; i<len; i++) res[i] = str[i];
  res[len] = char(0);
  return res;
}

void showString(char* str, char* prefix = NULL, bool newLine=false) {
  if (NULL!=prefix) {
    Serial.print(prefix);
    Serial.print(": ");
  }
  Serial.print(str); Serial.print(" (len: "); Serial.print(strlen(str)); Serial.println(")");
  if (newLine) Serial.println(F(""));
}

char* getPieceBefore(char* str, char delimiter, bool debug=false) {
  char* arr_delimiter = char2ptr(delimiter);
  if (debug) {
    Serial.print(F("arr_delimiter: ")); Serial.println(arr_delimiter);
  }
  
  byte len = strcspn(str, arr_delimiter);
  if (debug) {
    Serial.print(F("piece len: ")); Serial.println(len);
  }
  
  delete arr_delimiter;

  char* piece = new char[len + 1]; // +1 для нулевого символа
  strncpy(piece, str, len);
  piece[len] = char(0);
  if (debug) {
    Serial.print(F("piece: ")); Serial.println(piece);
  }
  
  return piece;
}


void setup() {
  Serial.begin(9600);
  Serial.println("=strchr-test=");

    showMem();
    
    //char* str = str2ptr("123|aaa_bbb_ccc_ddd");
    char* str = str2ptr("123|1_22_333_444");
    char command_delimiter = '|';
    char data_delimiter = '_';

    unsigned int* data_array = new unsigned int[128];
    byte counter = 0;

    char* work_string = str;
    
    char* piece = getPieceBefore(work_string, command_delimiter, false);
      showString(piece, "PIECE");
      showString(str, "orig string");
    data_array[counter] = (unsigned int)atoi(piece);
      Serial.print("data_array piece: "); Serial.println(data_array[counter]);
    counter++;
    delete piece;
    work_string = strchr(str, command_delimiter);
      showString(work_string, "work_string", true);

    while (NULL != work_string) {
      work_string += 1;
      char* piece = getPieceBefore(work_string, data_delimiter, false);
        showString(piece, "PIECE");
        showString(str, "orig string");
      data_array[counter] = (unsigned int)atoi(piece);
        Serial.print("data_array piece: "); Serial.println(data_array[counter]);
      counter++;
      delete piece;
      work_string = strchr(work_string, data_delimiter);
        showString(work_string, "work_string", true);
        
        if (1==strlen(work_string)) {
          Serial.print(F("[[ work_string: |"));
          Serial.print(work_string);
          Serial.print(F("| (char "));
          Serial.print(byte(work_string));
          Serial.println(F(") ]]"));
        }
    }
    
    delete str;
    
    Serial.print(F("data array: "));
    for (byte i=0; i<counter; i++) {
      Serial.print(data_array[i]); Serial.print(F(", "));
    } Serial.println(F(""));
    delete data_array;
    
    showMem();
}

void loop() {

}

