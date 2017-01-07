#include <SerialListener.h>
#include <MemoryFree.h>

void showMem(char* comment=NULL){
  //Serial.println(F(""));
  Serial.print(F("Free memory"));
  if (NULL != comment)  {
    Serial.print(F(" "));
    Serial.print(comment);
  }
  Serial.print(F(": "));
  Serial.println(freeMemory());
  //Serial.println(F(""));
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

void showString(byte str, char* prefix = NULL, bool newLine=false) {
    if (NULL!=prefix) {
    Serial.print(prefix);
    Serial.print(": ");
  }
  Serial.print(str);
  if (newLine) Serial.println(F(""));
}

class CmdParser {
  public:
    CmdParser(int max_input_len, char command_delimiter, char data_delimiter, char mode_delimiter) {
      this->_max_input_len = max_input_len;
      this->_command_delimiter = command_delimiter;
      this->_data_delimiter = data_delimiter;
      this->_mode_delimiter = mode_delimiter;
    }
    
    byte cmd() {
      //Serial.println(F("CmdParser.cmd()"));
        //Serial.print(F("cmd: ")); Serial.println(this->_cmd);
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
      this->_counter = 0;
    }
    byte parse(char* str) {
      Serial.println(F("CmdParser.parse()"));
      this->processCmd(str);
      this->processData(str);
      return this->count();
    }
    void debug() {
      Serial.print(F("len: ")); Serial.println(this->_counter);
      Serial.print(F("cmd: ")); Serial.println(this->_cmd);
      Serial.print(F("data: "));
      for (int i=0; i < this->_counter; i++) {
        Serial.print(this->_data[i]);
        Serial.print(F(", "));
      } Serial.println(F(""));
    }
    
  private:
    // полезные данные
    byte _cmd;
    unsigned int* _data = new unsigned int[this->_max_input_len];
    byte _counter = 0;

    // служебные данные
    char _command_delimiter;
    char _data_delimiter;
    char _mode_delimiter;
    int _max_input_len;

    // полезные методы
    byte processCmd(char* str) {
      //Serial.println(F("CmdParser.processCmd()"));
      char* cmd_piece = getPieceBefore(str,this->_command_delimiter);
      this->_cmd = (byte)atoi(cmd_piece);
      delete cmd_piece;
    }

    unsigned int* processData(char* str) {
        //Serial.println(F("CmdParser.processData()"));
        
        char* work_string = strchr(str, this->_command_delimiter);
    
        while (NULL != work_string) {
          work_string += 1;
          
          char* piece = getPieceBefore(work_string, this->_data_delimiter);
            //showString(piece, "piece");
            
          this->_data[this->_counter] = (unsigned int)atol(piece);
          
          delete piece;
          
          this->_counter++;
          
          work_string = strchr(work_string, this->_data_delimiter);
            //showString(work_string, "work_string", true);
        }
    
        delete work_string;
    }
    
    // служебные методы
    char* getPieceBefore(char* str, char delimiter) {
      //Serial.println(F("CmdParser.getPieceBefore()"));
      
      char* arr_delimiter = this->char2ptr(delimiter);
        byte len = strcspn(str, arr_delimiter);
      delete arr_delimiter;
    
      char* piece = new char[len + 1]; // +1 для нулевого символа
      strncpy(piece, str, len);
      piece[len] = char(0);
      
      return piece;
    }

    char* char2ptr(char letter) {
      //Serial.println(F("CmdParser.char2ptr()"));
      char* res = new char[2];
            res[0] = letter;
            res[1] = char(0);
      return res;
    }
};

SerialListener sListener(256,';');
CmdParser cParser(256, '|','_',':');

void setup() {
  Serial.begin(9600);
  Serial.println(F("=strchr-test="));
  showMem("on setup");
  
//  char* input_str = str2ptr("123|1_22_333");
//  cParser.parse(input_str);
//  cParser.debug();
//  cParser.clear();
//  Serial.println(F(""));
////  delete input_str;
//
//  input_str = str2ptr("123|1_22_333_4444");
//  cParser.parse(input_str);
//  cParser.debug();
//  cParser.clear();
//  Serial.println(F(""));
////  delete input_str;
//
//  input_str = str2ptr("456|1_22_333_4444_55555");
//  cParser.parse(input_str);
//  cParser.debug(); cParser.clear(); Serial.println(F(""));
////  delete input_str;
//
//  showMem();
}

void loop() {
  sListener.wait();

  if (sListener.recieved()) {
    char* data = new char[sListener.length()];
          data = sListener.data();

    cParser.parse(data);
    delete data;
    
    cParser.debug();
    cParser.clear();
    Serial.println(F(""));
    
    showMem();
  }
}

