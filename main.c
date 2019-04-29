/*          
  c3329722 - 2018
  Program takes an input file with a header to choose between the modes below. The program will then use algorithms to either encrypt
  or decrypt either a rotation or substitution cipher based on the user's input. If 'no key' mode is input, the program will perform
  a statistical analysis to attemp to identify the correct plain text.
  
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
                                    
  
  LINE #3: Cipher / plain text (all on the one line, case optional)
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


//Mainline function to direct the program into differnt functions
int main() {
  
  int mode; //Integer to be read from Line #1 of input file, specifying task selection, i.e. 0 = rotation encryption (see table above)
  int rotKey; //Variable to store rotation key if extracted from file
  char subKey[26]; //Variable to store substitution key if extracted from file
  char modeChar[50];
  char key[27]; //Enough space for '#' followed by 26-letter substitution
  char msg[1000] = {0}; //Very large array to store the inputted message
  
  readInput(modeChar, key, msg);
  int size = msgSize(msg); //Store the size of the message (no characters)
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

  return 0;
}

/////////////////////////////////////////////////// PERIPHERY FUNCTIONS ////////////////////////////////////////////////////

//Read input from file, and delineate header components. Args: *mode - to store mode from input, *key - to store key from input, 
//*msg to store the message from input.
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


//Determine the size of only the message read from the file. Args: *msg - to find its size
int msgSize(char *msg) {
    int size = 0;
    
    while(msg[size] != '\0') {
        size ++;
    }
    
    return size;
}


//Converts all input text to CAPITALS. Args *msg - to be capitalised, *size - to prevent accessing memory out of range.
void convertCaps(char *msg, int size){
    for(int i = 0; i < size; i++) {
        if(msg[i] >= 97 && msg[i] <= 122) {
            msg[i] -= 32;
        }
    }
    
    return;
}

//Returns integer for how many times any of the top 1000 english words appear in the input string. Args *msg - input string
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

//Determine how many times one 'needle' string (smaller) is found in a 'haystack' string (larger). Args *needle - small string,
//*haystack - larger string
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

//Convert the header file Line #2 to a computable rotation cipher key (integer). Args: *key - to read and manipulate the key text from input
int getRotKey(char *key) {
    key[0] = ' '; //Get rid of the prefix '#'
    return atoi(key); //Convert remaining numerals to an integer and return
}

//Remove the hash '#' of the header file substitution key from Line #2 and store in new array subKey[]
//Args: *key - to manupulate the key array from input, *subKey to correctly write the 26 characters to one substitutional key array.
void getSubKey(char *key, char *subKey) {
    for(int i = 0; i <= 25; i++) {
        subKey[i] = key[i + 1]; // + 1 to remove th first '#' which is not part of the key.
    }
    return;
}



//////////////////////////////////////////////////////// CIPHER FUNCTIONS //////////////////////////////////////////////////////////////////



///////////////////// ROTATION CIPHER ENCRYPTION ////////////////////////
//Args: *msg - message to be encrypted, k - rotation cipher key to use, size - size of the message (characters) for array computation
void rotationEnc(char *msg, int k, int size) { //Rotation cipher which takes the input message array, the empty output array, rotation key and size (of the array) as arguments
    
  FILE * encrypted; //Delare a pointer to a file
  char temp;
  encrypted = fopen("output.txt", "w");  //Open a new file
    
  for(int i = 0; i < size; i++) { //Iterate through each element in the msg array, converting each character to be encrypted, then storing in the cipher array.
      if(msg[i] < 65 || msg[i] > 90) {
          fprintf(encrypted, "%c", msg[i]); //Print to file
          printf("%c", msg[i]); //Print to stdout (for markers)
      } else {
          temp = ((msg[i] - 65 + k)%26) + 65; //Perform rotation encryption algorithm
          fprintf(encrypted, "%c", temp); //Print to file
          printf("%c", temp); //Print to stdout (for markers)
      }
  } 
  
  fclose(encrypted); //Close the file
  printf("\nFile output!\n");
  
  return;
}


////////////// ROTATION CIPHER DECRYPTION (KEY) ////////////////////
//Args: *msg - message to be decrypted, k - rotation cipher key to use, size - size of the message (characters) for array computation
void rotationDec(char *msg, int k, int size) {
    
    FILE * encrypted; //Delare a pointer to a file
    char temp;
    encrypted = fopen("output.txt", "w");  //Open a new file
    
    for(int i = 0; i < size; i++) { //Iterate through each element in the msg array, converting each character to be encrypted, then storing in the cipher array.
      if(msg[i] < 65 || msg[i] > 90) {
          fprintf(encrypted, "%c", msg[i]); //Print to file
          printf("%c", msg[i]); //Print to stdout (for markers)
      } else {
          temp = ((msg[i] - 65 + (26 - k))%26) + 65; //Perform rotation decryption algorithm
          fprintf(encrypted, "%c", temp); //Print to file
          printf("%c", temp); //Print to stdout (for markers)
      }
    }
    
    fclose(encrypted); //Close the file
    printf("\nFile output!\n");
  
    return;
}

////////////// ROTATION CIPHER DECRYPTION (NO KEY) ////////////////////
//Args: *msg - message to be encrypted, size - size of the message (characters) for array computation
//NOTE: no key provided
void rotationDecNoKey(char *msg, int size) {
    
    int msgSize = strlen(msg);
    printf("%d vs %d\n\n", size, msgSize);
    char attempts[26][msgSize]; //Store 26 new arrays, trying each key for later analysis
    int matchingWords[26] = {0}; //To test how many words match the dictionary for all 26 keys
    int matchingWordsIndex[26] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25};
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
        
        matchingWords[x] = uniqueWords(attempts[x]); //Record how many words match for this particular key.
    }

    //Sort number of matchingWords with their indexes to deduce most likely keys:
    for(int j = 0; j < 25; j++) {
        for(int i = 0; i < (25 - j); i++) {
            if(matchingWords[i] < matchingWords[i + 1]) {
                temp = matchingWords[i];
                matchingWords[i] = matchingWords[i + 1];
                matchingWords[i + 1] = temp;
                
                temp = matchingWordsIndex[i];
                matchingWordsIndex[i] = matchingWordsIndex[i + 1];
                matchingWordsIndex[i + 1] = temp;
            }
        }  
    }
    
    //Finally, the some likely keys are known. Now decrypt like usual with top 5 keys.
    FILE * encrypted; //Delare a pointer to a file
    char tempChar;
    encrypted = fopen("output.txt", "w");  //Open a new file
        
    for(int j = 0; j < 5; j++) {
        fprintf(encrypted, "Word matches: %d\n", matchingWords[j]);
        printf("Word matches: %d\n", matchingWords[j]);
        for(int i = 0; i < msgSize; i++) { //Iterate through each element in the msg array, converting each character to be encrypted, then printing
          if(msg[i] < 65 || msg[i] > 90) {
              fprintf(encrypted, "%c", msg[i]); //Print to file
              printf("%c", msg[i]); //Print to stdout (for markers)
          } else {
              tempChar = ((msg[i] - 65 + (26 - matchingWordsIndex[j]))%26) + 65;
              fprintf(encrypted, "%c", tempChar); //Print to file
              printf("%c", tempChar); //Print to stdout (for markers)
          }
        }
        
        fprintf(encrypted, "\n\n");
        printf("\n\n");
    }
    fclose(encrypted); //Close the file

}

////////////// SUBSTITUTION CIPHER ENCRYPTION ////////////////////
//Args: *msg - message to be encrypted, *s - substituion array (key) to use, size - size of the message (characters) for array computation
void substitutionEnc(char *msg, char *s, int size) {
    
    FILE * encrypted; //Delare a pointer to a file
    char temp;
    encrypted = fopen("output.txt", "w");  //Open a new file
    
    for(int i = 0; i < size; i++) {
        if(msg[i] < 65 || msg[i] > 90) {
          fprintf(encrypted, "%c", msg[i]); //Print to file
          printf("%c", msg[i]); //Print to stdout (for markers)
        } else {
          temp = s[msg[i] - 65]; //Index of the letter being read (1-26)
          fprintf(encrypted, "%c", temp); //Print to file
          printf("%c", temp); //Print to stdout (for markers)
        }
        
    }
    
}

////////////// SUBSTITUTION CIPHER DECRYPTION (KEY) ////////////////////
//Args: *msg - message to be decrypted, *s - substituion array (key) to use, size - size of the message (characters) for array computation
void substitutionDec(char *msg, char *s, int size) {
    
    FILE * encrypted; //Delare a pointer to a file
    char temp;
    encrypted = fopen("output.txt", "w");  //Open a new file
    
    for(int i = 0; i < size; i++) {
        if(msg[i] < 65 || msg[i] > 90) {
          fprintf(encrypted, "%c", msg[i]); //Print to file
          printf("%c", msg[i]); //Print to stdout (for markers)
        } else {
            for(int j = 0; j < 26; j++) {
                if(msg[i] == s[j]) {
                    temp = j + 65; //Determine char no. of the decrypted letter.
                    fprintf(encrypted, "%c", temp); //Print to file
                    printf("%c", temp); //Print to stdout (for markers)
                    break;
                }
            }
        }
        
    }
    
    fclose(encrypted);
}

////////////// SUBSTITUTION CIPHER DECRYPTION (NO KEY) ////////////////////
//Args: *msg - message to be decrypted, size - size of the message (characters) for array computation
//NOTE: no key provided
void substitutionDecNoKey(char *msg, int size) {
    char newKey[26] = {0};
    int letterFreq[26] = {0};
    char ones[26] = {0};
    char threes[50][3] = {0};
    char alphabet[26] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    char mostFrequent[26] = "ETAOINSRHLDCUMFPGWYBVKXJQZ";
    char special[26] = "NWLRBMQHCDAZOKYITXJFEGPUVD";
    char words[200][15];
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
    
    //Assume MOST frequent letter is 'E', 2nd 'T', 3rd 'A', 4th 'O', 5th 'I', and so on
    for(int i = 0; i < 26; i++) {
        newKey[mostFrequent[i] - 65] = alphabet[i];
    }
    
    //Break message up into individual words
    char msg2[size];
    char *found;
    
    //Zero words array
    for(int j = 0; j < 200; j++) {
        for(int i = 0; i < 15; i++) {
            words[j][i] = '\0';
        }
    }
    
    //Create new msg array to be 'broken up'
    for(int i = 0; i < size; i++) {
        msg2[i] = msg[i];
    }
    
    //Store each word in 2D array of chars.
    int noWords = 0;
    int wordLen;
    found = strtok(msg2, " ");
    while(found != NULL) {
        noWords++;
        wordLen = strlen(found);
        for(int i = 0; i < wordLen; i++) { //Iterate for each letter in this word
            if(found[i] >= 65 && found[i] <= 90) {
                words[noWords][i] = found[i];
            }
        }
        found = strtok(NULL, " ");
    }
    
    ///////// USING 1 LETTER WORDS AS A or I /////////
    for(int i = 0; i <= noWords; i++) {
        if(strlen(words[i]) == 1) {
            ones[words[i][0] - 65]++;
        }
    }
    
    int max = 0;
    int index;
    for(int i = 0; i < 26; i++) {
        if(ones[i] > max) {
            max = ones[i];
            index = i;
        }
    }
    
    newKey[0] = index + 65; //Assume this letter should be A, possibly overwriting the substitution for 'A' from frequency analysis.
    
    ////////// ASSIGNING MOST COMMON 3 LETTER WORD AS 'THE' /////////////
    int no3Words = 0; //Counter for the number of 3 letter words found
    int pos3 = 0; //Index position indictor for the array threes[][];
    for(int i = 0; i <= noWords; i++) {
        if(strlen(words[i]) == 3) {
            threes[pos3][0] = words[i][0];
            threes[pos3][1] = words[i][1];
            threes[pos3][2] = words[i][2];
            no3Words++;
            pos3++;
        }
    }
    
    //Find frequencies of all 3 letter words
    int threeMatches[no3Words];
    char tempStr1[3];
    char tempStr2[3];
    for(int j = 0; j < no3Words; j++) {
        for(int t = 0; t < 3; t++) {
                tempStr1[t] = threes[j][t];
            }
        for(int i = 0; i < no3Words; i++) {
            for(int t = 0; t < 3; t++) {
                tempStr2[t] = threes[i][t];
            }
            if(strcmp(tempStr1, tempStr2) == 0) {
                threeMatches[j]++;
            }
        }
    }
    
    //Find the most common 3 letter word from these frequencies
    int max2 = 0;
    int index2;
    for(int i = 0; i < no3Words; i++) {
        if(threeMatches[i] > max2) {
            max2 = threeMatches[i];
            index2 = i;
        }
    }
    
    //Most common 3 letter word found. Now assign the letters of this word to 'T', 'H', and 'E' respectively
    newKey[19] = threes[index2][0];
    newKey[7] = threes[index2][1];
    newKey[4] = threes[index2][2];
    
    //Now attempt to decrypt and print out possilbe answers to file and stdout
    FILE * encrypted; //Delare a pointer to a file
    char temp2;
    encrypted = fopen("output.txt", "w");  //Open a new file
    
    /////////// Day 1 Text Attempt ///////////
    fprintf(encrypted, "If day one cipher:\n");
    printf("If day one cipher:\n");
    
    for(int i = 0; i < size; i++) {
        if(msg[i] < 65 || msg[i] > 90) {
          fprintf(encrypted, "%c", msg[i]); //Print to file
          printf("%c", msg[i]); //Print to stdout (for markers)
        } else {
            for(int j = 0; j < 27; j++) {
                if(msg[i] == special[j]) {
                    temp2 = j + 65;
                    fprintf(encrypted, "%c", temp2); //Print to file
                    printf("%c", temp2); //Print to stdout (for markers)
                    break;
                }
                if(j == 26) {
                    printf("*");
                    break;
                }
            }
        }
    }
    
    /////////// Statistical Analysis Attempt ///////////
    fprintf(encrypted, "\n\nStatistical analysis attempt:\n");
    fprintf(encrypted, "Assumptions: Character frequencies follow 'ETAOINSRHLDCUMFPGWYBVKXJQZ' from MOST to LEAST frequent.\n");
    fprintf(encrypted, "           : The most common 1 letter word was '%c', and so this word was substituted for 'A'.\n", index + 65);
    fprintf(encrypted, "           : The most common 3 letter word was '%c%c%c', and so this word was substituted for 'T', 'H' and 'E' respectively.\n\n", threes[index2][0], threes[index2][1], threes[index2][2]);
    printf("\n\nStatistical analysis attempt:\n");
    printf("Assumptions: Character frequencies follow 'ETAOINSRHLDCUMFPGWYBVKXJQZ' from MOST to LEAST frequent.\n");
    printf("           : The most common 1 letter word was '%c', and so this word was substituted for 'A'.\n", index + 65);
    printf("           : The most common 3 letter word was '%c%c%c', and so this word was substituted for 'T', 'H' and 'E' respectively.\n\n", threes[index2][0], threes[index2][1], threes[index2][2]);
    for(int i = 0; i < size; i++) {
        if(msg[i] < 65 || msg[i] > 90) {
          fprintf(encrypted, "%c", msg[i]); //Print to file
          printf("%c", msg[i]); //Print to stdout (for markers)
        } else {
            for(int j = 0; j < 27; j++) {
                if(msg[i] == newKey[j]) {
                    temp2 = j + 65;
                    fprintf(encrypted, "%c", temp2); //Print to file
                    printf("%c", temp2); //Print to stdout (for markers)
                    break;
                }
                if(j == 26) { //If there are no substitutions availabe from the above process for a particular letter, just print '*'
                    printf("*");
                    break;
                }
            }
        }
    }
    
    fclose(encrypted); //Close the file as the program is finished with it
}
