#ifndef FHEW_EXTENSIONS_H
#define FHEW_EXTENSIONS_H

#include <LWE.h>
#include <FHEW.h>

namespace FHEW {

  void HomNOT(LWE::CipherText* res, const EvalKey& EK, const LWE::CipherText& ct);
  void HomOR(LWE::CipherText* res, const EvalKey& EK, const LWE::CipherText& ct1, const LWE::CipherText& ct2);
  void HomAND(LWE::CipherText* res, const EvalKey& EK, const LWE::CipherText& ct1, const LWE::CipherText& ct2);
  void HomNOR(LWE::CipherText* res, const EvalKey& EK, const LWE::CipherText& ct1, const LWE::CipherText& ct2);
  void HomXOR(LWE::CipherText* res, const EvalKey& EK, const LWE::CipherText& ct1, const LWE::CipherText& ct2);
  void HomSUM(LWE::CipherText* sum, LWE::CipherText* carry_out, const EvalKey& EK, const LWE::CipherText& ct1, const LWE::CipherText& ct2, const LWE::CipherText& carry_in);
  void HomSUM32(LWE::CipherText* sum, LWE::CipherText* carry_out, const EvalKey& EK, const LWE::CipherText* ct1, const LWE::CipherText* ct2, const LWE::CipherText& carry_in);

}

#endif
