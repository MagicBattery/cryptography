//Rotation cipher encryption

#include <stdio.h>

void rotationEnc(char *msg, char *cipher, int k, int size); //Rotation cipher function prototype.
void rotationDec(char *msg, char *cipher, int k, int size);
void substitution(char *msg, char *cipher, int k, int size); //Substitution cipher function prototype.

int main() {
    
  int dir = 0;
    
  printf("Welcome to cool cats cipher!\n");
  printf("Enter 0 for encryption, or 1 for decryption: ");
  scanf("%d", &dir);

  char msg[] = "MJQQT"; //Hard-coded input (for now)
  int k = 5; //Rotation cipher key
  int size = (int)sizeof(msg);
  
  char cipher[size]; //Declare an output array to store the encrypted message (size is optimised)
  
  if(dir == 0) {
      printf("Encrypting...");
      rotationEnc(msg, cipher, k, size);
  } else if(dir == 1) {
      printf("Decrypting...");
      rotationDec(msg, cipher, k, size);
  }
 
   //Call the rotate cipher function to perform the encryption

  printf("File output!\n");

  return 0;
}

void rotationEnc(char *msg, char *cipher, int k, int size) { //Rotation cipher which takes the input message array, the empty output array, rotation key and size (of the array) as arguments
    
  FILE * encrypted; //Delare a file pointer
  
  encrypted = fopen("output.txt", "w");  //Open a new file
    
  for(int i = 0; i < (size - 1); i++) { //Iterate through each elemen in the msg array, converting each character to be encrypted, then storing in the cipher array.
      if(msg[i] == ' ') {
          cipher[i] = msg[i];
          fprintf(encrypted, "%c", cipher[i]); //Print to file
      } else {
          cipher[i] = ((msg[i] - 65 + k)%26) + 65;
          fprintf(encrypted, "%c", cipher[i]); //Print to file
      }
  }
  
  fclose(encrypted); //Close the file
  
  return;
}

void rotationDec(char *msg, char *cipher, int k, int size) { //Rotation cipher which takes the input message array, the empty output array, rotation key and size (of the array) as arguments
    
  FILE * encrypted; //Delare a file pointer
  
  encrypted = fopen("output.txt", "w");  //Open a new file
    
  for(int i = 0; i < (size - 1); i++) { //Iterate through each elemen in the msg array, converting each character to be encrypted, then storing in the cipher array.
      if(msg[i] == ' ') {
          cipher[i] = msg[i];
          fprintf(encrypted, "%c", cipher[i]); //Print to file
      } else {
          cipher[i] = ((msg[i] - 65 - k)%26) + 65;
          fprintf(encrypted, "%c", cipher[i]); //Print to file
      }
  }
  
  fclose(encrypted); //Close the file
  
  return;
}

void substitution(char *msg, char *cipher, int k, int size) {
    
}


