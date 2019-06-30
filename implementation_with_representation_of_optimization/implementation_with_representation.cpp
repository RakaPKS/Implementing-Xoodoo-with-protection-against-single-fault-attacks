/*
 *  This representation shows where swaps are needed to run this implementation of Xoodoo with only 14 usable registers
 *  a00...a23, s0...s3 represent the variable stored in the RAM
 *  r0...r11, v0, v1 represent the registers
 */

#include <iostream>
#include <stdio.h>



using namespace std;
uint32_t r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10, r11, v0, v1;
uint32_t a00, a01, a02, a03, a10, a11, a12, a13, a20, a21, a22, a23, s0, s1, s2,
		s3;
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
 * Loads state into Registers
 */
void initialize_registers() {
	r0 = state[0];
	r1 = state[1];
	r2 = state[2];
	r3 = state[3];
	r4 = state[4];
	r5 = state[5];
	r6 = state[6];
	r7 = state[7];
	r8 = state[8];
	r9 = state[9];
	r10 = state[10];
	r11 = state[11];
}
/**
 * Writes values of registers to RAM
 */
void finalize_registers() {
	a00 = r0;
	a01 = r1;
	a02 = r2;
	a03 = r3;
	a10 = r4;
	a11 = r5;
	a12 = r6;
	a13 = r7;
	a20 = r8;
	a21 = r9;
	a22 = r10;
	a23 = r11;
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

// Assumes s0 == v0, s1 == v1
void VerifyEqualsFirstHalf() {
	//Verifies if the first sheet of parity is equal to the sum of the first sheet
	if (v0 != (r0 ^ r4 ^ r8)) {
		printf("Parity not equal to current State in first sheet\n");
		exit(1);
	}
	//Verifies if the second sheet of parity is equal to the sum of the second sheet
	if (v1 != (r1 ^ r5 ^ r9)) {
		printf("Parity not equal to current State in second sheet\n");
		exit(1);
	}
}

//Assumes s2 == v0, s3 == v1
void VerifyEqualsSecondHalf() {
	//Verifies if the third sheet of parity is equal to the sum of the third sheet
	if (v0 != (r2 ^ r6 ^ r10)) {
		printf("Parity not equal to current State in third sheet\n");
		exit(1);
	}
	//Verifies if the fourth sheet of parity is equal to the sum of the fourth sheets
	if (v1 != (r3 ^ r7 ^ r11)) {
		printf("Parity not equal to current State in fourth sheet\n");
		exit(1);
	}
}

void Theta() {
	v0 = s3;
	v1 = v0 << 5 ^ v0 << 14;					//Calculate S of first sheet
	r0 ^= v1;													//Calculate a00'
	v1 = v0 << 5 ^ v0 << 14;
	r4 ^= v1;                           						//Calculate a10'
	v1 = v0 << 5 ^ v0 << 14;
	r8 ^= v1;                           						//Calculate a20'
	v1 = s0;
	v0 = v1 ^ v0 << 5 ^ v0 << 14; 			//Calculate s0'
	s0 = v0;

	v0 = v1 << 5 ^ v1 << 14; 					//Calculate S of second sheet
	r1 ^= v0; 													//Calculate a01'
	v0 = v1 << 5 ^ v1 << 14;
	r5 ^= v0;													//Calculate a11'
	v0 = v1 << 5 ^ v1 << 14;
	r9 ^= v0; 													//Calculate a21'
	v0 = s1;
	v1 = v0 ^ v1 << 5 ^ v1 << 14; 			//Calculate s1'
	s1 = v1;

	v1 = v0 << 5 ^ v0 << 14; 					//Calculate P of third sheet
	r2 ^= v1; 													//Calculate a02'
	v1 = v0 << 5 ^ v0 << 14;
	r6 ^= v1; 													//Calculate a12'
	v1 = v0 << 5 ^ v0 << 14;
	r10 ^= v1;												 //Calculate a22'
	v1 = s2;
	v0 = v1 ^ v0 << 5 ^ v0 << 14;
	s2 = v0;

	v0 = v1 << 5 ^ v1 << 14; 					//Calculate P of fourth sheet
	r3 ^= v0; 													//Calculate a03'
	v0 = v1 << 5 ^ v1 << 14;
	r7 ^= v0; 													//Calculate a13'
	v0 = v1 << 5 ^ v1 << 14;
	r11 ^= v0; 												//Calculate a23
	v0 = s3;
	v1 = v0 ^ v1 << 5 ^ v1 << 14;
	s3 = v1;

	v0 = s2;
	VerifyEqualsSecondHalf();
	v0 = s0;
	v1 = s1;
	VerifyEqualsFirstHalf();
}

void Rho_west() {
	v0 ^= r4 ^ r8 ^ r5 ^ r8 << 11;  // Calculate s0'
	s0 = v0;

	v1 ^= r5 ^ r9 ^ r6 ^ r9 << 11; // Calculate s1'
	s1 = v1;

	v0 = s2;
	v0 ^= r6 ^ r10 ^ r7 ^ r10 << 11; // Calculate s2'
	s2 = v0;

	v1 = s3;
	v1 ^= r7 ^ r11 ^ r4 ^ r11 << 11; // Calculate s3'
	s3 = v1;

	//ROTL of a1to itself to calculate a1'
	v1 = r4;
	r4 = r5;
	r5 = r6;
	r6 = r7;
	r7 = v1;

	//ROTL of a2 to itself to calculate a2'
	r8 = r8 << 11;
	r9 = r9 << 11;
	r10 = r10 << 11;
	r11 = r11 << 11;

	v1 = s3;
	VerifyEqualsSecondHalf();
	v0 = s0;
	v1 = s1;
	VerifyEqualsFirstHalf();
}

void Iota(uint32_t rc) {
	//Load parity and Roundconstant
	v1 = rc;
	v0 ^= v1;
	r0 ^= v1;
	s0 = v0;

	//Verifies if the first sheet of parity is equal to the sum of the first sheets
	if (v0 != (r0 ^ r4 ^ r8)) {
		printf("Parity not equal to current State in first sheet\n");
		exit(1);
	}
}

void Chi() {
	v1 = s1;
	//Calculate first and second sheet of a_0' and also apply this to s0' and s1'
	r0 ^= ~r4 & r8;
	v0 ^= ~r4 & r8;
	r1 ^= ~r5 & r9;
	v1 ^= ~r5 & r9;

	// Calculate first and second sheet of a_1' and also apply this to s0' and s1'
	r4 ^= ~r8 & r0;
	v0 ^= ~r8 & r0;
	r5 ^= ~r9 & r1;
	v1 ^= ~r9 & r1;

	//Calculate first and second sheet of a_2' and also apply this to s0' and s1'
	r8 ^= ~r0 & r4;
	v0 ^= ~r0 & r4;
	r9 ^= ~r1 & r5;
	v1 ^= ~r1 & r5;

	s0 = v0;
	s1 = v1;
	VerifyEqualsFirstHalf();

	v0 = s2;
	v1 = s3;

	//Calculate third and fourth sheet of a_0' and also apply this to s2' and s3'
	r2 ^= ~r6 & r10;
	v0 ^= ~r6 & r10;
	r3 ^= ~r7 & r11;
	v1 ^= ~r7 & r11;

	//Calculate third and fourth sheet of a_1' and also apply this to s2' and s3'
	r6 ^= ~r10 & r2;
	v0 ^= ~r10 & r2;
	r7 ^= ~r11 & r3;
	v1 ^= ~r11 & r3;

	//Calculate third and fourth sheet of a_2' and also apply this to s2' and s3'
	r10 ^= ~r2 & r6;
	v0 ^= ~r2 & r6;
	r11 ^= ~r3 & r7;
	v1 ^= ~r3 & r7;

	s2 = v0;
	s3 = v1;

	VerifyEqualsSecondHalf();
}

void Rho_east() {
	v0 ^= r6 ^ r10 ^ r6 << 1 ^ r10 << 11; // Calculate s2'
	s2 = v0;

	v1 ^= r7 ^ r11 ^ r7 << 1 ^ r11 << 11; // Calculate s3'
	s3 = v1;

	//Calculate third and fourth sheet of a1'
	r6 = r6 << 1;
	r7 = r7 << 1;

	//Calculate third and fourth sheet of a2'
	r10 = r10 << 11;
	r11 = r11 << 11;

	VerifyEqualsSecondHalf();

	v0 = s0;
	v0 ^= r4 ^ r8 ^ r4 << 1 ^ r8 << 11; // Calculate s0'
	s0 = v0;

	v1 = s1;
	v1 ^= r5 ^ r9 ^ r5 << 1 ^ r9 << 11; // Calculate s1'
	s1 = v1;

	//Calculate first and second sheet of a0'
	r4 = r4 << 1;
	r5 = r5 << 1;

	//Calculate first and second sheet of a0'
	r8 = r8 << 11;
	r9 = r9 << 11;

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
	initialize_registers();

	//Computes Roundconstant
	calculateRoundConstants();
	uint32_t rc = (rc_p[0 % 7] ^ 8) << rc_s[0 % 6];

	round(rc);
	finalize_registers();
	finalize_state();
}
void six_rounds(uint32_t state[12]) {
	initialize_state();
	initialize_registers();
  calculateRoundConstants();
	for (int i = 0; i < 6; i++) {
	uint32_t rc = (rc_p[1 - i % 7] ^ 8) << rc_s[1 - i % 6];
		round(rc);
	}
	finalize_registers();
	finalize_state();
}

void twelve_rounds(uint32_t state[12]) {
	initialize_state();
  calculateRoundConstants();
	for (int i = 0; i < 12; i++) {
		uint32_t rc = (rc_p[1 - i % 7] ^ 8) << rc_s[1 - i % 6];
		round(rc);
	}
	finalize_registers();
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
