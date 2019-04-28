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
#include <string.h>

//Function Prototypes
void readInput(char *mode, char *key, char *msg);
int msgSize(char *msg);
int getRotKey(char *key);
void getSubKey(char *key, char *ubKey);
void convertCaps(char *msg, int size);
int uniqueWords(char *msg);
int strFreq(char *msg, char *line);

void rotationEnc(char *msg, int k, int size); //Rotation cipher function prototype (encryption mode)
void rotationDec(char *msg, int k, int size); //Rotation cipher function prototype (decryption mode)
void rotationDecNoKey(char *msg, int size); //Rotation cipher function prototype (decryption mode)

void substitutionEnc(char *msg, char *s, int size); //Substitution cipher function prototype
void substitutionDec(char *msg, char *s, int size); //Substitution cipher function prototype
void substitutionDecNoKey(char *msg, int size); //Substitution cipher function prototype
void partialSubDec(char *msg, char *newKey, int size); //Helper to the above function

int main() {
  
  int mode;
  int rotKey;
  char subKey[26];
  char modeChar[50];
  char key[27]; //Enough space for '#' followed by 26-letter substitution
  char msg[1000] = {0};
  
  readInput(modeChar, key, msg);
  int size = msgSize(msg);
  convertCaps(msg, size); //Compute text as block letters for consistency and simplicity
  mode = atoi(modeChar); //Convert the mode string to an integer
  
  
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
              
      default: printf("Mode not recognised.");
               break;
  }

  printf("File output!\n");

  return 0;
}

/////////////////////////////////////////////////// PERIPHERY FUNCTIONS ////////////////////////////////////////////////////

//Read input from file, and delineate header components
void readInput(char *mode, char *key, char *msg) {
    
    FILE *input;
    input = fopen("input.txt", "r");
    
    fgets(mode, 50, input);
    fgets(key, 50, input);
    
    int i = 0;
    while(!feof(input)) {
        fscanf(input, "%c", &msg[i]);
        i++;
    }
    
    fclose(input);
    
    return;      
}


//Determine the size of only the message read from the file
int msgSize(char *msg) {
    int size = 0;
    
    while(msg[size] != NULL) {
        size ++;
    }
    
    return size;
}

void convertCaps(char *msg, int size){
    for(int i = 0; i < size; i++) {
        if(msg[i] >= 97 && msg[i] <= 122) {
            msg[i] -= 32;
        }
    }
    
    return;
}

int uniqueWords(char *msg) {
    
    //Read the dictionary and store the words in a 2D array
    int words = 1000;
    char line[words + 1][10];
    FILE * dictionaryFile = fopen("words.txt", "r");
    int i = 0;
    int matches = 0;
    
    while(fgets(line[i], 15, dictionaryFile)) {
        line[i][strlen(line[i]) - 1] = '\0';
        i++;
    }
    
    fclose(dictionaryFile);
    
    for(int i = 0; i < words; i++) {
        matches += strFreq(msg, line[i]);
    }
    
    return matches;
}

