#include <stdio.h>
#include <string.h>


unsigned int JSHash(char* str, unsigned int len)  
{  
   unsigned int hash = 1315423911;  
   unsigned int i    = 0;  
  
   for(i = 0; i < len; str++, i++)  
   {  
      hash ^= ((hash << 5) + (*str) + (hash >> 2));  
   }  
  
   return hash;  
}  

unsigned int ELFHash(char* str, unsigned int len)  
{  
   unsigned int hash = 0;  
   unsigned int x    = 0;  
   unsigned int i    = 0;  
  
   for(i = 0; i < len; str++, i++)  
   {  
      hash = (hash << 4) + (*str);  
      if((x = hash & 0xF0000000L) != 0)  
      {  
         hash ^= (x >> 24);  
      }  
      hash &= ~x;  
   }  
  
   return hash;  
} 

unsigned int SDBMHash(char* str, unsigned int len)  
{  
   unsigned int hash = 0;  
   unsigned int i    = 0;  
  
   for(i = 0; i < len; str++, i++)  
   {  
      hash = (*str) + (hash << 6) + (hash << 16) - hash;  
   }  
  
   return hash;  
}  

unsigned int DJBHash(char* str, unsigned int len)  
{  
   unsigned int hash = 5381;  
   unsigned int i    = 0;  
  
   for(i = 0; i < len; str++, i++)  
   {  
      hash = ((hash << 5) + hash) + (*str);  
   }  
  
   return hash;  
}  


unsigned int DEKHash(char* str, unsigned int len)  
{  
   unsigned int hash = len;  
   unsigned int i    = 0;  
  
   for(i = 0; i < len; str++, i++)  
   {  
      hash = ((hash << 5) ^ (hash >> 27)) ^ (*str);  
   }  
   return hash;  
}  
/* End Of DEK Hash Function */

unsigned int BPHash(char* str, unsigned int len)  
{  
   unsigned int hash = 0;  
   unsigned int i    = 0;  
   for(i = 0; i < len; str++, i++)  
   {  
      hash = hash << 7 ^ (*str);  
   }  
  
   return hash;  
}  
/* End Of BP Hash Function */  


unsigned int FNVHash(char* str, unsigned int len)  
{  
   const unsigned int fnv_prime = 0x811C9DC5;  
   unsigned int hash      = 0;  
   unsigned int i         = 0;  
  
   for(i = 0; i < len; str++, i++)  
   {  
      hash *= fnv_prime;  
      hash ^= (*str);  
   }  
  
   return hash;  
}  
/* End Of FNV Hash Function */  


unsigned int APHash(char* str, unsigned int len)  
{  
   unsigned int hash = 0xAAAAAAAA;  
   unsigned int i    = 0;  
  
   for(i = 0; i < len; str++, i++)  
   {  
      hash ^= ((i & 1) == 0) ? (  (hash <<  7) ^ (*str) * (hash >> 3)) :  
                               (~((hash << 11) + ((*str) ^ (hash >> 5))));  
   }  
  
   return hash;  
}  
/* End Of AP Hash Function */ 

