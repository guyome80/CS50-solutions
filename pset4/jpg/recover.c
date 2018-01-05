#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>




int main(void)
{
   // open input file 
   FILE* cardptr = fopen("card.raw", "r");
   if (cardptr == NULL)
   {
      fclose(cardptr);
      printf("Could not open card.raw\n");
      return 1;
   }
    
   // open temporary outfile 
   FILE* jpgptr = NULL;
    
   // temporary storage for 1 block of 512 bytes
   uint8_t buffer[512];
    
   // photos counting variable
   int jpgNbr = 0;
   
   // block counting variable
    long long blckNbr = 0;
   
   // photos naming array
   char title[10];
   
   while (!feof(cardptr))
   {
      // prepares to move block
      ++blckNbr;
      
      // integer start of ith card block
      long long blockStart = 512 * blckNbr;
      
      // seeking beginning of the ith card block
      fseek(cardptr, blockStart, SEEK_SET);
      
      if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] == 0xe0 || buffer[3] == 0xe1))
      {
         // closes if temporary already exists
         if (jpgptr != NULL)
         {
            fclose(jpgptr);
         }
         
         // give the right name to the new jpg file
         sprintf(title, "%03d.jpg", jpgNbr);
         jpgptr = fopen(title, "w");
         
         // ensure proper opening 
         if (jpgptr == NULL)
         {
            fclose(cardptr);
            fprintf(stderr, "Could not create %s.\n", title);
         }
         
         // prepare for the next image name !
         jpgNbr++;
         
         // write block of 512 bytes in the jpg file
         fwrite(&buffer, sizeof(buffer), 1, jpgptr);
      }
         
      // checks if an image as already been stored temporary and moves block forward if so
      else if (jpgNbr != 0)
      {   
         fwrite(&buffer, sizeof(buffer), 1, jpgptr);
      }
      
      // read a block of info into temporary storage
      fread(&buffer, sizeof(buffer), 1, cardptr);
   }

   // fclose(card file);
   fclose(cardptr);
         
   return(0);  
}
// end of recover !!

