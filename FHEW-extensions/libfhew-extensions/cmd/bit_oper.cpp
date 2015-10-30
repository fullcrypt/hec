#include <iostream>
#include <cstdlib>
#include <cassert>

#include "common.h"

using namespace std;

FHEW::EvalKey* EK;


void help(char* cmd) {
  cerr << "\nusage: " << cmd << " Operation EvalKeyFileName InCTFileName1 [InCTFileName2 InCTFileName3] OutCTFileName1 [OutCTFileName2]  \n\n" 
       << "  Perform Homomorphic NOT, OR, AND, NOR, NAND, XOR, SUM computation.\n\n"
       << "  As their inputs gates use: 0 NOT  - InCTFileName1\n"
       << "                             1 OR   - InCTFileName1; InCTFileName2\n"
       << "                             2 AND  - InCTFileName1; InCTFileName2\n"
       << "                             3 NOR  - InCTFileName1; InCTFileName2\n"
       << "                             4 NAND - InCTFileName1; InCTFileName2\n"
       << "                             5 XOR  - InCTFileName1; InCTFileName2\n"
       << "                             6 SUM  - InCTFileName1; InCTFileName2; InCTFileName3 ( = Carry In)\n"
       << "  SUM also uses as its output:        OutCTFileName2 ( = Carry Out)\n\n";
  exit(0);
}

using namespace FHEW;

int main(int argc, char *argv[]) {

  if ((argc != 5) && (argc != 6) && (argc != 7)) help(argv[0]);

  int operation = atoi(argv[1]);
  char* ek_fn = argv[2];
  char* ict1_fn;
  char* ict2_fn;
  char* ict3_fn;
  char* oct_fn1;
  char* oct_fn2;

  LWE::CipherText *ct1, *ct2, *ct3, *ct_out1, *ct_out2;

  FHEW::Setup();

  if (operation == 0)
  {
    if (argc != 5)
      cout << "NOT requires one argument\n";
    ict1_fn = argv[3];
    oct_fn1 = argv[4];
    ct1 = LoadCipherText(ict1_fn);
  }
  else
  if (operation == 6)
  {
    if (argc != 8) 
      cout << "SUM requires three arguments\n";
    ict1_fn = argv[3];
    ict2_fn = argv[4];
    ict3_fn = argv[5];
    oct_fn1 = argv[6];
    oct_fn2 = argv[7];
    ct1 = LoadCipherText(ict1_fn);
    ct2 = LoadCipherText(ict2_fn);
    ct3 = LoadCipherText(ict3_fn);
  }
  else
  {
    if (argc != 6) 
      cout << argc << "This operation requires two arguments\n";
    ict1_fn = argv[3];
    ict2_fn = argv[4];
    oct_fn1 = argv[5];
    ct1 = LoadCipherText(ict1_fn);
    ct2 = LoadCipherText(ict2_fn);
  }

  EK = LoadEvalKey(ek_fn);

  ct_out1 = new LWE::CipherText;
  ct_out2 = new LWE::CipherText;

  switch (operation)
  {
    case 0:
      HomNOT(ct_out1, *EK, *ct1);
      break;
    case 1:
      HomOR(ct_out1, *EK, *ct1, *ct2);
      break;
    case 2:
      HomAND(ct_out1, *EK, *ct1, *ct2);
      break;
    case 3:
      HomNOR(ct_out1, *EK, *ct1, *ct2);
      break;
    case 4:
      HomNAND(ct_out1, *EK, *ct1, *ct2);
      break;
    case 5:
      HomXOR(ct_out1, *EK, *ct1, *ct2);
      break;
    case 6:
      HomSUM(ct_out1, ct_out2, *EK, *ct1, *ct2, *ct3);
      break;
    default:
      help(argv[0]);
  }

  SaveCipherText(ct_out1, oct_fn1);

  if (operation == 6)
    SaveCipherText(ct_out2, oct_fn2);
}
