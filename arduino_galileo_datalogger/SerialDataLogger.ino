#include <SD.h>

//Log details
int logN;
uint32_t nread = 0;
String logFilename;
char charLogFilename[50];

void DIE(String message) {
  while(true) {
    Serial.println(message);
    delay(100);  
  }
}

void setup() {

  delay(2000);
  
  Serial.begin(115200);
  Serial1.begin(115200);
  Serial2.begin(115200);
  
  Serial.println("****** STARTING DATALOGGER ******\n");
  
  initSDCard();
  setupLogging();

  Serial.println("\n\n****** LOGGER COMMAND LINE *******");
  Serial.println("h for help and info");
  Serial.print("> ");
}

void loop() {
  attemptConsole();
  attemptLogging();
}

void attemptConsole() {
  while (Serial.available() > 0) {
    uint8_t read = Serial.read();
    
    switch(read) {
      case 'h':
        Serial.print("\nHELP AND INFO:\n");
        Serial.print("Logging to ");
        Serial.println(logFilename);
        Serial.print(nread);
        Serial.print(" bytes logged.");
        break;
      default:
        Serial.print("\nUnknown command\n");
    }
    Serial.print("\n> ");
  }  
}

void attemptLogging() {
  
  if (Serial1.available() > 0) {   
    File targetFile = SD.open(charLogFilename, FILE_WRITE);
    
    while(Serial1.available() > 0) {
      uint8_t read = Serial1.read();
      uint8_t written = targetFile.write(&read, 1);
      nread += written;
    }
    targetFile.close();
  
  }
  
}

//--------------------------------------------------------- initSDCard()
void initSDCard() {
  if (!SD.begin()) {
    DIE("Could not initialize SD card");
  }
}
//----------------------------------------------------- END initSDCard()

//--------------------------------------------------------- setupLogging()
void setupLogging() {
  SD.mkdir("/logs/");
  //workaround for segfault bug if no files in folder...
  createFile("/logs/PLACEHOLDER.txt");
  logN = 0;

  File root = SD.open("/logs/");
  root.rewindDirectory();
  root.close();
  
  root = SD.open("/logs/");
  root.rewindDirectory();
  if (! root) {
    DIE("/logs/ does not open. DIE.");
  } else {
    if (root.isDirectory()) {
      Serial.println("Opened /logs/");
      while (true) {
        File entry = root.openNextFile();
        if (!entry) {
          break;
        } else {
          Serial.print("Found ");
          Serial.println(entry.name());
          logN++;
        }
        entry.close();
      }
    } else {
      DIE("/logs/ is not a directory");
    }
  }
  root.close();
  logFilename = String("/logs/log") + String(logN) + String(".dat");
  Serial.println("Starting with log " + logFilename);
  createFile(logFilename);
  logFilename.toCharArray(charLogFilename, sizeof(charLogFilename));  
}
//----------------------------------------------------- END setupLogging()

//--------------------------------------------------------- createFile()
int createFile(String fileName) {
  char charFileName[fileName.length() + 1];
  fileName.toCharArray(charFileName, sizeof(charFileName));
 
  if (SD.exists(charFileName)) { 
    return 0;
  }
  
  File newFile = SD.open(charFileName, FILE_WRITE);
  newFile.close();
  
  if (SD.exists(charFileName)) {
    return 0;
  } 
  
  return -2;
}
//----------------------------------------------------- END createFile()

//----------------------------------------------------- addFileContent()
//SD.open retrieves the file in append more. 
int addFileStringContent(String fileName, String content) {
  char charFileName[fileName.length() + 1];
  fileName.toCharArray(charFileName, sizeof(charFileName));
 
  if (SD.exists(charFileName)) { 
    File targetFile = SD.open(charFileName, FILE_WRITE);
    targetFile.print(content);
    targetFile.close();
    return 0; 
  }
  return -1;
}
//------------------------------------------------- END addFileContent()

//----------------------------------------------------- addFileContent()
//SD.open retrieves the file in append more. 
int addFileBinaryContent(String fileName, uint8_t len, uint8_t* buf) {
  char charFileName[fileName.length() + 1];
  fileName.toCharArray(charFileName, sizeof(charFileName));
 
  if (SD.exists(charFileName)) { 
    File targetFile = SD.open(charFileName, FILE_WRITE);
    uint8_t written = targetFile.write(buf, len);
    targetFile.close();
    return written; 
  }
  return -1;
}
//------------------------------------------------- END addFileContent()

//-------------------------------------------------- galileoCreateFile()
//There appears to ba a bug with fopen() in the C standard library stio.h on
// the Galileo board. The Galileo SD Library calls fopen correctly, but it 
// butfopen does not behave as expected. This function works around that. 
int galileoCreateFile(String fileName) {
  char charFileName[fileName.length() + 1];
  fileName.toCharArray(charFileName, sizeof(charFileName));
 
  if (SD.exists(charFileName)) { 
    return 0;
  }
      
  char system_message[256];
  char directory[] = "/media/realroot";
  sprintf(system_message, "touch %s/%s", directory, charFileName);
  system(system_message);
  if (SD.exists(charFileName)) {
    return 0;
  }
  
  return -1;
}
//---------------------------------------------- END galileoCreateFile()
 
