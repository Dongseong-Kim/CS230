/* 
 * CS:APP Data Lab 
 * 
 * <Please put your name and userid here>
 * name / userid : KIM DONGSEONG / cs20150112
 * 
 * bits.c - Source file with your solutions to the Lab.
 *          This is the file you will hand in to your instructor.
 *
 * WARNING: Do not include the <stdio.h> header; it confuses the dlc
 * compiler. You can still use printf for debugging without including
 * <stdio.h>, although you might get a compiler warning. In general,
 * it's not good practice to ignore compiler warnings, but in this
 * case it's OK.  
 */

#if 0
/*
 * Instructions to Students:
 *
 * STEP 1: Read the following instructions carefully.
 */

You will provide your solution to the Data Lab by
editing the collection of functions in this source file.

INTEGER CODING RULES:
 
  Replace the "return" statement in each function with one
  or more lines of C code that implements the function. Your code 
  must conform to the following style:
 
  int Funct(arg1, arg2, ...) {
      /* brief description of how your implementation works */
      int var1 = Expr1;
      ...
      int varM = ExprM;

      varJ = ExprJ;
      ...
      varN = ExprN;
      return ExprR;
  }

  Each "Expr" is an expression using ONLY the following:
  1. Integer constants 0 through 255 (0xFF), inclusive. You are
      not allowed to use big constants such as 0xffffffff.
  2. Function arguments and local variables (no global variables).
  3. Unary integer operations ! ~
  4. Binary integer operations & ^ | + << >>
    
  Some of the problems restrict the set of allowed operators even further.
  Each "Expr" may consist of multiple operators. You are not restricted to
  one operator per line.

  You are expressly forbidden to:
  1. Use any control constructs such as if, do, while, for, switch, etc.
  2. Define or use any macros.
  3. Define any additional functions in this file.
  4. Call any functions.
  5. Use any other operations, such as &&, ||, -, or ?:
  6. Use any form of casting.
  7. Use any data type other than int.  This implies that you
     cannot use arrays, structs, or unions.

 
  You may assume that your machine:
  1. Uses 2s complement, 32-bit representations of integers.
  2. Performs right shifts arithmetically.
  3. Has unpredictable behavior when shifting an integer by more
     than the word size.

EXAMPLES OF ACCEPTABLE CODING STYLE:
  /*
   * pow2plus1 - returns 2^x + 1, where 0 <= x <= 31
   */
  int pow2plus1(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     return (1 << x) + 1;
  }

  /*
   * pow2plus4 - returns 2^x + 4, where 0 <= x <= 31
   */
  int pow2plus4(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     int result = (1 << x);
     result += 4;
     return result;
  }

FLOATING POINT CODING RULES

For the problems that require you to implent floating-point operations,
the coding rules are less strict.  You are allowed to use looping and
conditional control.  You are allowed to use both ints and unsigneds.
You can use arbitrary integer and unsigned constants.

You are expressly forbidden to:
  1. Define or use any macros.
  2. Define any additional functions in this file.
  3. Call any functions.
  4. Use any form of casting.
  5. Use any data type other than int or unsigned.  This means that you
     cannot use arrays, structs, or unions.
  6. Use any floating point data types, operations, or constants.


NOTES:
  1. Use the dlc (data lab checker) compiler (described in the handout) to 
     check the legality of your solutions.
  2. Each function has a maximum number of operators (! ~ & ^ | + << >>)
     that you are allowed to use for your implementation of the function. 
     The max operator count is checked by dlc. Note that '=' is not 
     counted; you may use as many of these as you want without penalty.
  3. Use the btest test harness to check your functions for correctness.
  4. Use the BDD checker to formally verify your functions
  5. The maximum number of ops for each function is given in the
     header comment for each function. If there are any inconsistencies 
     between the maximum ops in the writeup and in this file, consider
     this file the authoritative source.

/*
 * STEP 2: Modify the following functions according the coding rules.
 * 
 *   IMPORTANT. TO AVOID GRADING SURPRISES:
 *   1. Use the dlc compiler to check that your solutions conform
 *      to the coding rules.
 *   2. Use the BDD checker to formally verify that your solutions produce 
 *      the correct answers.
 */


#endif
/* Copyright (C) 1991-2014 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <http://www.gnu.org/licenses/>.  */
/* This header is separate from features.h so that the compiler can
   include it implicitly at the start of every compilation.  It must
   not itself include <features.h> or any other header that includes
   <features.h> because the implicit include comes before any feature
   test macros that may be defined in a source file before it first
   explicitly includes a system header.  GCC knows the name of this
   header in order to preinclude it.  */
