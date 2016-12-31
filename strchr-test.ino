#include <MemoryFree.h>

void showMem(){
  Serial.print("freeMemory()=");
  Serial.println(freeMemory());
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

void showString(char* str, char* prefix = NULL) {
  if (NULL!=prefix) {
    Serial.print(prefix);
    Serial.print(": ");
  }
  Serial.print(str); Serial.print(" (len: "); Serial.print(strlen(str)); Serial.println(")");
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
    
    char* str = str2ptr("123|aaa_bbb_ccc_ddd");
    showString(str, "orig string");
    char* next_start = str;

    char delimiter = '|';
    char* piece = getPieceBefore(next_start, delimiter, true);
    delete piece;
    next_start = strchr(str, delimiter);

    while (NULL != next_start) {
      next_start += 1;
      showString(next_start, "next_start");
      
      char* piece = getPieceBefore(next_start, delimiter, true);
      delete piece;
      
      next_start = strchr(next_start, '_');
    }
    
    delete str;
    showMem();
    Serial.println("");
}

void loop() {

}
