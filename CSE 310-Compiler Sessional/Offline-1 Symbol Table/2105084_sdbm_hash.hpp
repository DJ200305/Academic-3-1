#ifndef HASH
#define HASH
#include<bits/stdc++.h>
using namespace std;
class Hash{
public:
   static unsigned int SDBMHash(string str,unsigned int num_buckets) {
	unsigned int hash = 0;
	unsigned int i = 0;
	unsigned int len = str.length();

	for (i = 0; i < len; i++)
	{
    hash =  ((str[i]) + (hash << 6) + (hash << 16)- hash) %
    num_buckets;
	}
	return hash;
}

static unsigned int BKDRHash(string str,unsigned int num_buckets)
{
   unsigned int length = str.length();	 
   unsigned int seed = 131; /* 31 131 1313 13131 131313 etc.. */
   unsigned int hash = 0;
   unsigned int i    = 0;

   for (i = 0; i < length;  ++i)
   {
      hash = (hash * seed) + (str[i]);
   }

   return hash;
}

static unsigned int DJBHash(string str,unsigned int bucket_size)
{
   unsigned int hash = 5381;
   unsigned int i    = 0;
   unsigned int length = str.length();
   for (i = 0; i < length;++i)
   {
      hash = ((hash << 5) + hash) + (str[i]);
   }

   return hash;
}

};
#endif