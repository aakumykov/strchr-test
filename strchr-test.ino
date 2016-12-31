#include <MemoryFree.h>

void showMem(){
  Serial.print("freeMemory()=");
  Serial.println(freeMemory());
}

char* str2ptr(char* str) {
  unsigned int len = strlen(str);
  char* res = new char[len+1];
  for (int i=0; i<len; i++) res[i] = str[i];
  res[len] = char(0);
  return res;
}

void showStr(char* str, char* prefix = NULL) {
  if (NULL!=prefix) {
    Serial.print(prefix);
    Serial.print(": ");
  }
  Serial.print(str); Serial.print(" ("); Serial.print(strlen(str)); Serial.println(")");
}

void setup() {
  Serial.begin(9600);
  Serial.println("=strchr-test=");

    showMem();
    
    char* str = str2ptr("123|aaa_bbb_ccc_ddd");
    showStr(str, "orig string");
    char* next_start = str;
    
    next_start = strchr(str, '|') + 1;
    showStr(next_start, "next_start");

    while (NULL != next_start) {
      next_start += 1;
      showStr(next_start, "next_start");
      next_start = strchr(next_start, '_');
    }
    
    delete str;
    showMem();
    Serial.println("");
}

void loop() {

}
