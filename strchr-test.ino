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

byte str2cmd(char* str, char command_delimiter) {
  char* cmd_piece = getPieceBefore(str,command_delimiter);
    byte cmd = (byte)atoi(cmd_piece);
    delete cmd_piece;
  return cmd;
}

unsigned int* str2data(char* str, char command_delimiter, char data_delimiter, char mode_delimiter) {
    // подготовка
    unsigned int* data_array = new unsigned int[128];
    byte counter = 0;

    // работа
    char* work_string = strchr(str, command_delimiter);

    while (NULL != work_string) {
      work_string += 1;
      
      char* _piece = getPieceBefore(work_string, data_delimiter, false);
        showString(_piece, "_piece");
        
      data_array[counter] = (unsigned int)atoi(_piece);
        Serial.print("data_array[counter]: "); Serial.println(data_array[counter]);
        
      counter++;
      delete _piece;
      
      work_string = strchr(work_string, data_delimiter);
        showString(work_string, "work_string", true);
    }

    delete work_string;

    return data_array;
}

void setup() {
  Serial.begin(9600);
  Serial.println("=strchr-test=");

    showMem();
    
    char* input_str = str2ptr("123|1_22_333_444");

    byte cmd = str2cmd(input_str, '|');
       Serial.print(F("cmd: ")); Serial.println(cmd);
       
    unsigned int* data = str2data(input_str, '|', '_', ':');
    Serial.print(F("data: "));
    for (byte i=0; i<4; i++) {
      Serial.print(data[i]); Serial.print(F(", "));
    } Serial.println(F(""));

    delete data;
    delete input_str;
    
    showMem();
}

void loop() {

}