/* glibc's intent is to support the IEC 559 math functionality, real
   and complex.  If the GCC (4.9 and later) predefined macros
   specifying compiler intent are available, use them to determine
   whether the overall intent is to support these features; otherwise,
   presume an older compiler has intent to support these features and
   define these macros by default.  */
/* wchar_t uses ISO/IEC 10646 (2nd ed., published 2011-03-15) /
   Unicode 6.0.  */
/* We do not support C11 <threads.h>.  */
/* 
 * bitAnd - x&y using only ~ and | 
 *   Example: bitAnd(6, 5) = 4
 *   Legal ops: ~ |
 *   Max ops: 8
 *   Rating: 1
 */
int bitAnd(int x, int y) {
  return ~(~x | ~y);
}
/* 
 * getByte - Extract byte n from word x
 *   Bytes numbered from 0 (LSB) to 3 (MSB)
 *   Examples: getByte(0x12345678,1) = 0x56
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 6
 *   Rating: 2
 */
int getByte(int x, int n) {
  int shift = n<<3;
  return (x>>shift)&0xFF;
}
/* 
 * logicalShift - shift x to the right by n, using a logical shift
 *   Can assume that 0 <= n <= 31
 *   Examples: logicalShift(0x87654321,4) = 0x08765432
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 20
 *   Rating: 3 
 */
int logicalShift(int x, int n) {
  int allone = ~0;
  int head = ((0x8<<28)>>n)<<1;
  int headzero = allone ^ head;
  
  return x>>n & headzero;
}
/*
 * bitCount - returns count of number of 1's in word
 *   Examples: bitCount(5) = 2, bitCount(7) = 3
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 40
 *   Rating: 4
 */
int bitCount(int x) {
  int num = 0x1 + (0x1<<8) + (0x1<<16) + (0x1<<24);
  int binaryBitNumber = (x&num) + ((x>>1)&num) + ((x>>2)&num) + ((x>>3)&num) + ((x>>4)&num) + ((x>>5)&num) + ((x>>6)&num) + ((x>>7)&num);
  
  int checker = 0xFF;
  int result = (binaryBitNumber&checker) + ((binaryBitNumber>>8)&checker) + ((binaryBitNumber>>16)&checker) + ((binaryBitNumber>>24)&checker);

  return result;
}
/* 
 * bang - Compute !x without using !
 *   Examples: bang(3) = 0, bang(0) = 1
 *   Legal ops: ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 4 
 */
int bang(int x) {
  int nx = ~x + 0x1;
  int head = 0x8 << 28;
  return (((nx | x) & head)>>31) + 0x1;
}
/* 
 * tmin - return minimum two's complement integer 
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 4
 *   Rating: 1
 */
int tmin(void) {
  return 0x1 << 31;
}
/* 
 * fitsBits - return 1 if x can be represented as an 
 *  n-bit, two's complement integer.
 *   1 <= n <= 32
 *   Examples: fitsBits(5,3) = 0, fitsBits(-4,3) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 15
 *   Rating: 2
 */
int fitsBits(int x, int n) {
  int shift = 33 + ~n;

  int fitsCheck = (x << shift) >> shift;

  return !(x ^ fitsCheck);
}
/* 
 * divpwr2 - Compute x/(2^n), for 0 <= n <= 30
 *  Round toward zero
 *   Examples: divpwr2(15,1) = 7, divpwr2(-33,4) = -2
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 15
 *   Rating: 2
 */
int divpwr2(int x, int n) {
	int head = 0x80 << 24;
	int signChecker = (head & x)>>31;
	int RRemover = (head >> 31) << n;
	
	int result = ((x & RRemover)>>n) + !!(signChecker&(~RRemover&x));
	
	return result;
}
/* 
 * negate - return -x 
 *   Example: negate(1) = -1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 5
 *   Rating: 2
 */
int negate(int x) {
  int result = ~x + 1;

  return result;
}
/* 
 * isPositive - return 1 if x > 0, return 0 otherwise 
 *   Example: isPositive(-1) = 0.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 8
 *   Rating: 3
 */
int isPositive(int x) {
  int signChecker = x >> 31;
  int NtoZ = !!(x + (((x ^ signChecker) + 1) & signChecker));

  return NtoZ;
}
/* 
 * isLessOrEqual - if x <= y  then return 1, else return 0 
 *   Example: isLessOrEqual(4,5) = 1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 24
 *   Rating: 3
 */
