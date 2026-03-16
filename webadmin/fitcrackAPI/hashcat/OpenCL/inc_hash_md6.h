/**
 * Author......: See docs/credits.txt
 * License.....: MIT
 */

#ifndef INC_HASH_MD6_H
#define INC_HASH_MD6_H

/* for standard word size */

#define RL00 loop_body(10,11, 0)
#define RL01 loop_body( 5,24, 1)
#define RL02 loop_body(13, 9, 2)
#define RL03 loop_body(10,16, 3)
#define RL04 loop_body(11,15, 4)
#define RL05 loop_body(12, 9, 5)
#define RL06 loop_body( 2,27, 6)
#define RL07 loop_body( 7,15, 7)
#define RL08 loop_body(14, 6, 8)
#define RL09 loop_body(15, 2, 9)
#define RL10 loop_body( 7,29,10)
#define RL11 loop_body(13, 8,11)
#define RL12 loop_body(11,15,12)
#define RL13 loop_body( 7, 5,13)
#define RL14 loop_body( 6,31,14)
#define RL15 loop_body(12, 9,15)

/* n == 89 */

#define  t0   17     /* index for linear feedback */
#define  t1   18     /* index for first input to first and */
#define  t2   21     /* index for second input to first and */
#define  t3   31     /* index for first input to second and */
#define  t4   67     /* index for second input to second and */
#define  t5   89     /* last tap */

#define md6_w 64
#define md6_n 89            /* size of compression input block, in words  */
#define md6_c 16            /* size of compression output, in words       */
#define md6_q 15            /* # Q words in compression block (>=0)       */
#define md6_k  8            /* # key words per compression block (>=0)    */
#define md6_b 64            /* # data words per compression block (>0)    */

#define md6_default_L   64  /* large so that MD6 is fully hierarchical */

#define MD6_256_ROUNDS 104  // default value
#define MD6_256_DLEN   256  // digest len

#define loop_body(rs,ls,step)                                       \
  x  = S;                               /* feedback constant     */ \
  x ^= A[i+step-t5];                    /* end-around feedback   */ \
  x ^= A[i+step-t0];                    /* linear feedback       */ \
  x ^= ( A[i+step-t1] & A[i+step-t2] ); /* first quadratic term  */ \
  x ^= ( A[i+step-t3] & A[i+step-t4] ); /* second quadratic term */ \
  x ^= (x >> rs);                       /* right-shift           */ \
  A[i+step] = x ^ (x << ls);            /* left-shift            */

#endif // INC_HASH_MD6_H
