# FHEW-extensions 

## An extended interface for a Fully Homomorphic Encryption library

**version 0.1** -- *Release date: 2015.10.30*

**Authors:** Stephan Rybakov <stephan.rybakov@gmail.com>

FHEW-extensions is open-source software distributed under the terms of the GNU
General Public License. See the file LICENSE for complete details on the licensing of FHEW. 

### Requirements 

FHEW-extensions requires:
- The FFTW 3 library available at <http://www.fftw.org/download.html>,
- The FHEW library itself,
- and a c++ compiler. 

### Installation

If you installed the FHEW library to its default path, don't forget to include
~/include and ~/lib into your environmental variables.

To build the library, just run ```make```.  This will produce a
library (libfhew-extensions.a), and a number of test/example programs. (See
below.) You can also run ``` make install``` to install the header
files and fhew-extensions library required to compile your own programs making
use of FHEW. By default, these are also installed in $(HOME)/include and
$(HOME)/lib. Edit the Makefile to fit your needs. 

### Command Line Usage

FHEW-extensions is a library and was not designed to be used directly from the
command line, but we do provide a command line interface for testing purposes. 
The *cmd* subfolder contains four programs, which allow user to test several
Homomorphic boolean functions: NOT, AND, OR, NAND, NOR, XOR, SUM, as well as
SUM32 - 32 bit unsigned int addition.

Example 1:
```
gen sec.key ev.key
enc 0 sec.key a.ct
enc 1 sec.key b.ct
bit_oper 4 ev.key a.ct b.ct c.ct
dec sec.key c.ct
--- 
Example 2:
gen sec.key ev.key
enc32 12 sec.key a.ct
enc32 15 sec.key b.ct
enc32 0 sec.key c.ct
sum32 ev.key a.ct b.ct c_in.ct sum.ct c_out.ct
dec32 sec.key sum.ct
```
Example 1 explanation:
This generates a secret key and corresponding evaluation key, which
are stored in the files sec.key and ev.key. 
The secret key is then used to encrypt the message 0 and the message
1, storing the ciphertexts in a.ct and b.ct.
The homomorphic operation №4 (see ./bit_oper) of the two ciphertexts is computed using the evaluation key. Finally, the resulting ciphertext stored in c.ct is
decrypted.

### Library Interface

You can find the Library Interface in FHEW-extensions.h
