/***********************************************************************
 * @file      main.c
 * @version   0.8
 * @brief     Function implementation file.
 *
 * @author    Dave Sluiter, David.Sluiter@Colorado.edu
 * @date      0.1 Dec 23, 2021
 *            0.2 Jan 11, 2022. Added a numerical check for the results of
 *            get_queue_status(). 
 *            0.3 Jan 13, 2022. Added USE_ALL_ENTRIES, implemented both options:
 *                1: Leave 1 entry empty
 *                2: Use all entries
 *            0.4 Sept 9, 2022. Fixed a bug in the scoring code where unsigned int math
 *                was wrapping around. Also added some clamping functions for implementations
 *                that have high error counts, clamping the worst score to 50 out of 100.
 *            0.5 Jan 27, 2023. Created a local definition of the struct so that students have
 *                the freedom to change the name of the anonymous struct definition and
 *                struct element names in their circular_buffer.h file. 
 *            0.6 Aug 22, 2023. Undid the use of a local definition of the queue struct 
 *                queue_struct_t (v0.5), forcing students to use the definition as defined in 
 *                circular_buffer.h.
 *            0.7 Nov 20, 2023. Updated the queue_struct_t to use the exact values we'll
 *                need in Assignment 8. 
 *            0.8 Dec 8, 2023. Updated genRandom2to5() to genRandom1to5(). Previously was
 *                testing buffer fill lengths of 2 to 5, now it's 1 to 5, better test
 *                coverage.
 *
 * @institution University of Colorado Boulder (UCB)
 * @course      ECEN 5823 - IoT Embedded Firmware
 * @instructor  David Sluiter
 *
 * @assignment Assignment 0.5 - Circular Buffer
 * @due        
 *
 * @resources   
 * 
 * @description  Implements the circular buffer testbench.
 *               run as: % ./main | tee main.log 
 *               run as: % ./main <seed> | tee main.log 
 *
 * *************************************************************
 * Students: Use this file as a starting point for your testing. 
 * *************************************************************
 *
 */




#include <stdio.h>  // for printf()
#include <stdint.h> // for uint8_t etc.

#include <stdlib.h> // for rand(), srand()
#include <string.h> // for memcpy()

#include "circular_buffer.h"



#if (USE_ALL_ENTRIES == 1)
#define ACTUAL_QUEUE_DEPTH (QUEUE_DEPTH)
#else
#define ACTUAL_QUEUE_DEPTH (QUEUE_DEPTH-1)
#endif



// target number of entries written to fifo
#define NUMBER_OF_TEST_CASES              (128000)

// constrained random loop count
#define NUMBER_CONSTRAINED_RANDOM_LOOPS   (100000)






   // these were in main(), I made globals for ease/speed, sniff... 

   // test bench data 
   bool                status;
   int                 seed;
   uint32_t            tb_errors = 0;                  // used for data mismatch errors
   uint32_t            tb_meta_errors = 0;             // used for meta errors, queue depth etc
   uint32_t            score;

   queue_struct_t      tb_array[NUMBER_OF_TEST_CASES]; // known good data
   uint32_t            tb_wptr = 0;                    // write pointer
   uint32_t            tb_rptr = 0;                    // read pointer

   uint32_t            status_wptr;
   uint32_t            status_rptr;
   bool                status_full;
   bool                status_empty;
   



// ---------------------------------------------------------------------
// Generator an int between 1 to 5
// ---------------------------------------------------------------------
int genRandom1to5() {

   int theValue = rand() & 0x07; // so range at this point is 0 to 7
   
   if (theValue < MIN_BUFFER_LENGTH) { // 0 
       return (theValue + 1);          // make it 1
   }
   
   if (theValue > MAX_BUFFER_LENGTH) { // 6 or 7
       return (theValue - 2);          // make it 4 or 5
   }
   
   return (theValue); // 1, 2, 3, 4, 5
   
} // genRandom1to5()



// ---------------------------------------------------------------------
// printf 2 buffers of data using length.
// ---------------------------------------------------------------------
void printfBuffers(uint8_t *buf1, uint8_t *buf2, int length) {

    printf ("  buffers:\n  ");
    
    for (int i=0; i<length; i++) {
        printf ("%d ", *buf1);
        buf1++;
    } // for
    printf ("\n");
    
    for (int i=0; i<length; i++) {
        printf ("%d ", *buf2);
        buf2++;
    } // for
    printf ("\n");   
    
} // printfBuffers




