#include <iostream>
#include <cstdlib>
#include "common.h"
#include <cassert>
#include <string>

using namespace std;

void help(char* cmd) {
  cerr << "\nusage: " << cmd << " Message SecretKeyFileName CipherTextFileName  \n\n" 
       << "  Encrypt the Message under some SecretKey and store it in a File.\n\n";
  exit(0);
}


int main(int argc, char *argv[]) {
  if (argc != 4) help(argv[0]);
  unsigned int message = atoll(argv[1]);
  char* sk_fn = argv[2]; 
  char* ct_fn = argv[3];
  int two_pow_31 = 1 << 31;
  int bit;

  LWE::SecretKey* SK = LoadSecretKey(sk_fn);

  LWE::CipherText* ct = new LWE::CipherText[32];

  cout << "Binary: ";

  for (int i = 0; i < 32; i++)
  {
    bit = (message & two_pow_31) ? 1 : 0;
    cout << bit;
    LWE::Encrypt(&ct[i], *SK, bit);
    message = message << 1;
  }

  cout << endl;

  SaveCipherText32(ct,ct_fn);
}
