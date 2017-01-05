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

byte str2data(char* str, char command_delimiter, char data_delimiter, char mode_delimiter) {
    // подготовка
    char* _command_delimiter = char2ptr(command_delimiter);
    char* _data_delimiter = char2ptr(data_delimiter);

    unsigned int* data_array = new unsigned int[128];
    byte _counter = 0;

    // работа
    char* _work_string = strchr(str, _command_delimiter);

    while (NULL != _work_string) {
      _work_string += 1;
      
      char* _piece = getPieceBefore(_work_string, _data_delimiter, false);
        showString(_piece, "_piece");
        
      data_array[_counter] = (unsigned int)atoi(_piece);
        Serial.print("data_array piece: "); Serial.println(data_array[_counter]);
        
      _counter++;
      delete _piece;
      
      _work_string = strchr(_work_string, _data_delimiter);
        showString(_work_string, "_work_string", true);
    }

    delete _command_delimiter;
    delete _data_delimiter;
    delete _work_string;
}

void setup() {
  Serial.begin(9600);
  Serial.println("=strchr-test=");

    showMem();
    
    char* input_str = str2ptr("123|1_22_333_444");
      
    byte cmd = getPieceBefore(input_str,'|');
      showString(cmd, "cmd");
      
    unsigned int* data = str2data(input_str, '|', '_', ':');
//    Serial.print(F("data array: "));
//    for (byte i=0; i<counter; i++) {
//      Serial.print(data_array[i]); Serial.print(F(", "));
//    } Serial.println(F(""));
//    delete data_array;

    delete input_str;
    
    showMem();
}

void loop() {

}

