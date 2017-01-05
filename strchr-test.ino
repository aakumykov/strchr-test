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


void setup() {
  Serial.begin(9600);
  Serial.println("=strchr-test=");

    showMem();
    
    char* input_str = str2ptr("123|1_22_333_444");

    delete input_str;
    
    showMem();
}

void loop() {

}

class CmdParser {
  public:
    CmdParser(char command_delimiter, char data_delimiter, char mode_delimiter) {
      this->_command_delimiter = command_delimiter;
      this->_data_delimiter = data_delimiter;
      this->_mode_delimiter = mode_delimiter;
    }
    
    byte cmd() {
      return this->_cmd;
    }
    byte count() {
      return this->_counter;
    }
    byte length() {
      return this->count();
    }
    unsigned int* data() {
      return this->_data;
    }
    void clear() {
      
    }
    byte parse(char* str) {
      this->_cmd = this->str2cmd(str);
      this->_data = this->str2data(str);
    }
    
  private:
    // полезные данные
    byte _cmd;
    unsigned int* _data = new unsigned int[128];
    byte _counter = 0;

    // служебные данные
    char _command_delimiter;
    char _data_delimiter;
    char _mode_delimiter;

    // полезные методы
    byte str2cmd(char* str) {
      char* cmd_piece = getPieceBefore(str,this->_command_delimiter);
      this->_cmd = (byte)atoi(cmd_piece);
      delete cmd_piece;
    }

    unsigned int* str2data(char* str) {
        char* work_string = strchr(str, this->_command_delimiter);
    
        while (NULL != work_string) {
          work_string += 1;
          
          char* piece = getPieceBefore(work_string, this->_data_delimiter, false);
            showString(piece, "piece");
            
          this->_data[this->_counter] = (unsigned int)atoi(piece);
          delete piece;
          
          this->_counter++;
          
          work_string = strchr(work_string, this->_data_delimiter);
            showString(work_string, "work_string", true);
        }
    
        delete work_string;
    }
    
    // служебные методы
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

    
};

