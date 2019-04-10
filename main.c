//Rotation cipher encryption

/*               MODE TABLE
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
  
  
  */

#include <stdio.h>
#include <stdlib.h>

void getInput(char *msg);

void rotationEnc(char *msg, int k, int size); //Rotation cipher function prototype (encryption mode)
void rotationDec(char *msg, int k, int size); //Rotation cipher function prototype (decryption mode)
void rotationDecNoKey(char *msg, int size); //Rotation cipher function prototype (decryption mode)

void substitutionEnc(char *msg, char *s, int size); //Substitution cipher function prototype
void substitutionDec(char *msg, char *s, int size); //Substitution cipher function prototype
void substitutionDecNoKey(char *msg, char *s, int size); //Substitution cipher function prototype

int main() {
    
  int mode = 0; //Mode to be selected by user
  
  char msg[12]; //Hard-coded input (for now)
  //getInput(msg); //Get the input file
  int size = (int)sizeof(msg);
  //printf("%s", msg);
  
  
  //Display welcome message and mode table to the user
  printf("Welcome to cool cats cipher!\n\n");
  printf("******* MODE TABLE *********\n");
  printf("0: Rotation - Encrypt\n");
  printf("1: Rotation - Decrypt (With Key)\n");
  printf("2: Rotation - Decrypt (No Key)\n");
  printf("3: Substitution - Encrypt\n");
  printf("4: Substitution - Decrypt (With Key)\n");
  printf("5: Substitution - Decrypt (No Key)\n\n");
  printf("Choose a mode and press enter: ");
  scanf("%d", &mode); //User selects their desired mode
  printf("\n");
  
  /////////////////////////////////////////////
  
  
  /////////////////////////////////////////////
  
  int k; //Rotation cipher key
  char s[26]; //Substitution cipher key
  
  /////////////////////////////////////////////
  
  switch(mode) { //User's input for mode determines which function is called
      case 0:
              printf("ROTATION ENCRYPTION SELECTED\n");
              printf("Please enter desired rotation cipher key (int [1, 25]): ");
              scanf("%d", &k);
              rotationEnc(msg, k, size);
              break;
              
      case 1:
              printf("ROTATION DECRYPTION (WITH KEY) SELECTED\n");
              printf("Please enter the rotation cipher key of the inputted message (int [0, 25]): ");
              scanf("%d", &k);
              rotationDec(msg, k, size);
              break;
              
      case 2:
              printf("ROTATION DECRYPTION (NO KEY) SELECTED\n");
              printf("No Key required!\n");
              rotationDecNoKey(msg, size);
              break;
              
      case 3:
              printf("SUBSTITUTION ENCRYPTION SELECTED\n");
              printf("Please enter desired rotation cipher key (26 unique letters): ");
              scanf("%s", s);
              substitutionEnc(msg, s, size);
              break;
              
      case 4:
              printf("SUBSTITUTION DECRYPTION (WITH KEY) SELECTED\n");
              printf("Please enter the substitution cipher key of the inputted message (26 unique letters): ");
              scanf("%s", s);
              substitutionDec(msg, s, size);
              break;
      
      case 5:
              printf("SUBSTITUTION DECRYPTION (NO KEY) SELECTED\n");
              printf("No Key required!\n");
              substitutionDecNoKey(msg, s, size);
              break;
              
      default: printf("Please choose a valid mode (0-5)");
               break;
  }

  printf("File output!\n");

  return 0;
}


//Get Input
void getInput(char *msg) {
    char temp = '0';
    int i = 0;
    
    while(temp != '#') {
        scanf("%c", &temp);
        msg[i] = temp;
        i++;
    }

    return;
}



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
void substitutionDecNoKey(char *msg, char *s, int size) {
    
}


