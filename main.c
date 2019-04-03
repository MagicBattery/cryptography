//Rotation cipher encryption

#include <stdio.h>

void rotation(char *msg, char *cipher, int k, int size); //Rotation cipher function prototype.

int main() {
  
  char msg[] = "WASHINGTON JOSEPH R. BIDEN JR. CAME UP IN POLITICS AS AN OLD-SCHOOL BACKSLAPPER WHOSE GREATEST STRENGTH WAS HIS ABILITY TO CONNECT. HE DOLED OUT HANDSHAKES AND HUGS TO FRIENDS AND STRANGERS ALIKE, AND HIS TENDENCY TO LAVISH HIS AFFECTIONS ON WOMEN AND GIRLS WAS SO CENTRAL TO HIS PERSONA THAT IT BECAME FODDER FOR LATE-NIGHT TELEVISION JOKES. BUT THE POLITICAL GROUND HAS SHIFTED UNDER MR. BIDEN, AND HIS TACTILE STYLE OF RETAIL POLITICKING IS NO LONGER A LAUGHING MATTER IN THE ERA OF METOO. NOW, AS HE CONSIDERS A RUN FOR PRESIDENT, MR. BIDEN IS STRUGGLING TO PREVENT A STRENGTH FROM TURNING INTO A CRIPPLING LIABILITY; ON TUESDAY ALONE, TWO MORE WOMEN TOLD THE NEW YORK TIMES THAT THE FORMER VICE PRESIDENT’S TOUCHES MADE THEM UNCOMFORTABLE. FOR MR. BIDEN, 76, THE RISKS ARE OBVIOUS: THE ACCUSATIONS FEED INTO A NARRATIVE THAT HE IS A RELIC OF THE PAST, UNSUITED TO REPRESENT HIS PARTY IN THE MODERN ERA, AGAINST AN INCUMBENT PRESIDENT WHOSE TREATMENT OF WOMEN SHOULD BE A CENTRAL LINE OF ATTACK. MR. BIDEN HAS DENIED ACTING INAPPROPRIATELY BUT HAS SAID HE WILL LISTEN RESPECTFULLY. SO FAR, NO PROMINENT DEMOCRAT HAS SUGGESTED HE NOT RUN, AND THE WOMEN COMPLAINING ABOUT HIM HAVE NOT CLAIMED SEXUAL HARASSMENT OR ASSAULT. OTHER WOMEN HAVE STEPPED FORWARD TO SAY MR. BIDEN’S TOUCHES WERE WELCOME. BUT THE ACCUSATIONS LODGED AGAINST MR. BIDEN HAVE RAISED QUESTIONS ABOUT WHEN A “TACTILE POLITICIAN” CROSSES THE LINE INTO INAPPROPRIATENESS."; //Hard-coded input of up to 256 characters
  int k = 23; //Rotation cipher key
  int size = (int)sizeof(msg);
  
  char cipher[size]; //Declare an output array to store the encrypted message (size is optimised)
 
  rotation(msg, cipher, k, size); //Call the rotate cipher function to perform the encryption

  printf("File output!");

  return 0;
}

void rotation(char *msg, char *cipher, int k, int size) { //Rotation cipher which takes the input message array, the empty output array, rotation key and size (of the array) as arguments
    
  FILE * encrypted;
  
  encrypted = fopen("output.txt", "w");  
    
  for(int i = 0; i < (size - 1); i++) { //Iterate through each elemen in the msg array, converting each character to be encrypted, then storing in the cipher array.
      if(msg[i] == ' ') {
          cipher[i] = msg[i];
          fprintf(encrypted, "%c", cipher[i]);
      } else {
          cipher[i] = ((msg[i] - 65 + k)%26) + 65;
          fprintf(encrypted, "%c", cipher[i]);
      }
  }
  
  fclose(encrypted);
  
  return;
}


