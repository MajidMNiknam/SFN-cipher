// SFN-related-key-recovery.cpp : Defines the entry point for the console application.
//

# include "stdafx.h"
# include <iostream>
# include <stdint.h>
# include <stdio.h>
# include <stdlib.h>
# include <fstream>	//file header
# include <time.h>
using namespace std;

#define number_of_test (5)
#define max_number_of_trials (2)                      // the max_number_of_trials can be 5 maximum.
#define max_of_iteration_of_iteration (30)            
#define number_of_iteration (900)                     // the number of iterations or Rep.
#define Nib_Rot_left_number (0)                       // the number of nibbles rotation to left (from lower values to higher values) for input differences (diff_in).               

#define diff_in_1   (0x00060000000b0002)         // input and output of characteristic number 1.
#define diff_out1_1 (0x000b000900030000)
//
#define diff_out2_1 (0) 
#define diff_out3_1 (0)
#define number_of_output_1 (1)
//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

#define diff_in_2   (0x0007000000040003)
#define diff_out1_2 (0x000b000900030000) 
//
#define diff_out2_2 (0) 
#define diff_out3_2 (0)
#define number_of_output_2 (1)
//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

#define diff_in_3   (0x0004000000040003)
#define diff_out1_3 (0x000b000900030000) 
#define diff_out2_3 (0x0008000700010000) 
#define diff_out3_3 (0)
#define number_of_output_3 (2)
//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

#define diff_in_4   (0x00040000000b0002)
#define diff_out1_4 (0x000b000900030000)
#define diff_out2_4 (0x000b000700040000) 
#define diff_out3_4 (0)
#define number_of_output_4 (2)
//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

#define diff_in_5   (0x0004000000070003)
#define diff_out1_5 (0x000b000900030000)
#define diff_out2_5 (0x0006000f00040000)
#define diff_out3_5 (0x000e000800050000) 
#define number_of_output_5 (3)
//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
void intersection_of_two_sets( int A[17], int B[17], int C[17] ) {
	int h = 0;
	for ( int i = 1; i <= A[0] ; i++ )
		for ( int j = 1; j <= B[0] ; j++ ) 
			if ( A[i] == B[j] ) {
               h++;
			   C[h] = A[i];
			   j = 16;
			}
	C[0] = h;
}


//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
void multiply_two_matrix_in_field(uint8_t A[4][4], uint8_t B[4][4], uint8_t C[4][4], int power_of_2[15], int power_of_element[15] ){
	for ( int i = 0; i < 4; i++ )
		for ( int j = 0; j < 4; j++ ) {
			C[i][j] = 0;
			for ( int k = 0; k < 4; k++ )
				if ( (A[i][k] != 0) & (B[k][j] != 0) ) C[i][j] ^= power_of_2[(power_of_element[A[i][k]] + power_of_element[B[k][j]]) % 15];
		}
}