// ---------------------------------------------------------------------
// Fill and empty the queue
// return 1 for stop test, 0 for no stop
// ---------------------------------------------------------------------
int fill_and_empty() {

   // for writes
   uint16_t       charHandle;                 // GATT DB handle from gatt_db.h
   uint32_t       bufLength;                  // Number of bytes written to field buffer[5]
   uint8_t        buffer[MAX_BUFFER_LENGTH];  // The actual data buffer for the indication,
  
   // for reads
   uint16_t       charHandle2;                 // GATT DB handle from gatt_db.h
   uint32_t       bufLength2;                  // Number of bytes written to field buffer[5]
   uint8_t        buffer2[MAX_BUFFER_LENGTH];  // The actual data buffer for the indication
   
   // ----------------------------------------------------------------
   // fill the queue
   // ----------------------------------------------------------------
   printf("TB: write %d entries...\n", ACTUAL_QUEUE_DEPTH);
   for (int index=0; index<ACTUAL_QUEUE_DEPTH; index++) {
   
      // Generate the data
      charHandle = (uint16_t) rand();
      bufLength  = (uint32_t) genRandom1to5();
      for (int index2=0; index2<bufLength; index2++) {
          buffer[index2] = (uint8_t) rand();
      }
      
      // write the data to tb array
      tb_array[tb_wptr].charHandle = charHandle;
      tb_array[tb_wptr].bufLength  = bufLength;
      memcpy(&tb_array[tb_wptr].buffer[0], &buffer[0], bufLength);

      // write the data to the DUT 
      status = write_queue (charHandle, bufLength, &buffer[0]);
      if (status != 0) {
         printf("TB Error: write_queue() returned an error. tb_wptr=%d, charHandle=%d, bufLength=%d\n", 
                 tb_wptr, charHandle, bufLength); // not displaying buffer[], for now
         tb_meta_errors++;
      }
      
      // testbench maintenance 
      tb_wptr++;
      if (tb_wptr == NUMBER_OF_TEST_CASES) {
         // stop the run
         printf("Max number of test cases reached (%d), ending run.\n", NUMBER_CONSTRAINED_RANDOM_LOOPS);
         return 1; // stop test
      }
      
   } // for - write
   
   
   // -------------------------------
   // FIFO should be full
   // -------------------------------
   
   // ----------------------------------------------------------------
   // get queue status
   // ----------------------------------------------------------------
   get_queue_status(&status_wptr, &status_rptr, &status_full, &status_empty);
   printf ("get_queue_status(): wptr=%d, rptr=%d, full=%d, empty=%d\n", 
           status_wptr, status_rptr, status_full, status_empty);
           
   if (status_full != true) {
       printf ("TB Error: queue should be full\n");
       tb_meta_errors++;
   }
           
   // ----------------------------------------------------------------
   // validate queue depth
   // ----------------------------------------------------------------
   if (get_queue_depth() != ACTUAL_QUEUE_DEPTH) {
       printf ("TB Error: get_queue_depth() return wrong value, got=%d, expected=%d\n",
               get_queue_depth(), ACTUAL_QUEUE_DEPTH);
       tb_meta_errors++;
   }

   
      
   
   // ----------------------------------------------------------------
   // read back data
   // ----------------------------------------------------------------
   printf("TB: read %d entries...\n", ACTUAL_QUEUE_DEPTH);
   for (int index=0; index<ACTUAL_QUEUE_DEPTH; index++) {
   
      // read the values from the DUT
      status = read_queue (&charHandle2, &bufLength2, &buffer2[0]);
      if (status != 0) {
         printf("TB Error: read_queue() returned an error. charHandle2=%d, bufLength2=%d\n", charHandle2, bufLength2);
         tb_meta_errors++;
      }
      
      // compare against known good data
      if ( (tb_array[tb_rptr].charHandle != charHandle2) || 
           (tb_array[tb_rptr].bufLength != bufLength2) ||
           (memcmp(&tb_array[tb_rptr].buffer[0], &buffer2[0], tb_array[tb_rptr].bufLength) != 0) ) { // memcmp() == 0 -> matches
         printf("TB Error: read_queue() value mismatch. tb_rptr=%d, charHandle expected=%d, charHandle got=%d \
                 bufLength expected=%d, bufLength got=%d\n", 
                 tb_rptr, tb_array[tb_rptr].charHandle, charHandle2, tb_array[tb_rptr].bufLength, bufLength2);
         printfBuffers(&tb_array[tb_rptr].buffer[0], &buffer2[0], tb_array[tb_rptr].bufLength);
         tb_errors++;
      }
      
      // testbench maintenance 
      tb_rptr++;
      if (tb_rptr == NUMBER_OF_TEST_CASES) {
         // stop the run
         printf("Max number of test cases reached (%d), ending run.\n", NUMBER_CONSTRAINED_RANDOM_LOOPS);
         return 1; // stop test 
      }
   
   } // for read
   
   
   // -------------------------------
   // FIFO is empty
   // -------------------------------
   
   // ----------------------------------------------------------------
   // get queue status
   // ----------------------------------------------------------------
   get_queue_status(&status_wptr, &status_rptr, &status_full, &status_empty);
   printf ("get_queue_status(): wptr=%d, rptr=%d, full=%d, empty=%d\n", 
           status_wptr, status_rptr, status_full, status_empty);

   if (status_empty != true) {
       printf ("TB Error: queue should be empty\n");
       tb_meta_errors++;
   }
   
   // ----------------------------------------------------------------
   // validate queue depth
   // ----------------------------------------------------------------
   if (get_queue_depth() != 0) {
       printf ("TB Error: get_queue_depth() return wrong value, got=%d, expected=%d\n",
               get_queue_depth(), 0);
       tb_meta_errors++;
   }

   return 0; 

} // fill_and_empty()