int isLessOrEqual(int x, int y) {
  int signX = x >> 31;
  int signY = y >> 31;
  int isSignDiff = signX ^ signY;

  int signRemover = ~(0x1 << 31);
  int revisedX = x & signRemover;
  int revisedY = y & signRemover;

  int result = !!(isSignDiff & signX) + (~isSignDiff & !( (revisedY + (~revisedX + 1)) >> 31) );

  return result;
}
/*
 * ilog2 - return floor(log base 2 of x), where x > 0
 *   Example: ilog2(16) = 4
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 90
 *   Rating: 4
 */
int ilog2(int x) {
  int byteCheck = 0xFF;
	
  int byteNum0 = !!(x & byteCheck);
  int byteNum1 = (!!(x & (byteCheck << 8))) << 1;
  int byteNum2 = (!!(x & (byteCheck << 16))) << 2;
  int byteNum3 = (!!(x & (byteCheck << 24))) << 3;

  int byteNumResult = byteNum0 + byteNum1 + byteNum2 + byteNum3;

  int i1 = !(byteNumResult >> 1);
  int i2 = !(byteNumResult >> 2);
  int i3 = !(byteNumResult >> 3);

  int whichByte = 4 + (~(i1 + i2 + i3));

  int toNum0Byte = x >> (whichByte << 3);

  int ii2 = !(toNum0Byte>>1);
  int ii3 = !(toNum0Byte >> 2);
  int ii4 = !(toNum0Byte >> 3);
  int ii5 = !(toNum0Byte >> 4);
  int ii6 = !(toNum0Byte >> 5);
  int ii7 = !(toNum0Byte >> 6);
  int ii8 = !(toNum0Byte >> 7);

  int floorInByte = 8 + ~(ii2 + ii3 + ii4 + ii5 + ii6 + ii7 + ii8);

  int floor = (whichByte << 3) + floorInByte;

  return floor;
}
/* 
 * float_neg - Return bit-level equivalent of expression -f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representations of
 *   single-precision floating point values.
 *   When argument is NaN, return argument.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 10
 *   Rating: 2
 */
unsigned float_neg(unsigned uf) {
  if ((uf & 0x7FFFFFFF) <=  0x7F800000)
    return uf ^ 0x80000000;
  else
    return uf;
}
/* 
 * float_i2f - Return bit-level equivalent of expression (float) x
 *   Result is returned as unsigned int, but
 *   it is to be interpreted as the bit-level representation of a
 *   single-precision floating point values.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
unsigned float_i2f(int x) {
	int sign = x & 0x80000000;
	int positiveX, head = 0x80000000;
	int E = 31, exp = 0, M = 0;
	int Mcut = ~0x80000000;
	int rawM, tempM;
	int check;

	if (x < 0)
		positiveX = -x;
	else if (x == 0)
		return 0;
	else
		positiveX = x;

	while (!(positiveX & head)) {
		if (E == 0) {
			break;
		}
		head = head >> 1;
		E--;
	}
	
	exp = E + 127;
	rawM = (Mcut >> (31 - E)) & positiveX;
	if (E <= 23) {
		M = rawM << (23 - E);
	}
	else {
		tempM = rawM >> (E - 24);
		M = tempM >> 1;
		check = tempM & 0x1;
		if (M & 0x1) {
			if (check)
				M += 1;
		}
		else {
			if (check) {
				while (tempM != rawM) {
					if (rawM & 0x1) {
						M += 1;
						break;
					}
					rawM = rawM >> 1;
				}
			}
		}

	}

	return sign + (exp << 23) + M;
}
/* 
 * float_twice - Return bit-level equivalent of expression 2*f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representation of
 *   single-precision floating point values.
 *   When argument is NaN, return argument
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
unsigned float_twice(unsigned uf) {
	int exceptSign = uf & 0x7FFFFFFF;

	if (exceptSign <= 0x7F800000) {
		if (uf == 0x0)
			return uf;
		else if (uf == 0x80000000)
			return uf;
		else {
			int sign = uf & 0x80000000;
			int exp = uf & 0x7f800000;

			if (exp == 0) {
				return sign + (uf << 1);
			}
			else if (exp == 0x7F800000)
				return uf;
			else
				return uf + 0x00800000;
		}

	}
	else
		return uf;
}
