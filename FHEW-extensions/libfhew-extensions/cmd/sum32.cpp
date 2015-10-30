#include <iostream>
#include <cstdlib>
#include <cassert>

#include "common.h"

using namespace std;

FHEW::EvalKey* EK;


void help(char* cmd) {
  cerr << "\nusage: " << cmd << " EvalKeyFileName InCTFileName1 InCTFileName2 InCTFileName3 OutCTFileName1 OutCTFileName2  \n\n" 
       << "  Perform Homomorphic 32-bit SUM computation.\n\n"
       << "  InCTFileName1, InCTFileName2 - are 32-bit arguments; OutCTFileName1 - is their 32-bit sum\n"
       << "  InCTFileName3 is 1-bit carry in; OutCTFileName2 is 1-bit carry out\n";
  exit(0);
}

using namespace FHEW;

int main(int argc, char *argv[]) {

  if (argc != 7) help(argv[0]);

  LWE::CipherText *ct1, *ct2, *ct3, *ct_out1, *ct_out2;

  char* ek_fn   = argv[1];
  char* ict1_fn = argv[2];
  char* ict2_fn = argv[3];
  char* ict3_fn = argv[4];
  char* oct_fn1 = argv[5];
  char* oct_fn2 = argv[6];

  FHEW::Setup();

  ct1 = LoadCipherText32(ict1_fn);
  ct2 = LoadCipherText32(ict2_fn);
  ct3 = LoadCipherText32(ict3_fn);

  EK = LoadEvalKey(ek_fn);

  ct_out1 = new LWE::CipherText[32];
  ct_out2 = new LWE::CipherText;

  cout << "Calling HomSUM32" << endl;
  cout << "----------------------------------" << endl;

  HomSUM32(ct_out1, ct_out2, *EK, ct1, ct2, *ct3);

  cout << "Finished" << endl;

  SaveCipherText32(ct_out1, oct_fn1);
  SaveCipherText(ct_out2, oct_fn2);
}
