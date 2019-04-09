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

void rotationEnc(char *msg, int k, int size); //Rotation cipher function prototype (encryption mode)
void rotationDec(char *msg, int k, int size); //Rotation cipher function prototype (decryption mode)
void rotationDecNoKey(char *msg, int k, int size); //Rotation cipher function prototype (decryption mode)

void substitutionEnc(char *msg, char *s, int size); //Substitution cipher function prototype
void substitutionDec(char *msg, char *s, int size); //Substitution cipher function prototype
void substitutionDecNoKey(char *msg, char *s, int size); //Substitution cipher function prototype

int main() {
    
  int mode = 0; //0 for encryption, 1 for decryption
  printf("Welcome to cool cats cipher!\n");
  printf("Choose a mode and press enter: ");
  scanf("%d", &mode);
  
  /////////////////////////////////////////////
  
  char msg[] = "AAA"; //Hard-coded input (for now)
  int size = (int)sizeof(msg);
  
  /////////////////////////////////////////////
  
  int k = 5; //Rotation cipher key
  char s[26] = "QAZXSWEDCVFRTGBNHYUJMKILOP"; //Substitution cipher key
  
  /////////////////////////////////////////////
  
  switch(mode) { //User's input for mode determines which function is called
      case 0:
              rotationEnc(msg, k, size);
              break;
              
      case 1:
              rotationDec(msg, k, size);
              break;
              
      case 2:
              rotationDecNoKey(msg, k, size);
              break;
              
      case 3:
              substitutionEnc(msg, s, size);
              break;
              
      case 4:
              substitutionDec(msg, s, size);
              break;
      
      case 5:
              substitutionEncNoKey(msg, s, size);
              break;
              
      default: printf("Please choose a valid mode (0-5)");
               break;
  }

  printf("File output!\n");

  return 0;
}






///////////////////// ROTATION CIPHER ENCRYPTION ////////////////////////
void rotationEnc(char *msg, int k, int size) { //Rotation cipher which takes the input message array, the empty output array, rotation key and size (of the array) as arguments
    
  FILE * encrypted; //Delare a pointer to a file
  char temp;
  encrypted = fopen("output.txt", "w");  //Open a new file
    
  for(int i = 0; i < (size - 1); i++) { //Iterate through each element in the msg array, converting each character to be encrypted, then storing in the cipher array.
      if(msg[i] == ' ') {
          fprintf(encrypted, " "); //Print to file
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
      if(msg[i] == ' ') {
          fprintf(encrypted, " "); //Print to file
      } else {
          temp = ((msg[i] - 65 - k)%26) + 65;
          fprintf(encrypted, "%c", temp); //Print to file
      }
    }
    
    fclose(encrypted); //Close the file
  
    return;
}

////////////// ROTATION CIPHER DECRYPTION (NO KEY) ////////////////////
void rotationDecNoKey(char *msg, int k, int size) {
    
}

////////////// SUBSTITUTION CIPHER ENCRYPTION ////////////////////
void substitutionEnc(char *msg, char *s, int size) {
    
    FILE * encrypted; //Delare a pointer to a file
    char temp;
    encrypted = fopen("output.txt", "w");  //Open a new file
    
    for(int i = 0; i < (size - 1); i++) {
        
        if(msg[i] == ' ') {
          fprintf(encrypted, " "); //Print to file  
        } else {
            temp = s[msg[i] - 65]; //Index of the letter being read (1-26)
            fprintf(encrypted, "%c", temp);
        }
        
    }
    
}

////////////// SUBSTITUTION CIPHER DECRYPTION (KEY) ////////////////////
void substitutionDec(char *msg, char *s, int size) {
    
}

////////////// SUBSTITUTION CIPHER DECRYPTION (NO KEY) ////////////////////
void substitutionDecNoKey(char *msg, char *s, int size);


