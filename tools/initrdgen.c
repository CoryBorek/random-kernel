#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct initrd_header
{
   unsigned char magic; // The magic number is there to check for consistency.
   char name[64];
   unsigned int offset; // Offset in the initrd the file starts.
   unsigned int length; // Length of the file.
};

int main(int argc, char * argv[])
{
   int nheaders = (argc-1)/2;
   struct initrd_header headers[64];
   printf("size of header: %ld\n", sizeof(struct initrd_header));
   unsigned int off = sizeof(struct initrd_header) * 64 + sizeof(int);
   int i;
   for(i = 0; i < nheaders; i++)
   {
       printf("writing file %s->%s at 0x%x\n", argv[i*2+1], argv[i*2+2], off);
       strcpy(headers[i].name, argv[i*2+2]);
       headers[i].offset = off;
       FILE *stream = fopen(argv[i*2+1], "r");
       if(stream == 0)
       {
         printf("Error: file not found: %s\n", argv[i*2+1]);
         return 1;
       }
       fseek(stream, 0, SEEK_END);
       headers[i].length = ftell(stream);
       off += headers[i].length;
       fclose(stream);
       headers[i].magic = 0xBF;
   }
   
   FILE *wstream = fopen("./initrd.img", "w");
   unsigned char *data = (unsigned char *)malloc(off);
   fwrite(&nheaders, sizeof(int), 1, wstream);
   fwrite(headers, sizeof(struct initrd_header), 64, wstream);
   
   for(i = 0; i < nheaders; i++)
   {
     FILE *stream = fopen(argv[i*2+1], "r");
     unsigned char *buf = (unsigned char *)malloc(headers[i].length);
     int status = fread(buf, 1, headers[i].length, stream);
     printf("status: %d\n", status);
     fwrite(buf, 1, headers[i].length, wstream);
     fclose(stream);
     free(buf);
   }
   
   fclose(wstream);
   free(data);
   
   return 0;
}