void P_layer( uint8_t v[8] ) {
	int permutarion[32] = { 9,28,7,13,8,12,29,6,0,2,17,23,30,24,18,11,31,4,15,19,5,1,25,27,3,10,22,21,26,16,20,14 };

	int v_bit[32];
	int pv_bit[32];
   
	for ( int j = 0; j < 8; j++ ) {
		int hv = v[j];
		for (int k = 0; k < 4; k++ ) {
			v_bit[4 * j + k] = hv % 2;
			hv = hv / 2;
		}
	}

	for ( int j = 0; j < 32; j++ ) {
		pv_bit[permutarion[j]] = v_bit[j];
	}

	for ( int j = 0; j < 8; j++ ) {
		int hpv = 0;
		for (int k = 3; k >= 0; k-- ) {
			hpv = 2 * hpv + pv_bit[4 * j + k];
		}
		v[j] = hpv;
	}
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
void MixXors( uint8_t v[8] ) {
	//int help_vector[8] ;

    //help_vector[0] = v[1] ^ v[2] ^ v[3] ^ v[4] ^ v[5] ^ v[6];
    //help_vector[1] = v[0] ^ v[2] ^ v[3] ^ v[5] ^ v[6] ^ v[7];
    //help_vector[2] = v[0] ^ v[1] ^ v[3] ^ v[4] ^ v[6] ^ v[7];
    //help_vector[3] = v[0] ^ v[1] ^ v[2] ^ v[4] ^ v[5] ^ v[7];
    //help_vector[4] = v[0] ^ v[1] ^ v[3] ^ v[4] ^ v[5];
    //help_vector[5] = v[0] ^ v[1] ^ v[2] ^ v[5] ^ v[6];
    //help_vector[6] = v[1] ^ v[2] ^ v[3] ^ v[6] ^ v[7];
    //help_vector[7] = v[0] ^ v[2] ^ v[3] ^ v[4] ^ v[7];
	//
	//for ( int j = 0; j < 8; j++ ) v[j] = help_vector[j];

	v[7] = v[3] ^ v[7];
	v[6] = v[2] ^ v[6];
	v[5] = v[1] ^ v[5];
	v[4] = v[0] ^ v[4];
	
	v[1] = v[1] ^ v[7];
	v[0] = v[0] ^ v[6];
	v[3] = v[3] ^ v[5];
	v[2] = v[2] ^ v[4];
	
	v[4] = v[3] ^ v[4];
	v[7] = v[2] ^ v[7];
	v[6] = v[1] ^ v[6];
	v[5] = v[0] ^ v[5];
	
	v[3] = v[3] ^ v[7];
	v[2] = v[2] ^ v[6];
	v[1] = v[1] ^ v[5];
	v[0] = v[0] ^ v[4];
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
void creat_K0(uint64_t &K0, uint8_t S1_box[16], uint64_t round_key_31)
{
	uint32_t kh = round_key_31 & 0xffffffff ;

	// Addconstants
	//%%%%%%%%%%%%%
	kh ^= 0x1f;
	
	// Creating nibbles and First S1-box layer
	//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	uint8_t nibble[8];
	for ( int j = 0; j < 8; j++ ) {
		nibble[j] = kh & 0x0f;
		nibble[j] = S1_box[nibble[j]];
		kh = kh >> 4;
	}

	// Permutation layer
	//%%%%%%%%%%%%%%%%%%
	P_layer(nibble);

	// MixXors layer
	//%%%%%%%%%%%%%%
	MixXors(nibble);

	// Second S1-box layer
	//%%%%%%%%%%%%%%%%%%%%
	for ( int j = 0; j < 8; j++ ) 
		nibble[j] = S1_box[nibble[j]];

	// Creating K0
	//%%%%%%%%%%%%
	kh = 0;
	for ( int j = 8; j > -1; j-- ) 
		kh = (kh << 4) ^ nibble[j];
	kh = ((round_key_31 >> 32) & 0xffffffff) ^ kh;
	K0 = ((round_key_31 & 0xffffffff) << 32) ^ kh;
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
void creat_K1(uint64_t &K1, uint8_t S2_box[16], uint8_t Mix_column_row_matrix[4][4], int power_of_2[15], int power_of_element[16], uint64_t round_key_31)
{
	// Swapping pre and post half of the input_key
	//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	uint64_t kh = (round_key_31 << 32) ^ (round_key_31 >> 32);

	// AddConstants
	//%%%%%%%%%%%%%
	kh ^= 0x1f;
	
	// First S2-box layer
	//%%%%%%%%%%%%%%%%%%%
	uint8_t key_matrix[4][4];
	uint8_t result_matrix[4][4];

	for (int i = 0; i < 4; i++ )
		for (int j = 0; j < 4; j++ ) {
			key_matrix[i][j] = kh & 0x0f;
			kh = kh >> 4;
		}

	for (int i = 0; i < 4; i++ )
		for (int j = 0; j < 4; j++ ) 
	        key_matrix[i][j] = S2_box[key_matrix[i][j]];

	// MixColumns
	//%%%%%%%%%%%
	multiply_two_matrix_in_field( Mix_column_row_matrix, key_matrix, result_matrix, power_of_2, power_of_element );
	
	for ( int i = 0; i < 4; i++ )
	    for ( int j = 0; j < 4; j++ ) key_matrix[i][j] = result_matrix[i][j];

	// MixRows
	//%%%%%%%%
	multiply_two_matrix_in_field( key_matrix, Mix_column_row_matrix, result_matrix, power_of_2, power_of_element );

	// Second S2-box layer
	//%%%%%%%%%%%%%%%%%%%%
	for (int i = 0; i < 4; i++ )
		for (int j = 0; j < 4; j++ ) 
	        key_matrix[i][j] = S2_box[result_matrix[i][j]];

	// Creating K1
	//%%%%%%%%%%%%
	K1 = 0;
	for (int i = 3; i > -1; i-- )
	    for ( int j = 3; j > -1; j-- ) 
	    	K1 = (K1 << 4) ^ key_matrix[i][j];
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
uint64_t O_RK(uint64_t input_pt, uint8_t Mix_column_row_matrix[4][4], uint8_t S1_box[16], uint8_t S2_box[16], uint64_t K0, uint64_t K1, int power_of_2[15], int power_of_element[16], uint64_t round_key_31, int CK_31) {

	// using input text
	//%%%%%%%%%%%%%%%%%
	uint64_t c0 = input_pt;
	uint64_t ct;

	// First Addroundkey
	//%%%%%%%%%%%%%%%%%%
	if ( CK_31 == 0 )
	   c0 = c0 ^ K0;
	else 
       c0 = c0 ^ K1;


	if ( CK_31 == 0 ) {

	   // Swapping pre and post half of c0
	   //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	   ct = (c0 << 32) ^ (c0 >> 32);
	   
	   // First S1-box layer
	   //%%%%%%%%%%%%%%%%%%%
	   uint8_t state_matrix[4][4];
	   for (int i = 0; i < 4; i++ )
	   	for (int j = 0; j < 4; j++ ) {
	   		state_matrix[i][j] = ct & 0x0f;
	   		ct = ct >> 4;
	   	}
	   
	   for (int i = 0; i < 4; i++ )
	   	for (int j = 0; j < 4; j++ ) 
	   		state_matrix[i][j] = S1_box[state_matrix[i][j]];
	   
	   // MixRows
	   //%%%%%%%%
	   uint8_t result_matrix[4][4];
	   multiply_two_matrix_in_field( state_matrix, Mix_column_row_matrix, result_matrix, power_of_2, power_of_element );
	   
	   // MixColumns
	   //%%%%%%%%%%%
	   multiply_two_matrix_in_field(Mix_column_row_matrix, result_matrix, state_matrix, power_of_2, power_of_element );
	   
	   // Second S1-box layer
	   //%%%%%%%%%%%%%%%%%%%%
	   for (int i = 0; i < 4; i++ )
	   	for (int j = 0; j < 4; j++ ) 
	   		state_matrix[i][j] = S1_box[state_matrix[i][j]];
	   
	   // Second Addroundkey
	   //%%%%%%%%%%%%%%%%%%%
	   ct = 0;
	   for (int i = 3; i > -1; i--)
	   	for (int j = 3; j > -1; j--) 
	   		ct = (ct << 4) ^ state_matrix[i][j];
	   c0 = ct ^ round_key_31;
	   
	   // Swapping pre and post half of c0
	   // Creating 31st cipher of SFN 
	   //%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	   ct = (c0 << 32) ^ (c0 >> 32);
	   
	   // Third AddRoundKey
	   //%%%%%%%%%%%%%%%%%%
	   uint32_t ct_help = ct & 0xffffffff; 
	   ct_help ^= (round_key_31 & 0xffffffff);
	   
	   // First S2-box layer
	   //%%%%%%%%%%%%%%%%%%%
	   uint8_t nibble[8];
	   for (int i = 0; i < 2; i++ )
	   	for (int j = 0; j < 4; j++ ) {
	   		nibble[4 * i + j] = ct_help & 0x0f;
	   		ct_help = ct_help >> 4;
	   	}
	   
	   for (int i = 0; i < 2; i++ )
	   	for (int j = 0; j < 4; j++ ) 
	   		nibble[4 * i + j] = S2_box[nibble[4 * i + j]];
	   
	   // Permutation layer
	   //%%%%%%%%%%%%%%%%%%
	   P_layer(nibble);
	   
	   // MixXors layer
	   //%%%%%%%%%%%%%%
	   MixXors(nibble);
	   
	   // Second S2-box layer
	   //%%%%%%%%%%%%%%%%%%%%
	   for (int i = 0; i < 2; i++ )
	   	for (int j = 0; j < 4; j++ ) 
	   		nibble[4 * i + j] = S2_box[nibble[4 * i + j]];
	   
	   // Creating c0 before final AddRoundKey 
	   //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	   ct_help = 0;
	   for (int i = 1; i > -1; i--)
	   	for (int j = 3; j > -1; j--) 
	   		ct_help = (ct_help << 4) ^ nibble[4 * i + j];
       ct_help = ct_help ^ ((ct >> 32) & 0xffffffff);
	   c0 = ct_help;
	   c0 = (ct & 0xffffffff) ^ (c0 << 32);
	   
	   // Final AddRoundKey
	   //%%%%%%%%%%%%%%%%%%
        c0 = c0 ^ K1;
	}
	else {

	// second AddRoundKey
	//%%%%%%%%%%%%%%%%%%%
	uint32_t ct_help = c0 & 0xffffffff; 
	ct_help ^= (round_key_31 & 0xffffffff);
	
	// First S2-box layer
	//%%%%%%%%%%%%%%%%%%%
	uint8_t nibble[8];
	for (int i = 0; i < 2; i++ )
		for (int j = 0; j < 4; j++ ) {
			nibble[4 * i + j] = ct_help & 0x0f;
			ct_help = ct_help >> 4;
		}
	
	for (int i = 0; i < 2; i++ )
		for (int j = 0; j < 4; j++ ) 
			nibble[4 * i + j] = S2_box[nibble[4 * i + j]];
	
	// Permutation layer
	//%%%%%%%%%%%%%%%%%%
	P_layer(nibble);
	
	// MixXors layer
	//%%%%%%%%%%%%%%
	MixXors(nibble);
	
	// Second S2-box layer
	//%%%%%%%%%%%%%%%%%%%%
	for (int i = 0; i < 2; i++ )
		for (int j = 0; j < 4; j++ ) 
			nibble[4 * i + j] = S2_box[nibble[4 * i + j]];

	// creating the 31st ciphertext
	//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	ct = 0;
	for (int i = 1; i > -1; i--)
		for (int j = 3; j > -1; j--) {
			ct = (ct << 4) ^ nibble[4 * i + j];
		}
    c0 = c0 ^ (ct << 32);

	// Swapping pre and post half of c0
	//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	ct = (c0 << 32) ^ (c0 >> 32);

	// Second Addroundkey
	//%%%%%%%%%%%%%%%%%%%
	c0 = ct ^ round_key_31;

	// first S1-box layer
	//%%%%%%%%%%%%%%%%%%%
	uint8_t state_matrix[4][4];
	for (int i = 0; i < 4; i++ )
		for (int j = 0; j < 4; j++ ) {
			state_matrix[i][j] = c0 & 0x0f;
			c0 = c0 >> 4;
		}

	for (int i = 0; i < 4; i++ )
		for (int j = 0; j < 4; j++ ) 
			state_matrix[i][j] = S1_box[state_matrix[i][j]];
	
	// MixColumns
	//%%%%%%%%%%%
	uint8_t result_matrix[4][4];
	multiply_two_matrix_in_field(Mix_column_row_matrix,state_matrix,  result_matrix, power_of_2, power_of_element );
	
	// MixRows
	//%%%%%%%%
	multiply_two_matrix_in_field( result_matrix, Mix_column_row_matrix, state_matrix, power_of_2, power_of_element );
	
	// second S1-box layer
	//%%%%%%%%%%%%%%%%%%%%
	for (int i = 0; i < 4; i++ )
		for (int j = 0; j < 4; j++ ) 
			state_matrix[i][j] = S1_box[state_matrix[i][j]];
	
	// Creating c0 before swapping 
	//%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	c0 = 0;
	for (int i = 3; i > -1; i--)
		for (int j = 3; j > -1; j--) 
			c0 = (c0 << 4) ^ state_matrix[i][j];

	// Swapping pre and post of c0
	//%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	ct = (c0 << 32) ^ (c0 >> 32);

	// Final Addroundkey
	//%%%%%%%%%%%%%%%%%%
	c0 = ct ^ K0;
	}

	return c0;
}
// end of O_RK.
//&&&&&&&&&&&&&

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
int Algorithm2 (uint8_t Mix_column_row_matrix[4][4], uint8_t S1_box[16], uint8_t S2_box[16], uint64_t K0, uint64_t K1, int power_of_2[15], int power_of_element[16], uint64_t round_key_31, int round_signal_bit_31, ofstream &outputfile) {
	
	int CK_31 = 1;
	int n = number_of_iteration;
	int j = Nib_Rot_left_number;

	uint64_t c0 = 0;
	uint64_t c_prime0;
	uint64_t c1;
	uint64_t c_prime1;
	uint64_t diff_in_h;

	for ( int i = 0 ; i < n; i++ ) {
	
	    // choosing a random c0 and c_prime0=c0\oplus diff_in_h and printng them.
	    //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

		for ( int r = 0; r < 4; r++ ) {

			int t = rand();
			c0 = (c0 << 16) ^ t;
		}
	
		diff_in_h = diff_in_1 << (4 * j);
		c_prime0 = c0 ^ diff_in_h;
		
        c1 = O_RK(c0, Mix_column_row_matrix, S1_box, S2_box, K0, K1, power_of_2, power_of_element, round_key_31, round_signal_bit_31);
        c_prime1 = O_RK(c_prime0, Mix_column_row_matrix, S1_box, S2_box, K0, K1, power_of_2, power_of_element, round_key_31, round_signal_bit_31);
		
		if ( (((c1 ^ c_prime1) & 0xffff) == 0) & ((((c1 ^ c_prime1) >> 16) & 0xffff) == 0) ) {
			CK_31 = 0;
		    i = number_of_iteration;
		}
	} 
	
	//cout << "\n\n";
	//outputfile << "\n\n";
	//
	//cout << "c0 =" ;
	//outputfile << "c0 =" ;
	//for (int i = 3; i > -1; i-- ) {
	//	cout << " ";
	//	outputfile << " ";
	//	for (int j1 = 3; j1 > -1; j1-- ) {
	//	    cout << hex << ((c0 >> (16 * i + 4 * j1)) & 0x0f);
	//	    outputfile << hex << ((c0 >> (16 * i + 4 * j1)) & 0x0f);
	//	}
	//}
	//
	//cout << "\n\n";
	//outputfile << "\n\n";
	//
	//cout << "c_prime0 =" ;
	//outputfile << "c_prime0 =" ;
	//for (int i = 3; i > -1; i-- ) {
	//	cout << " ";
	//	outputfile << " ";
	//	for (int j = 3; j > -1; j-- ) {
	//	    cout << hex << ((c_prime0 >> (16 * i + 4 * j)) & 0x0f);
	//	    outputfile << hex << ((c_prime0 >> (16 * i + 4 * j)) & 0x0f);
	//	}
	//}
	
	cout << "\n\n";
	outputfile << "\n\n";
	
	cout << " found:\n CK_31 = " << CK_31 << "\n\n"; 
	outputfile << " found:\n CK_31 = " << CK_31 << "\n\n";

	return CK_31;                      // CK[31] is the found control signal bit of last round.
}
// end of Algorithm2
//&&&&&&&&&&&&&&&&&&

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

int Algorithm3 (int j, int m, uint8_t Mix_column_row_matrix[4][4], uint8_t S1_box[16], uint8_t S2_box[16], uint64_t K0, uint64_t K1, int power_of_2[15], int power_of_element[16], uint64_t round_key_31, int CTR[3][16], int J[3][17], ofstream &outputfile) {
	
	int count_cipher = 0;

	int a[3];
	int b[3];
	for ( int r = 0; r < 3; r++ )
	    switch (r) {
	        case 0: {
	        	a[0] = j;
	        	b[0] = 8;
	        }
	        case 1: {
	        	a[1] = 4 + j;
	        	b[1] = 12;
	        }
	        case 2: {
	        	a[2] = 12 + j;
	        	b[2] = 4;
	        }
	    }

	uint64_t diff_in_h;
    uint64_t diff_out_h[3];

	// creating the values of input and output differences.
	//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	int number_of_output;
	switch(m){ 
	   case 1: {
		       diff_in_h = diff_in_1 << (4 * j);
			   diff_out_h[0] = diff_out1_1;
	           diff_out_h[1] = diff_out2_1;
	           diff_out_h[2] = diff_out3_1;
			   number_of_output =number_of_output_1;
			   break;
			}
	   case 2: {
		       diff_in_h = diff_in_2 << (4 * j);
			   diff_out_h[0] = diff_out1_2;
	           diff_out_h[1] = diff_out2_2;
	           diff_out_h[2] = diff_out3_2;
			   number_of_output =number_of_output_2;
			   break;
			}
	   case 3: {
		       diff_in_h = diff_in_3 << (4 * j);
			   diff_out_h[0] = diff_out1_3;
	           diff_out_h[1] = diff_out2_3;
	           diff_out_h[2] = diff_out3_3;
			   number_of_output =number_of_output_3;
			   break;
			}
	   case 4: {
		       diff_in_h = diff_in_4 << (4 * j);
			   diff_out_h[0] = diff_out1_4;
	           diff_out_h[1] = diff_out2_4;
	           diff_out_h[2] = diff_out3_4;
			   number_of_output =number_of_output_4;
			   break;
			}
	   case 5: {
		       diff_in_h = diff_in_5 << (4 * j);
			   diff_out_h[0] = diff_out1_5;
	           diff_out_h[1] = diff_out2_5;
	           diff_out_h[2] = diff_out3_5;
			   number_of_output =number_of_output_5;
			   break;
			}
	   default:{
		       cout << " m must be between 1 and 5 , but it is not.";
			   outputfile << " m must be between 1 and 5 , but it is not.";
			}
	}

	int n0 = number_of_iteration;     // number_of_iteration is defined 2 at the first of the program.

	uint64_t c0;
	uint64_t c_prime0;
	uint64_t c1;
	uint64_t c_prime1;

	for ( int n = 0 ; n < n0 ; n++ ) {
        
		c0 = 0;
	    for ( int i = 0; i < 4; i++ ) {
	    	//srand (time(NULL));
	    	int t = rand();
	    	c0 = (c0 << 16) ^ t;
	    }
	    
	    c_prime0 = c0 ^ diff_in_h;
	            
	    // Quarring from the oracle O-RK.
		//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
        c1 = O_RK(c0, Mix_column_row_matrix, S1_box, S2_box, K0, K1, power_of_2, power_of_element, round_key_31, 0);
        c_prime1 = O_RK(c_prime0, Mix_column_row_matrix, S1_box, S2_box, K0, K1, power_of_2, power_of_element, round_key_31, 0);
	            
	    if ( (((c1 ^ c_prime1) & 0xffff) == 0) & ((((c1 ^ c_prime1) >> 16) & 0xffff) == 0) ) { 
	          
		    int xh[2];
		    int yh[2];
	        count_cipher++;

	        for ( int r = 0; r < 3; r++ ) {

	            if ( J[r][0] > 1) {
	               xh[0] = (c0 >> (4 * a[r])) & 0x000f;
	               xh[1] = (c_prime0 >> (4 * a[r])) & 0x000f;
	                  
	               for ( int K1 = 0 ; K1 < 16 ; K1++ ) {
	               
	                   yh[0] = S1_box[xh[0] ^ K1]; 
	                   yh[1] = S1_box[xh[1] ^ K1];
	               
	                   for ( int l = 0; l < number_of_output; l++ ) 
	    	   	           if ( (yh[0] ^ yh[1]) == ((diff_out_h[l] >> (4 * b[r])) & 0x0f) ) 
	    		                         CTR[r][K1]++ ;
	               }
			    }
	    	}
	    }
	}

	return count_cipher;                     // count_cipher is the number of pairs ciphers with specified input/output differential.
}
// end of Algorithm3
//&&&&&&&&&&&&&&&&&&

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

int Algorithm4 (uint8_t Mix_column_row_matrix[4][4], uint8_t S1_box[16], uint8_t S2_box[16], uint64_t K0, uint64_t K1, int power_of_2[15], int power_of_element[16]
                , uint64_t round_key_31, int &number_of_pairs, int finding_three_nibbles_flag, ofstream &outputfile) {

	int j = Nib_Rot_left_number;

	int a[3];
	int J[3][17];
	int MS[3][17];
	for ( int r = 0; r < 3; r++ ) {
		a[r] = j + (4 * (r + r / 2));
        J[r][0] = 16;
		MS[r][0] = 0;
		for ( int j = 1; j < 17; j++ ) {
			J[r][j] = j - 1;
			MS[r][j] = 0;
		}
	}
			
    int count_cipher = 0;
	int Max_CTR[3];
	
	int CTR[3][16];
	for (int r = 0; r < 3; r++ ) {
		Max_CTR[r] = 0;
		for ( int j = 0 ; j < 16; j++ ) 
			CTR[r][j] = 0; 
	}

	int n0 = number_of_iteration;     // number_of_iteration is defined 2 at the first of the program.

	uint64_t diff_in_h;
    uint64_t diff_out_h[3];

	uint64_t c0;
	uint64_t c_prime0;
	uint64_t c1;
	uint64_t c_prime1;

	for ( int Rep = 1; Rep < max_of_iteration_of_iteration + 1; Rep++ ) {
	
	    // m is the number of trail which the program use.
	    for ( int m = 1; m < max_number_of_trials + 1; m++ ) {
	        
	        // creating three sets CTR[r], (Algorithm 3).
	        //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
			int count_cipher = Algorithm3 (j, m, Mix_column_row_matrix, S1_box, S2_box, K0, K1, power_of_2, power_of_element, round_key_31, CTR, J, outputfile);
	        
			// finding three numbers Max_CTR[r], r= 0,1,2.
			//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	        for (int r = 0; r < 3; r++ )
				if ( J[r][0] > 1) 
	               for ( int K1 = 0 ; K1 < 16 ; K1++ ) 
	               	   if ( CTR[r][K1] >  Max_CTR[r] )
	               	   	  Max_CTR[r] = CTR[r][K1];
	            
	        //  creating the three sets MS[r], r= 0,1,2.
			//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	        int h; 
	        for (int r = 0; r < 3; r++ ) {

				if ( J[r][0] > 1) {
	        	   h = 0;
	               for ( int K1 = 0 ; K1 < 16 ; K1++ ) 
                       if ( CTR[r][K1] == Max_CTR[r] ) {
                          h++;
	               	      MS[r][h] = K1;
	               	   }
	        	   MS[r][0] = h; 
				}
	        }

	        // updating the three sets J[r], r= 0,1,2.
			//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	        for (int r = 0; r < 3; r++ )
				if ( J[r][0] > 1)
	            intersection_of_two_sets(J[r], MS[r], J[r]);

	        if ( (J[0][0] == 1) & (J[1][0] == 1) & (J[2][0] == 1) ) {
				number_of_pairs = ((Rep - 1) * max_number_of_trials + m) * number_of_iteration;

	            //cout << dec << "\n\n the number of the final trail = " << m << "\n Rep = " 
	            //	 << Rep << "\n\n" << " the number of pairs = " << number_of_pairs << "\n\n";
	            //outputfile << dec << "\n\n the number of the final trail = " << m << "\n Rep = " 
	            //	 << Rep << "\n\n"<< " the number of pairs = " << number_of_pairs << "\n\n";
				
		    	finding_three_nibbles_flag = 1;
				m = number_of_iteration + 1;
		    	Rep = max_of_iteration_of_iteration + 1;
	        }
	    }
	}
	if ( finding_three_nibbles_flag == 1 ) {

	   for (int r = 0; r < 3; r++ ) {
	       cout << " K0[" << dec << a[r] << "] = " << hex << J[r][1] << "\n";
	       outputfile << " K0[" << dec << a[r] << "] = " << hex << J[r][1] << "\n";
	       if ( r == 2 ) {
	                cout << "\n*************************\n";
	                outputfile << "\n*************************\n";
	       }
	   }

	   //int temp;
	   //for ( int r = 0; r < 3; r++ ) {
	   //	temp = ((K0 >> (4 * a[r])) & 0x0f);
	   //	if ( J[r][1] != temp ) {
	   //	   cout << "    K0 =[" << dec << a[r] << "] = " << hex << temp;
	   //	   outputfile << "    K0 =[" << dec << a[r] << "] = " << hex << temp;
	   //	   int p;
	   //	   cout << " p = "; cin >> p;
	   //	}
	   //}
	}
	else { 
		
		cout << "\n\n RK^31 =" ;
	    outputfile << "\n\n RK^31 =" ;
	    for (int i = 3; i > -1; i-- ) {
	    	cout << " ";
	    	outputfile << " ";
	    	for (int j = 3; j > -1; j-- ) {
	    	    cout << hex << ((round_key_31 >> (16 * i + 4 * j)) & 0x0f);
	    	    outputfile << hex << ((round_key_31 >> (16 * i + 4 * j)) & 0x0f);
	    	}
	    }
	    cout << "\n\n";
	    outputfile << "\n\n";
	    
	    cout << "    K0 =" ;
	    outputfile << "    K0 =" ;
	    for (int i = 3; i > -1; i-- ) {
	    	cout << " ";
	    	outputfile << " ";
	    	for (int j = 3; j > -1; j-- ) {
	    	    cout << hex << ((K0 >> (16 * i + 4 * j)) & 0x0f);
	    	    outputfile << hex << ((K0 >> (16 * i + 4 * j)) & 0x0f);
	    	}
	    }
		
		cout << "\n This program can not find the three nibble of the K0, for this"; 
		cout << " purpose we have to increase the value of m or Rep or both of them. \n";
		outputfile << "\n This program can not find the three nibble of the K0, for this";
		outputfile << " purpose we have to increase the value of m or Rep or both of them. \n";
		int w1 ; cout << "\n\n w1 = "; cin >> w1 ;
	}

	return finding_three_nibbles_flag;              // finding_three_nibbles_flag = 1 iff finding three nibbles of K0 is succesfull.
}
// end of Algorithm4
//&&&&&&&&&&&&&&&&&&

// the main program
//*****************
//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
int main()
{
	ofstream outputfile("key_recovery_diff_SFN.txt", std::ofstream::app);
	if (!outputfile)
	{
		cerr << "some thing wrong during opening file!" << endl;
		int w2; cout << "\n w2 = "; cin >> w2;
		exit(1);
	}

	uint8_t S1_box[16] = { 12,10,13,3,14,11,15,7,8,9,1,5,0,2,4,6 } ;
	uint8_t S2_box[16] = { 11,15,3,2,10,12,9,1,6,7,8,0,14,5,13,4 } ;

	uint8_t Mix_column_row_matrix[4][4] = { {1,2,6,4},
	                                        {2,1,4,6},
	                                        {6,4,1,2},
	                                        {4,6,2,1} 
	                                      };
	int power_of_2[15]; 
	int power_of_element[16];
	power_of_element[0] = -1;
 
	for ( int i = 0; i < 15; i++ ) {                                             // power_of_2[i] is 2^i in the field GF(16) with 16 element.
		if ( i == 0 ) power_of_2[i] = 1;
		else if (power_of_2[i - 1] < 8) power_of_2[i] = 2 * power_of_2[i - 1];
		     else power_of_2[i] = ((power_of_2[i - 1] << 1) & 0x0f) ^ 3;
	}

	for ( int i = 1; i < 16; i++ ) {                                             // power_of_element[i] = j  when i = 2^j in the field GF(16) with 16 element.
		for ( int i1 = 0; i1 < 15; i1++ )
		if (power_of_2[i1] == i) {
		    power_of_element[i] = i1;
			i1 = 15;
		}
	}

	int number_of_pairs;
	double average_of_number_of_pairs = 0;
	int max_of_number_of_pairs = 0;
	int finding_three_nibbles_flag;

	int j = Nib_Rot_left_number;

	//###############################################
	// Starting key recovery ni times.
	//********************************
	//###############################################
	for ( int ni = 1; ni < number_of_test + 1; ni++ ) {
		//if ( (ni % 100000) == 0 ) {
		//	cout << "\n\n ni = " << dec << ni << "\n the average of number of pairs = " 
		//		 << dec << average_of_number_of_pairs << "\n the max of number of pairs = " 
		//		 << dec << max_of_number_of_pairs << "\n the number of rotations = " << j ;
		//	outputfile << "\n\n ni = " << ni << "\n the average of number of pairs = " 
		//		 << dec << average_of_number_of_pairs << "\n the max of number of pairs = " 
		//		 << dec << max_of_number_of_pairs << "\n the number of rotations = " << j ;
		//}

		if ( ni == 33 )
			cout << "&*&\n";

		number_of_pairs = 0;
		finding_three_nibbles_flag = 0;

	    uint64_t diff_in_h;
        uint64_t diff_out_h[3];

		// choosing a random main 31 round key.
		//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	    uint64_t round_key_31 = 0;
	    for ( int i = 0; i < 4; i++ ) {
	    	int t = rand();
	    	round_key_31 = (round_key_31 << 16) ^ t;
	    }
		
		// choosing a random 31 round signal bit.
		//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
		int round_signal_bit_31;
		round_signal_bit_31 = rand() & 0x01;
	    
	    // creating round keys K0, K1
	    //%%%%%%%%%%%%%%%%%%%%%%%%%%%
	    uint64_t K0 = 0; 
	    creat_K0(K0, S1_box, round_key_31);
	    
	    uint64_t K1 = 0;
	    creat_K1(K1, S2_box, Mix_column_row_matrix, power_of_2, power_of_element, round_key_31);
	    
	    // printjng the values of initialising.
	    //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	    cout << "\n " << dec << ni << ") chosen:\n RK^31 =" ;                        
	    outputfile << "\n" << dec << ni << ")\n chosen:\n RK^31 =" ; 
	    for (int i = 3; i > -1; i-- ) {
	    	cout << " ";
	    	outputfile << " ";
	    	for (int j = 3; j > -1; j-- ) {
	    	    cout << hex << ((round_key_31 >> (16 * i + 4 * j)) & 0x0f);
	    	    outputfile << hex << ((round_key_31 >> (16 * i + 4 * j)) & 0x0f);
	    	}
	    }
	    cout << "\n\n CK_31 = " << dec << round_signal_bit_31 << "\n\n"; 
	    outputfile << "\n\n CK_31 = " << dec << round_signal_bit_31 << "\n\n";
	    
	    cout << " created:\n K0 =" ;
	    outputfile << " created:\n K0 =" ;
	    for (int i = 3; i > -1; i-- ) {
	    	cout << " ";
	    	outputfile << " ";
	    	for (int j = 3; j > -1; j-- ) {
	    	    cout << hex << ((K0 >> (16 * i + 4 * j)) & 0x0f);
	    	    outputfile << hex << ((K0 >> (16 * i + 4 * j)) & 0x0f);
	    	}
	    }
		
	    //cout << "\n\n";
	    //outputfile << "\n\n";
	    //
	    //cout << "    K1 =" ;
	    //outputfile << "    K1 =" ;
	    //for (int i = 3; i > -1; i-- ) {
	    //	cout << " ";
	    //	outputfile << " ";
	    //	for (int j = 3; j > -1; j-- ) {
	    //	    cout << hex << ((K1 >> (16 * i + 4 * j)) & 0x0f);
	    //	    outputfile << hex << ((K1 >> (16 * i + 4 * j)) & 0x0f);
	    //	}
	    //}

		// finding the round signal bit (Algorithm 2).
		//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
		int CK_31;

		CK_31 = Algorithm2 (Mix_column_row_matrix, S1_box, S2_box, K0, K1, power_of_2, power_of_element, round_key_31, round_signal_bit_31, outputfile); 
		
		if ( round_signal_bit_31 != CK_31 ) {
			cout << "\n ni = " << ni << " , the chosen CK[31] is different with found CK[31].\n";
			outputfile << "\n ni = " << ni << " , the chosen CK[31] is different with found CK[31].\n";
			exit(1);
		}

		// finding three nibbles of K0 (Algorithm 4).
		//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
		finding_three_nibbles_flag = Algorithm4 (Mix_column_row_matrix, S1_box, S2_box, K0, K1, power_of_2, power_of_element, round_key_31
			                                     , number_of_pairs, finding_three_nibbles_flag, outputfile);

		if ( finding_three_nibbles_flag == 0 ) {
        
		    cout << dec << " ni = " << ni << " the number of rotations = " << j <<"\n"; 
		    outputfile << dec << " ni = " << ni << " the number of rotations = " << j <<"\n";
			
			cout << "\n This program can not find the three nibble of the K0, for this"; 
			cout << " purpose we have to increase the value of m or Rep or both of them. \n";
			outputfile << "\n This program can not find the three nibble of the K0, for this";
			outputfile << " purpose we have to increase the value of m or Rep or both of them. \n";
			int w1 ; cout << "\n\n w1 = "; cin >> w1 ;
		}

		average_of_number_of_pairs = ( (ni - 1) * average_of_number_of_pairs + number_of_pairs ) / ni;
		
		if ( number_of_pairs > max_of_number_of_pairs )
		     max_of_number_of_pairs = number_of_pairs;

	}

	cout << "\n ***the average of number of pairs = " << dec << average_of_number_of_pairs 
		 << "\n the max of number of pairs = " << dec << max_of_number_of_pairs; 
	outputfile << "\n the average of number of pairs = " << dec << average_of_number_of_pairs 
		 << "\n the max of number of pairs = " << dec << max_of_number_of_pairs; 
	
	int w ; cout << "\n\n w = "; cin >> w ;
	return 0 ;
}