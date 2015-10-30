#include <iostream>
#include <cstdlib>
#include "common.h"
#include <cassert>

using namespace std;


void help(char* cmd) {
  cerr << "\nusage: " << cmd << " SecretKeyFileName CipherTextFileName  \n\n" 
       << "  Decrypt the CipherText under some SecretKey and print it on the std output.\n\n";
  exit(0);
}


int main(int argc, char *argv[]) {
  if (argc != 3) help(argv[0]);
  char* sk_fn = argv[1]; 
  char* ct_fn = argv[2]; 

  FHEW::Setup();

  LWE::SecretKey* SK = LoadSecretKey(sk_fn);
  LWE::CipherText* ct = LoadCipherText32(ct_fn);

  unsigned int m = 0;
  int temp;

  cout << "Binary: ";

  for (int i = 0; i < 32; i++)
  {
    temp = LWE::Decrypt(*SK, ct[i]);
    cout << temp;
    m += temp;
    if ( i != 31)
      m = m << 1;
  }
  cout << endl;
  cout << "Decimal: " << m << "\n";

}
