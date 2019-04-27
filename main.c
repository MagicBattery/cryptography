/*          
  To be included as the header file, 'input.txt':
  
  LINE #1:Encryption / Decryption Mode
   _________________________________________
  |     |                                   |
  |  0  |  Rotation - Encrypt               | 
  |_____|___________________________________|
  |     |                                   |
  |  1  |  Rotation - Decrypt (Key)         | 
  |_____|___________________________________|
  |     |                                   |
  |  2  |  Rotation - Decrypt (No Key)      |
  |_____|___________________________________|
  |     |                                   |
  |  3  |  Substitution - Encrypt           |
  |_____|___________________________________|
  |     |                                   |
  |  4  |  Substitution - Decrypt (Key)     |
  |_____|___________________________________|
  |     |                                   |
  |  5  |  Substitution - Decrypt (No Key)  |
  |_____|___________________________________|
  
  
  LINE #2: Key: '#' followed by ____________ ROTATION: integer [0, 25] (ie. #3)
                                    |
                                    |
                                    |_______ SUBSTITUTION: 26 unique letters (ie. #QWERTYUIOPASDFGHJKLZXCVBNM)
                                    |
                                    |
                                    |_______ Nothing if key not applicable (ie. #)
                                    
  
  FOLLOWING LINES: Cipher / plain text
*/

#include <stdio.h>
#include <stdlib.h>

void readInput(char *mode, char *key, char *msg);
int getRotKey(char *key);
void getSubKey(char *key, char *ubKey);

void rotationEnc(char *msg, int k, int size); //Rotation cipher function prototype (encryption mode)
void rotationDec(char *msg, int k, int size); //Rotation cipher function prototype (decryption mode)
void rotationDecNoKey(char *msg, int size); //Rotation cipher function prototype (decryption mode)

void substitutionEnc(char *msg, char *s, int size); //Substitution cipher function prototype
void substitutionDec(char *msg, char *s, int size); //Substitution cipher function prototype
void substitutionDecNoKey(char *msg, int size); //Substitution cipher function prototype

int main() {
  
  int mode;
  int rotKey;
  char subKey[26];
  char modeChar[50];
  char key[27];
  char msg[1000];
  int size = sizeof(msg);
  
  readInput(modeChar, key, msg);
  mode = atoi(modeChar); //Convert the read mode string to an integer
  
  
  printf("Mode: %d\n", mode);
  printf("Key: %s", key);
  printf("Message: %s", msg);
  
  
  switch(mode) { //Use the header file mode to determine which function must be called
      case 0:
              //Rotation encryption
              rotKey = getRotKey(key);
              rotationEnc(msg, rotKey, size);
              break;
              
      case 1:
              //Rotation decryption
              rotKey = getRotKey(key);
              rotationDec(msg, rotKey, size);
              break;
              
      case 2:
              //Rotation decryption (no key)
              rotationDecNoKey(msg, size);
              break;
              
      case 3:
              //Substitution encryption
              getSubKey(key, subKey);
              substitutionEnc(msg, subKey, size);
              break;
              
      case 4:
              //Substitution decryption
              getSubKey(key, subKey);
              substitutionDec(msg, subKey, size);
              break;
      
      case 5:
              //Substitution decryption (no key)
              substitutionDecNoKey(msg, size);
              break;
              
      default: printf("Mode input not recognised.");
               break;
  }

  printf("File output!\n");

  return 0;
}

///////////////// PERIPHERY FUNCTIONS //////////////////

//Read input from file, and delineate header components
void readInput(char *mode, char *key, char *msg) {
    
    FILE *input;
    input = fopen("input.txt", "r");
    
    //Read mode from header file
    //printf("Reading mode from header file...\n");
    fgets(mode, 50, input);
    
    //Read key from header file
    //printf("Looking for key...\n");
    fgets(key, 50, input);
    
    int i = 0;
    while(!feof(input)) {
        fscanf(input, "%c", &msg[i]);
        i++;
    }
    
    fclose(input);
    
    return;      
}

//Convert the header file Line #2 to a computable rotation cipher key (integer)
int getRotKey(char *key) {
    key[0] = ' '; //Get rid of the prefix '#'
    return atoi(key); //Convert remaining numerals to an integer and return
}

