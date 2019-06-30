/*
 * This is a Xoodoo implementation that protects it against fault attacks.
 */

#include <iostream>
#include <stdio.h>

using namespace std;
uint32_t a00, a01, a02, a03, a10, a11, a12, a13, a20, a21, a22, a23, s0, s1, s2,
    s3;
uint32_t temp1;
uint32_t temp2;
uint32_t rc_s[6];
uint32_t rc_p[7];
uint32_t state[12];


/**
 * Loads state into RAM
 */
void initialize_state() {
  a00 = state[0];
  a01 = state[1];
  a02 = state[2];
  a03 = state[3];
  a10 = state[4];
  a11 = state[5];
  a12 = state[6];
  a13 = state[7];
  a20 = state[8];
  a21 = state[9];
  a22 = state[10];
  a23 = state[11];
  s0 = a00 ^ a10 ^ a20;
  s1 = a01 ^ a11 ^ a21;
  s2 = a02 ^ a12 ^ a22;
  s3 = a03 ^ a13 ^ a23;
}

/**
 * Writes values in RAM back to state
 */
void finalize_state() {
  state[0] = a00;
  state[1] = a01;
  state[2] = a02;
  state[3] = a03;
  state[4] = a10;
  state[5] = a11;
  state[6] = a12;
  state[7] = a13;
  state[8] = a20;
  state[9] = a21;
  state[10] = a22;
  state[11] = a23;
}


void VerifyEqualsFirstHalf() {
  //Verifies if the first sheet of parity is equal to the sum of the first sheet
  if (s0 != (a00^ a10 ^ a20)) {
    printf("Parity not equal to current State in first sheet\n");
    exit(1);
  }
  //Verifies if the second sheet of parity is equal to the sum of the second sheet
  if (s1 != (a01 ^ a11 ^ a21)) {
    printf("Parity not equal to current State in second sheet\n");
    exit(1);
  }
}

void VerifyEqualsSecondHalf() {
  //Verifies if the third sheet of parity is equal to the sum of the third sheet
  if (s2!= (a02 ^ a12 ^a22)) {
    printf("Parity not equal to current State in third sheet\n");
    exit(1);
  }
  //Verifies if the fourth sheet of parity is equal to the sum of the fourth sheets
  if (s3 != (a03 ^ a13 ^ a23)) {
    printf("Parity not equal to current State in fourth sheet\n");
    exit(1);
  }
}

void Theta() {
  a00^= s3 << 5 ^ s3 << 14;												//Calculate a00'
  a10 ^= s3 << 5 ^ s3 << 14;                      //Calculate a10'
  a20 ^= s3 << 5 ^ s3 << 14;                      //Calculate a20'
  temp1 = s0;
  s0 ^= s3 << 5 ^ s3 << 14; 											//Calculate s0'

  a01 ^=	 temp1 << 5 ^ temp1<< 14; 							//Calculate a01'
  a11 ^= 	 temp1<< 5 ^ temp1<< 14; 								//Calculate a11'
  a21 ^= 	 temp1<< 5 ^ temp1 << 14; 							//Calculate a21'
  temp2= s1;
  s1 ^= temp1 << 5 ^ temp1 << 14; 								//Calculate s1'

  a02 ^= temp2 << 5 ^ temp2 << 14; 								//Calculate a02'
  a12 ^= temp2 << 5 ^ temp2 << 14; 								//Calculate a12'
  a22 ^= temp2 << 5 ^ temp2 << 14;								//Calculate a22'
  temp1 = s2;
  s2 ^= temp2 << 5 ^ temp2 << 14;  								//Calculate s2'


  a03 ^= temp1 << 5 ^ temp1<< 14; 								//Calculate a03'
  a13 ^=  temp1 << 5 ^ temp1<< 14; 								//Calculate a13'
  a23 ^=  temp1 << 5 ^ temp1<< 14; 								//Calculate a23
  s3 ^= temp1 << 5 ^ temp1<< 14;									//Calculate s3'

  VerifyEqualsSecondHalf();
  VerifyEqualsFirstHalf();

}

void Rho_west() {
  s0  ^= a10 ^ a20 ^ a11 ^ a20<<11;  //Calculate s0'
  s1 ^= a11 ^ a21 ^ a12 ^ a21 << 11; // Calculate s1'
  s2 ^= a12 ^ a22 ^ a13 ^ a22 << 11; // Calculate s2'
  s3 ^= a13 ^ a23 ^ a10 ^ a23 << 11; // Calculate s3'

  //ROTL of a1to itself to calculate a1'
  temp1= a10;
  a10 = a11;
  a11 = a12;
  a12 = a13;
  a13 = temp1;

  //ROTL of a2 to itself to calculate a2'
  a20 = a20 << 11;
  a21 = a21 << 11;
  a22 = a22 << 11;
  a23 = a23 << 11;
  VerifyEqualsSecondHalf();
  VerifyEqualsFirstHalf();
}

