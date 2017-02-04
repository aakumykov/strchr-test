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
//      Serial.println(F("CmdParser.parse()"));
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
//      Serial.println(F("CmdParser.processCmd()"));
//      showString(str, "str", true);
      char* cmd_piece = getPieceBefore(str,this->_command_delimiter);
      this->_cmd = (unsigned int)atol(cmd_piece);
      delete cmd_piece;
    }

    unsigned int* processData(char* str) {
//        Serial.println(F("CmdParser.processData()"));
//        showString(str, "input string");
       
        // 123|Y0,Y2000,Y100,Y300;
        // 456|N0,N0,Y0,Y1000,Y1000,Y1000,Y1000,Y0,Y0,Y0;

//        char* work_string;
//        char* data_piece;

        // передвигаю "курсор" за разделитель команды
        char* work_string = strchr(str, this->_command_delimiter) + 1;

        // первый кусочек данных
        char* data_piece = getPieceBefore(work_string,this->_data_delimiter);
        work_string = strchr(work_string,this->_data_delimiter) + 1;

        // последующие куски, пока не закончатся
        while (0 != strlen(data_piece)) {
          unsigned int raw_number = (unsigned int)atol(data_piece+1);
          
          if ('Y'==data_piece[0]) raw_number += 32768;
          
          //showString(raw_number, "raw_number",true);

          data_piece = getPieceBefore(work_string,this->_data_delimiter);
          work_string = strchr(work_string,this->_data_delimiter) + 1;
        }

        delete data_piece;
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
CmdParser cParser('|',',',':');

void setup() {
  Serial.begin(9600);
  Serial.println(F("=strchr-test="));

    showMem("on setup");
    Serial.println(F(""));
}

void loop() {
/*
123|Y0,Y2000,Y100,Y300;
456|N0,N0,Y0,Y1000,Y1000,Y1000,Y1000,Y0,Y0,Y0;
*/

  sL.wait();

  if (sL.recieved()) {
    showMem("on start");
    
    char* inputData = new char[sL.length()];
    inputData = sL.data();
    showString(inputData, "inputData");

    cParser.parse(inputData);
//     unsigned int cmd = cParser.cmd();
//     showString(cmd,"cmd",true);
//    cParser.clear();
    
    delete inputData;
    showMem("on finish");
    Serial.println(F(""));
  }
}


