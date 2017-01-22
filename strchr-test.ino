#include <MemoryFree.h>
#include <SerialListener.h>

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

void showString(unsigned int str, char* prefix = NULL, bool newLine=false) {
    if (NULL!=prefix) {
    Serial.print(prefix);
    Serial.print(": ");
  }
  Serial.print(str);
  if (newLine) Serial.println(F(""));
}

class CmdParser {
  public:
    CmdParser(char command_delimiter, char data_delimiter, char mode_delimiter) {
      this->_command_delimiter = command_delimiter;
      this->_data_delimiter = data_delimiter;
      this->_mode_delimiter = mode_delimiter;
    }
    
    unsigned int cmd() {
      //Serial.println(F("CmdParser.cmd()"));
        //Serial.print(F("cmd: ")); Serial.println(this->_cmd);
      return this->_cmd;
    }
    unsigned int count() {
      return this->_counter;
    }
    unsigned int length() {
      return this->count();
    }
    unsigned int* data() {
      return this->_data;
    }
    void clear() {
      this->_counter = 0;
    }
    unsigned int parse(char* str) {
      Serial.println(F("CmdParser.parse()"));
      this->processCmd(str);
      this->processData(str);
      return this->count();
    }
    void debug() {
      Serial.print(F("cmd: ")); Serial.println(this->_cmd);
      Serial.print(F("data[")); Serial.print(this->_counter); Serial.print(F("]: "));
      for (int i=0; i < this->_counter; i++) {
        Serial.print(this->_data[i]);
        Serial.print(F(", "));
      } Serial.println(F(""));
    }
    
  private:
    // полезные данные
    unsigned int _cmd;
    unsigned int* _data = new unsigned int[128];
    unsigned int _counter = 0;

    // служебные данные
    char _command_delimiter;
    char _data_delimiter;
    char _mode_delimiter;

    // полезные методы
    unsigned int processCmd(char* str) {
      //Serial.println(F("CmdParser.processCmd()"));
      char* cmd_piece = getPieceBefore(str,this->_command_delimiter);
      this->_cmd = (unsigned int)atol(cmd_piece);
      delete cmd_piece;
    }

    unsigned int* processData(char* str) {
        Serial.println(F("CmdParser.processData()"));
        
        char* work_string = strchr(str, this->_command_delimiter);
    
        while (NULL != work_string) {
          work_string += 1;
          
          char* piece = getPieceBefore(work_string, this->_data_delimiter);
            showString(piece, "piece");

          bool laserActive = byte(piece[0])=='Y';
            showString(laserActive, "laserActive");

          piece = piece + 1;
            showString(piece, "piece");

          char* rawX = this->getPieceBefore(piece,',');
            showString(rawX, "rawX");
          
          char* rawY = strchr(piece,',') + 1;
            showString(rawY, "rawY");
          
//123|N0,0
          unsigned int x = (unsigned int)atol(rawX);
          unsigned int y = (unsigned int)atol(rawY);

          delete rawX, rawY;
          delete piece;

          if (laserActive) {
            x += 32768;
            y += 32768;
          }

          this->_data[this->_counter++] = x;
          this->_data[this->_counter++] = y;
          
          work_string = strchr(work_string, this->_data_delimiter);
            //showString(work_string, "work_string", true);
        }
    
        delete work_string;
    }
    
    // служебные методы
    char* getPieceBefore(char* str, char delimiter) {
      //Serial.println(F("CmdParser.getPieceBefore()"));
      
      char* arr_delimiter = this->char2ptr(delimiter);
        unsigned int len = strcspn(str, arr_delimiter);
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

SerialListener sL(256,';');
CmdParser cParser('|','_',':');

void setup() {
  Serial.begin(9600);
  Serial.println(F("=strchr-test="));

    showMem("on setup");
    Serial.println(F(""));
}

void loop() {
/*
123|1_2_3_4_5; 456|11_22_33_44_55_66; 78|1_222_33;

123|N0,0_Y0,1000_Y1000,1000_Y1000,0_Y0,0;
*/

  sL.wait();

  if (sL.recieved()) {
    showMem("on start");
    
    char* rawData = new char[sL.length()];
    rawData = sL.data();
    showString(rawData, "rawData");

    cParser.parse(rawData);
    delete rawData;
    
    unsigned int cmd = cParser.cmd();
    showString(cmd, "cmd", true);
    
    int len = cParser.count();
    showString(len, "len", true);
    
    unsigned int* data = new unsigned int[len];
    data = cParser.data();
    showMem("on retrive data");
//    delete data; // это удалять нельзя: программа падает из-за проблем с памятью
    
    //cParser.debug();
    cParser.clear();
    
    showMem("on finish");
    Serial.println(F(""));
  }
}


