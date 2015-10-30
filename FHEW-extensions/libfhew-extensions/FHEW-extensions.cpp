#include <iostream>
#include <cstdlib>
#include "FHEW-extensions.h"
#include <cassert>

using namespace std;
namespace FHEW {

  void HomNOT(LWE::CipherText* res, const EvalKey& EK, const LWE::CipherText& ct) {

    HomNAND(res, EK, ct, ct);

  }

  void HomAND(LWE::CipherText* res, const EvalKey& EK, const LWE::CipherText& ct1, const LWE::CipherText& ct2) {

    LWE::CipherText temp;

    HomNAND(&temp, EK, ct1, ct2);
    HomNOT(res, EK, temp);

  }

  void HomOR(LWE::CipherText* res, const EvalKey& EK, const LWE::CipherText& ct1, const LWE::CipherText& ct2) {

    LWE::CipherText temp1;
    LWE::CipherText temp2;

    HomNOT(&temp1, EK, ct1);
    HomNOT(&temp2, EK, ct2);
    HomNAND(res, EK, temp1, temp2);

  }

  void HomNOR(LWE::CipherText* res, const EvalKey& EK, const LWE::CipherText& ct1, const LWE::CipherText& ct2) {

    LWE::CipherText res_or;

    HomOR(&res_or, EK, ct1, ct2);
    HomNOT(res, EK, res_or);

  }

  void HomXOR(LWE::CipherText* res, const EvalKey& EK, const LWE::CipherText& ct1, const LWE::CipherText& ct2) {

    LWE::CipherText temp;
    LWE::CipherText temp1;
    LWE::CipherText temp2;

    HomNAND(&temp, EK, ct1, ct2);
    HomNAND(&temp1, EK, ct1, temp);
    HomNAND(&temp2, EK, ct2, temp);
    HomNAND(res, EK, temp1, temp2);

  }

  void HomSUM(LWE::CipherText* sum, LWE::CipherText* carry_out, const EvalKey& EK, const LWE::CipherText& ct1, const LWE::CipherText& ct2, const LWE::CipherText& carry_in) {

    LWE::CipherText u1, u2, u3, u4, u5, u6, u7;
    
    HomNAND(&u1, EK, ct1, ct2); 
    HomNAND(&u2, EK, ct1, u1); 
    HomNAND(&u3, EK, ct2, u1); 
    HomNAND(&u4, EK, u2, u3); 
    HomNAND(&u5, EK, u4, carry_in); 
    HomNAND(&u6, EK, u4, u5); 
    HomNAND(&u7, EK, u5, carry_in); 
    HomNAND(sum, EK, u6, u7); 
    HomNAND(carry_out, EK, u1, u5);
 
  }

  void HomSUM32(LWE::CipherText* sum, LWE::CipherText* carry_out, const EvalKey& EK, const LWE::CipherText* ct1, const LWE::CipherText* ct2, const LWE::CipherText& carry_in) {
    LWE::CipherText* carry;
    
    carry = new LWE::CipherText[31];

    cout << "." << flush;

    HomSUM(&sum[31], &carry[30], EK, ct1[31], ct2[31], carry_in);

    cout << "." << flush;

    for (int i = 30; i > 0; i--)
    {
        HomSUM(&sum[i], &carry[i-1], EK, ct1[i], ct2[i], carry[i]);
        cout << "." << flush;
    }

    HomSUM(&sum[0], carry_out, EK, ct1[0], ct2[0], carry[0]);
    cout << ".\n";
  }

}