//Remove the hash '#' of the header file substitution key from Line #2 and store in new array subKey[]
void getSubKey(char *key, char *subKey) {
    for(int i = 0; i <= 25; i++) {
        subKey[i] = key[i + 1];
    }
    
    return;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////                                                                                                      /////////////////  
/////////////////                          CRYPTOGRAPHY                         ALGORITHMS                             ///////////////// 
/////////////////                                                                                                      /////////////////  
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////// ROTATION CIPHER ENCRYPTION ////////////////////////
void rotationEnc(char *msg, int k, int size) { //Rotation cipher which takes the input message array, the empty output array, rotation key and size (of the array) as arguments
    
  FILE * encrypted; //Delare a pointer to a file
  char temp;
  encrypted = fopen("output.txt", "w");  //Open a new file
    
  for(int i = 0; i < (size - 1); i++) { //Iterate through each element in the msg array, converting each character to be encrypted, then storing in the cipher array.
      if(msg[i] < 65 || msg[i] > 90) {
          fprintf(encrypted, "%c", msg[i]); //Print to file
      } else {
          temp = ((msg[i] - 65 + k)%26) + 65;
          fprintf(encrypted, "%c", temp); //Print to file
      }
    } 

  
  fclose(encrypted); //Close the file
  
  return;
}


////////////// ROTATION CIPHER DECRYPTION (KEY) ////////////////////
void rotationDec(char *msg, int k, int size) {
    
    FILE * encrypted; //Delare a pointer to a file
    char temp;
    encrypted = fopen("output.txt", "w");  //Open a new file
    
    for(int i = 0; i < (size - 1); i++) { //Iterate through each element in the msg array, converting each character to be encrypted, then storing in the cipher array.
      if(msg[i] < 65 || msg[i] > 90) {
          fprintf(encrypted, "%c", msg[i]); //Print to file
      } else {
          temp = ((msg[i] - 65 - k)%26) + 65;
          fprintf(encrypted, "%c", temp); //Print to file
      }
    }
    
    fclose(encrypted); //Close the file
  
    return;
}

////////////// ROTATION CIPHER DECRYPTION (NO KEY) ////////////////////
void rotationDecNoKey(char *msg, int size) {

    int k;
    int letterFreq[26] = {0};
    int max = 0;
    int maxIndex;
    
    //Tally the frequency of each letter, and store in array letterFreq[]
    for(int i = 0; i < (size - 1); i++) {
      letterFreq[msg[i] - 65]++;
    }
    
    //Identify most frequent letter from the above array
    for(int i = 0; i < 26; i++) {
        if(letterFreq[i] > max) {
            max = letterFreq[i];
            maxIndex = i;
        }
    }
    
    //Now, ASSUME this most common letter is 'E' (big assumption)
    //Determine rotation key
    k = (maxIndex + 65) - 69;
    if(k < 0) {
        k += 26;
    }
    
    //Using key, decrypt using rotationDec function
    rotationDec(msg, k, size);
}

////////////// SUBSTITUTION CIPHER ENCRYPTION ////////////////////
void substitutionEnc(char *msg, char *s, int size) {
    
    FILE * encrypted; //Delare a pointer to a file
    char temp;
    encrypted = fopen("output.txt", "w");  //Open a new file
    
    for(int i = 0; i < (size - 1); i++) {
        if(msg[i] < 65 || msg[i] > 90) {
          fprintf(encrypted, "%c", msg[i]); //Print to file  
        } else {
          temp = s[msg[i] - 65]; //Index of the letter being read (1-26)
          fprintf(encrypted, "%c", temp);
        }
        
    }
    
}

////////////// SUBSTITUTION CIPHER DECRYPTION (KEY) ////////////////////
void substitutionDec(char *msg, char *s, int size) {
    
    FILE * encrypted; //Delare a pointer to a file
    char temp;
    encrypted = fopen("output.txt", "w");  //Open a new file
    
    for(int i = 0; i < (size - 1); i++) {
        if(msg[i] < 65 || msg[i] > 90) {
          fprintf(encrypted, "%c", msg[i]); //Print to file  
        } else {
            for(int j = 0; j < 26; j++) {
                if(msg[i] == s[j]) {
                    temp = j + 65;
                    fprintf(encrypted, "%c", temp);
                    break;
                }
            }
        }
        
    }
    
    fclose(encrypted);
}

////////////// SUBSTITUTION CIPHER DECRYPTION (NO KEY) ////////////////////
void substitutionDecNoKey(char *msg, int size) {
    
}