// ---------------------------------------------------------------------
// Fill to 1/2 and empty
// return 1 for stop test, 0 for no stop
// ---------------------------------------------------------------------
int fill_to_half_and_empty() {

   // for writes
   uint16_t       charHandle;                 // GATT DB handle from gatt_db.h
   uint32_t       bufLength;                  // Number of bytes written to field buffer[5]
   uint8_t        buffer[MAX_BUFFER_LENGTH];  // The actual data buffer for the indication,
  
   // for reads
   uint16_t       charHandle2;                 // GATT DB handle from gatt_db.h
   uint32_t       bufLength2;                  // Number of bytes written to field buffer[5]
   uint8_t        buffer2[MAX_BUFFER_LENGTH];  // The actual data buffer for the indication
   

   // ----------------------------------------------------------------
   // fill the queue to 1/2 full
   // ----------------------------------------------------------------
   printf("TB: write %d entries...\n", QUEUE_DEPTH/2);
   for (int index=0; index<QUEUE_DEPTH/2; index++) {
   
      // Generate the data
      charHandle = (uint16_t) rand();
      bufLength  = (uint32_t) genRandom1to5();
      for (int index2=0; index2<bufLength; index2++) {
          buffer[index2] = (uint8_t) rand();
      }
      
      // write the data to tb array
      tb_array[tb_wptr].charHandle = charHandle;
      tb_array[tb_wptr].bufLength  = bufLength;
      memcpy(&tb_array[tb_wptr].buffer[0], &buffer[0], bufLength);
      
      // write the data to the DUT 
      status = write_queue (charHandle, bufLength, &buffer[0]);
      if (status != 0) {
         printf("TB Error: write_queue() returned an error. tb_wptr=%d, charHandle=%d, bufLength=%d\n", 
                 tb_wptr, charHandle, bufLength); // not displaying buffer[], for now
         tb_meta_errors++;
      }
      
      // testbench maintenance 
      tb_wptr++;
      if (tb_wptr == NUMBER_OF_TEST_CASES) {
         // stop the run
         printf("Max number of test cases reached (%d), ending run.\n", NUMBER_CONSTRAINED_RANDOM_LOOPS);
         return 1; // stop test
      }
      
   } // for write 1/2
   
   
   // ---------------------
   // should be 1/2 full
   // ---------------------
   
   // ----------------------------------------------------------------
   // validate queue depth
   // ----------------------------------------------------------------
   if (get_queue_depth() != QUEUE_DEPTH/2) {
       printf ("TB Error: get_queue_depth() return wrong value, got=%d, expected=%d\n",
               get_queue_depth(), QUEUE_DEPTH/2);
       tb_meta_errors++;
   }
   
   
   // ----------------------------------------------------------------
   // read back data the 1/2 full
   // ----------------------------------------------------------------
   printf("TB: read %d entries...\n", QUEUE_DEPTH/2);
   for (int index=0; index<QUEUE_DEPTH/2; index++) {
   
      // read the values from the DUT
      status = read_queue (&charHandle2, &bufLength2, &buffer2[0]);
      if (status != 0) {
         printf("TB Error: read_queue() returned an error. charHandle2=%d, bufLength2=%d\n", charHandle2, bufLength2);
         tb_meta_errors++;
      }
      
      // compare against known good data
      if ( (tb_array[tb_rptr].charHandle != charHandle2) || 
           (tb_array[tb_rptr].bufLength != bufLength2) ||
           (memcmp(&tb_array[tb_rptr].buffer[0], &buffer2[0], tb_array[tb_rptr].bufLength) != 0) ) { // memcmp() == 0 -> matches
         printf("TB Error: read_queue() value mismatch. tb_rptr=%d, charHandle expected=%d, charHandle got=%d \
                 bufLength expected=%d, bufLength got=%d\n", 
                 tb_rptr, tb_array[tb_rptr].charHandle, charHandle2, tb_array[tb_rptr].bufLength, bufLength2);
         printfBuffers(&tb_array[tb_rptr].buffer[0], &buffer2[0], tb_array[tb_rptr].bufLength);
         tb_errors++;
      }
      
      // testbench maintenance 
      tb_rptr++;
      if (tb_rptr == NUMBER_OF_TEST_CASES) {
         // stop the run
         printf("Max number of test cases reached (%d), ending run.\n", NUMBER_CONSTRAINED_RANDOM_LOOPS);
         return 1; // stop test
      }
   
   } // for read 1/2
   
   
   
   // - - - - - - - - - - - - - - - - - - - - -
   // so the wptr/rptr values are 1/2 way,
   // should be empty
   // - - - - - - - - - - - - - - - - - - - - -
   
   // ----------------------------------------------------------------
   // get queue status
   // ----------------------------------------------------------------
   get_queue_status(&status_wptr, &status_rptr, &status_full, &status_empty);
   printf ("get_queue_status(): wptr=%d, rptr=%d, full=%d, empty=%d\n", 
           status_wptr, status_rptr, status_full, status_empty);
   
   if (status_empty != true) {
       printf ("TB Error: queue should be empty\n");
       tb_meta_errors++;
   }
   
   // ----------------------------------------------------------------
   // validate queue depth
   // ----------------------------------------------------------------
   if (get_queue_depth() != 0) {
       printf ("TB Error: get_queue_depth() return wrong value, got=%d, expected=%d\n",
               get_queue_depth(), 0);
       tb_meta_errors++;
   }
   
   
   
   // -------------------------------------------
   // test the results from get_queue_status()
   // Added on 01-11-2022
   // -------------------------------------------
   
   // wptr should be == (ACTUAL_QUEUE_DEPTH/2)
   if (status_wptr != (ACTUAL_QUEUE_DEPTH/2)) {
       printf("TB Error: get_queue_status() wptr value mismatch. expected wptr=%d, got wptr=%d\n", 
                 (ACTUAL_QUEUE_DEPTH/2), status_wptr);
       tb_meta_errors++;
   }
   // rptr should be == (ACTUAL_QUEUE_DEPTH/2)
   if (status_rptr != (ACTUAL_QUEUE_DEPTH/2)) {
       printf("TB Error: get_queue_status() rptr value mismatch. expected rptr=%d, got rptr=%d\n", 
                 (ACTUAL_QUEUE_DEPTH/2), status_rptr);
       tb_meta_errors++;
   }
   // full should be == false
   if (status_full != false) {
       printf("TB Error: get_queue_status() full value mismatch. expected full=%d, got full=%d\n", 
                 false, status_full);
       tb_meta_errors++;
   }
   // empty should be == true
   if (status_empty != true) {
       printf("TB Error: get_queue_status() empty value mismatch. expected empty=%d, got empty=%d\n", 
                 true, status_empty);
       tb_meta_errors++;
   }
   

 return 0;

} // fill_to_half_and_empty()