int strFreq(char *haystack, char *needle) {
    int len1 = strlen(haystack);
    int len2 = strlen(needle);
    int i = 0, j, count, freq = 0;
    
    while(i < len1) {
        j = 0;
        count = 0;
        
        while(haystack[i] == needle[j] && j < len2) {
            count++;
            i++;
            j++;
        }
        
        if(count == len2) {
            freq++;
        }
        
        i++;
    }
    return freq;
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



//////////////////////////////////////////////////////// CIPHER FUNCTIONS //////////////////////////////////////////////////////////////////



///////////////////// ROTATION CIPHER ENCRYPTION ////////////////////////
void rotationEnc(char *msg, int k, int size) { //Rotation cipher which takes the input message array, the empty output array, rotation key and size (of the array) as arguments
    
  FILE * encrypted; //Delare a pointer to a file
  char temp;
  encrypted = fopen("output.txt", "w");  //Open a new file
    
  for(int i = 0; i < size; i++) { //Iterate through each element in the msg array, converting each character to be encrypted, then storing in the cipher array.
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
    
    for(int i = 0; i < size; i++) { //Iterate through each element in the msg array, converting each character to be encrypted, then storing in the cipher array.
      if(msg[i] < 65 || msg[i] > 90) {
          fprintf(encrypted, "%c", msg[i]); //Print to file
      } else {
          temp = ((msg[i] - 65 + (26 - k))%26) + 65;
          fprintf(encrypted, "%c", temp); //Print to file
      }
    }
    
    fclose(encrypted); //Close the file
  
    return;
}

////////////// ROTATION CIPHER DECRYPTION (NO KEY) ////////////////////
void rotationDecNoKey(char *msg, int size) {
    
    int msgSize = strlen(msg);
    char attempts[26][msgSize]; //Store 26 new arrays, trying each key for later analysis
    int matchingWords[26] = {0}; //To test how many words match the dictionary for all 26 keys
    int key; //To be found
    char temp;
    
    //Decrypt with all 26 possible keys
    for(int x = 0; x < 26; x++) {
        
        for(int i = 0; i < msgSize; i++) { //Iterate through each element in the msg array, converting each character to be encrypted, then storing in the cipher array.
            if(msg[i] < 65 || msg[i] > 90) {
                attempts[x][i] = msg[i]; //Print to file
            } else {
                temp = ((msg[i] - 65 + (26 - x))%26) + 65;
                attempts[x][i] = temp; //Print to file
            }
        }
        
        matchingWords[x] = uniqueWords(attempts[x]);
    }

    //Find max index of matchingWords to deduce most likely key:
    int max = 0;
    for(int i = 0; i < 26; i++) {
        if(matchingWords[i] > max) {
            max = matchingWords[i];
            key = i;
        }
    }
    
    //Finally, the key is known and key = maxIndex. Now decrypt like usual.
    rotationDec(msg, key, msgSize);

}

////////////// SUBSTITUTION CIPHER ENCRYPTION ////////////////////
void substitutionEnc(char *msg, char *s, int size) {
    
    FILE * encrypted; //Delare a pointer to a file
    char temp;
    encrypted = fopen("output.txt", "w");  //Open a new file
    
    for(int i = 0; i < size; i++) {
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
    
    for(int i = 0; i < size; i++) {
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
    char newKey[26] = {0};
    int letterFreq[26] = {0};
    char ones[26] = {0};
    char alphabet[26] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    char temp;
    
    //Tally the frequency of each letter, and store in array letterFreq[]
    for(int i = 0; i < 26; i++) {
      for(int j = 0; j < size; j++) {
          if(msg[j] == (i + 65)) {
            letterFreq[i]++;
          }
      }
      
    }
    
    //Sort the array from highest frequency to lowest (bubble sort)
    
    for(int j = 0; j < 25; j++) {
        for(int i = 0; i < (25 - j); i++) {
            if(letterFreq[i] < letterFreq[i + 1]) {
                temp = letterFreq[i];
                letterFreq[i] = letterFreq[i + 1];
                letterFreq[i + 1] = temp;
                
                temp = alphabet[i];
                alphabet[i] = alphabet[i + 1];
                alphabet[i + 1] = temp;
            }
        }  
    }
    
    //Assume MOST frequent letter is 'E', 2nd 'T', 3rd 'A', 4th 'O', 5th 'I'
    newKey[4] = alphabet[0];
    newKey[19] = alphabet[1];
    newKey[0] = alphabet[2];
    newKey[14] = alphabet[3];
    newKey[8] = alphabet[4];
    
    //Break message up into individual words to locate all one-letter words
    
    char msg2[size];
    char *found;
    
    //Create new msg array to be 'broken up'
    for(int i = 0; i < size; i++) {
        msg2[i] = msg[i];
    }
    
    found = strtok(msg2, " ");
    while(found != NULL) {
        if(strlen(found) == 1) {
            ones[found[0] - 65]++; //Add to the index of the letter found. Ie is N is found, it is the 13th letter, so increment index 13.
        }
        found = strtok(NULL, " ");
    }
    
    //TEST DECRYPTION 1
    //partialSubDec(msg, newKey, size);
    
    //Find most common one-letter word.
    int max = 0;
    int index;
    for(int i = 0; i < 26; i++) {
        if(ones[i] > max) {
            max = ones[i];
            index = i;
        }
    }
    
    newKey[0] = index + 65; //Assume this letter should be A.
    newKey[7] = 'H';
    newKey[17] = 'X';
    newKey[21] = 'G';
    newKey[13] = 'M';
    newKey[20] = 'E';
    newKey[6] = 'Q';
    newKey[3] = 'R';
    newKey[24] = 'V';
    newKey[13] = 'K';
    newKey[18] = 'J';
    newKey[5] = 'M';
    newKey[11] = 'Z';
    newKey[10] = 'A';
    newKey[9] = 'D';
    newKey[22] = 'P';
    newKey[15] = 'I';
    newKey[2] = 'L';
    newKey[12] = 'O';
    newKey[1] = 'W';
    
    //TEST DECRYPTION 2
    partialSubDec(msg, newKey, size);
}

void partialSubDec(char *msg, char *newKey, int size) {
    char temp2;
    printf("////////// CIPHER ATTEMPT //////////\n\n");
    
    for(int i = 0; i < size; i++) {
        if(msg[i] < 65 || msg[i] > 90) {
          printf("%c", msg[i]);
        } else {
            for(int j = 0; j < 27; j++) {
                if(msg[i] == newKey[j]) {
                    temp2 = j + 65;
                    printf("%c", temp2);
                    break;
                }
                if(j == 26) {
                    printf("*");
                    break;
                }
            }
        }

    }
    
    printf("\n");
}


