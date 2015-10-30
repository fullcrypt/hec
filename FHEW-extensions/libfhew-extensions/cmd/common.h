#ifndef COMMON_H
#define COMMON_H


#include <iostream>
#include <cstdlib>
#include <LWE.h>
#include <FHEW.h>
#include <cassert>
#include "../FHEW-extensions.h"

using namespace std;


void SaveSecretKey(const LWE::SecretKey* ct, char* filepath);
LWE::SecretKey* LoadSecretKey(char* filepath);


void SaveEvalKey(const FHEW::EvalKey *EK, char* filepath);
FHEW::EvalKey* LoadEvalKey(char* filepath);


void SaveCipherText(const LWE::CipherText* ct, char* filepath);
LWE::CipherText* LoadCipherText(char* filepath);

void SaveCipherText32(const LWE::CipherText* ct, char* filepath);
LWE::CipherText* LoadCipherText32(char* filepath);


#endif