void Iota(uint32_t rc) {
  //Load parity and Roundconstant
  s0 ^= rc;
  a00 ^= rc;
  VerifyEqualsFirstHalf();
}

void Chi() {
  //Calculate first and second sheet of a_0' and also apply this to s0' and s1'
  a00 ^= ~a10 & a20;
  s0 ^= ~a10& a20;
  a01 ^= ~a11 & a21;
  s1 ^= ~a11& a21;

  // Calculate first and second sheet of a_1' and also apply this to s0' and s1'
  a10 ^= ~a20 & a00;
  s0 ^= ~a20 & a00;
  a11 ^= ~a21 & a01;
  s1 ^= ~a21 & a01;

  //Calculate first and second sheet of a_2' and also apply this to s0' and s1'
  a20 ^= ~a00 & a10;
  s0 ^= ~a00 & a10;
  a21 ^= ~a01 & a11;
  s1 ^= ~a01 & a11;

  VerifyEqualsFirstHalf();

  //Calculate third and fourth sheet of a_0' and also apply this to s2' and s3'
  a02 ^= ~a12 & a22;
  s2 ^= ~a12 & a22;
  a03 ^= ~a13 & a23;
  s3 ^= ~a13 & a23;

  //Calculate third and fourth sheet of a_1' and also apply this to s2' and s3'
  a12 ^= ~a22 & a02;
  s2 ^= ~a22 & a02;
  a13 ^= ~a23 & a03;
  s3 ^= ~a23 & a03;

  //Calculate third and fourth sheet of a_2' and also apply this to s2' and s3'
  a22 ^= ~a02 & a12;
  s2 ^= ~a02 & a12;
  a23 ^= ~a03 & a13;
  s3 ^= ~a03 & a13;

  VerifyEqualsSecondHalf();
}

void Rho_east() {
  s2 ^= a12 ^ a22 ^ a12 << 1 ^ a22 << 11; // Calculate s2'
  s3 ^= a13 ^ a23 ^ a13 << 1 ^ a23 << 11; // Calculate s3'

  //Calculate third and fourth sheet of a1'
  a12 = a12 << 1;
  a13 = a13 << 1;

  //Calculate third and fourth sheet of a2'
  a22 = a22 << 11;
  a23 = a23 << 11;

  VerifyEqualsSecondHalf();

  s0 ^= a10 ^ a20 ^ a10 << 1 ^ a20 << 11; // Calculate s0'

  s1 ^= a11 ^ a21 ^ a11 << 1 ^ a21 << 11; // Calculate s1'

  //Calculate first and second sheet of a0'
  a10 = a10 << 1;
  a11 = a11 << 1;

  //Calculate first and second sheet of a0'
  a20 = a20 << 11;
  a21 = a21 << 11;

  VerifyEqualsFirstHalf();
}

void round(uint32_t rc) {
  Theta();
  Rho_west();
  Iota(rc);
  Chi();
  Rho_east();
}

//Computes roundconstants
void calculateRoundConstants() {
  uint32_t s = 1;
  uint32_t p = 1;

  for (unsigned int i = 0; i < 6; i++) {
    rc_s[i] = s;
    s = (s * 5) % 7;
  }

  for (unsigned int i = 0; i < 7; i++) {
    rc_p[i] = (p);
    p = p ^ (p << 2);
    if ((p & 16) != 0)
      p ^= 22;
    if ((p & 8) != 0)
      p ^= 11;
  }

}

void single_round(uint32_t state[12]) {
  initialize_state();
  //Computes Roundconstant
  calculateRoundConstants();
  uint32_t rc = (rc_p[0 % 7] ^ 8) << rc_s[0 % 6];
  round(rc);
  finalize_state();
}
void six_rounds(uint32_t state[12]) {
  initialize_state();
  calculateRoundConstants();
  for (int i = 0; i < 6; i++) {
  uint32_t rc = (rc_p[1 - i % 7] ^ 8) << rc_s[1 - i % 6];
    round(rc);
  }
  finalize_state();
}

void twelve_rounds(uint32_t state[12]) {
  initialize_state();
  calculateRoundConstants();
  for (int i = 0; i < 12; i++) {
    uint32_t rc = (rc_p[1 - i % 7] ^ 8) << rc_s[1 - i % 6];
    round(rc);
  }
  finalize_state();

}
/*
 *  Main generates a random state and runs single_round() six_rounds() and twelve_rounds sequentially. The output gets printed .
 */

int main(void) {
  cout << "Initial state: " << endl;
  for (int i = 0; i < 12; i++) {
    state[i] = rand();
    cout << (state[i]) << endl;
  }

  single_round(state);
  six_rounds(state);
  twelve_rounds(state);

  cout << "\nFinal state: " << endl;
  for (int i = 0; i < 12; i++) {
    cout << (state[i]) << endl;
  }

  return 0;
}