// ---------------------------------------------------------------------
// Public main() function
// ---------------------------------------------------------------------

int main(int argc, char *argv[]) {

   int            flag; // status code returned by functions
   
   // for writes
   uint16_t       charHandle;                 // GATT DB handle from gatt_db.h
   uint32_t       bufLength;                  // Number of bytes written to field buffer[5]
   uint8_t        buffer[MAX_BUFFER_LENGTH];  // The actual data buffer for the indication,
  
   // for reads
   uint16_t       charHandle2;                 // GATT DB handle from gatt_db.h
   uint32_t       bufLength2;                  // Number of bytes written to field buffer[5]
   uint8_t        buffer2[MAX_BUFFER_LENGTH];  // The actual data buffer for the indication
   
   
   
   if (argc == 2) { // assume argv[1] is seed value
       sscanf (argv[1], "%d", &seed);
       srand (seed); // init the random seed, rand() returns an integer type between 0 and RAND_MAX
   } else {
	   srand (100); // default seed 
   }
   printf ("seed=%d\n", seed);



   



   // ------------------------------------------------------------------
   // Example usage:
   
//    // --------------------------
//    // Initialization
//    // --------------------------
//    reset_queue();
//    
//    // write 1 queue entry
//    charHandle = 5;
//    bufLength  = 2;
//    buffer[0]  = 6;
//    buffer[1]  = 7;
//    write_queue (charHandle, bufLength, &buffer[0]); // write an entry to the queue
// 
//    // read 1 queue entry
//    charHandle2 = bufLength2 = 0;
//    read_queue (&charHandle2, &bufLength2, &buffer2[0]); // read an entry from the queue
//     
//    // print the read-back results
//    printf("charHandle2=%d, bufLength2=%d\n", charHandle2, bufLength2);
//    printf("buffer data:\n  ");
//    for (int i=0; i<bufLength2; i++) {
//       printf("%d ", buffer2[i]);
//    }
//    printf("\n");
//    
//    // compare the written values against the read-back values
//    if (charHandle != charHandle2) {
//       printf("TB Error: charHandle=%d, charHandle2=%d\n", charHandle, charHandle2);
//    }
//    if (bufLength != bufLength2) {
//       printf("TB Error: bufLength=%d, bufLength2=%d\n", bufLength, bufLength2);
//    }
//    for (int i=0; i<bufLength; i++) { // use known, correct bufLength for compare
//       if (buffer[i] != buffer2[i]) {
//          printf("TB Error: buffer[%d]=%d, buffer2[%d]=%d\n", i, buffer[i], i, buffer2[i]);
//       }
//    } // for
   
   // End example usage
   // ------------------------------------------------------------------


     
   // --------------------------
   // Initialization
   // --------------------------
   reset_queue(); 
   
   // ----------------------------------------------------------------
   // Run a few "smoke tests" to validate that basic functionality is
   // working.
   // ----------------------------------------------------------------
   flag = fill_and_empty();
   if (flag) goto tests_done;
     
   flag = fill_to_half_and_empty();
   if (flag) goto tests_done;

   flag = fill_and_empty();
   if (flag) goto tests_done;
   


   // ----------------------------------------------------------------
   // This is the constrained random part of the test. Hammer away at
   // the queue, testing only for correct values read back.
   // ----------------------------------------------------------------
  
   // Students: In the real autograder we will use, this is the part of the test that runs
   //           the constrained random portion of the test.
   // 
   // Action: Create more tests here to thoroughly test your design.
   //
   
   
   
   
   
   
   
   
   
   
   
   
   
 

   
   
tests_done:
   printf ("\nTest Done.\n");
   
   // if the number of tb_errors > tb_wptr, clamp tb_errors to tb_wptr + meta errors
   if ((tb_errors + tb_meta_errors) > tb_wptr) {
      tb_errors = tb_wptr;
      printf ("Warning: High error count. Clamping error count to number of test cases\n");
   }
                     
   // score should be a number from 0 to 100 
   if (tb_wptr != 0) {
     score = ((tb_wptr - tb_errors)*100/tb_wptr); // % of read errors and meta errors
   } else {
     score = 0;
   }

   // clamp score to 50 if it is < 50, 
   // this is partial credit for a poor implementation 
   printf("Pre-clamped score = %d \n", score);
   if (score < 50) {
     score = 50; 
     printf ("Warning: Low score. Clamping score to 50\n");
   }
   printf ("Score = %d\n\n", score);
   

   
   return (0);
   
} // main() 


