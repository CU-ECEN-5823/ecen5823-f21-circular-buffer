/***********************************************************************
 * @file      example_main.c
 * @version   0.1
 * @brief     Main function to test circular buffer implementation.
 *            Provided to students as a starting point for their testing.
 *
 *   
 * @description  Implements the circular buffer testbench.
 *               run as: % ./main | tee main.log 
 *               run as: % ./main <seed> | tee main.log 
 */




#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include <stdlib.h> // for rand(), srand()

#include "circular_buffer.h"

#if (USE_ALL_ENTRIES == 1)
#define ACTUAL_QUEUE_DEPTH (QUEUE_DEPTH)
#else
#define ACTUAL_QUEUE_DEPTH (QUEUE_DEPTH-1)
#endif






// these were in main(), I made globals for ease/speed, sniff... 

   uint32_t     index, index2;
   
   bool         status;
   int          seed;
   
   uint8_t      a;  
   uint16_t     b;    

   // test bench data 
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
// Fill and empty the queue
// return 1 for stop test, 0 for no stop
// ---------------------------------------------------------------------
int fill_and_empty() {


   // ----------------------------------------------------------------
   // fill the queue
   // ----------------------------------------------------------------
   printf("TB: write %d entries...\n", ACTUAL_QUEUE_DEPTH);
   for (index=0; index<ACTUAL_QUEUE_DEPTH; index++) {
   
      a = (uint8_t)  rand();
      b = (uint16_t) rand();
      
      // write the data to tb array
      tb_array[tb_wptr].a = a;
      tb_array[tb_wptr].b = b;
      
      // write the data to the DUT 
      status = write_queue (a, b);
      if (status != 0) {
         printf("TB Error: write_queue() returned an error. tb_wptr=%d, a=%d, b=%d\n", 
                 tb_wptr, a, b);
         tb_meta_errors++;
      }
      
      // testbench maintenance 
      tb_wptr++;
      if (tb_wptr == NUMBER_OF_TEST_CASES) {
         // stop the run
         printf("Max number of test cases reached (%d), ending run.\n", NUMBER_CONSTRAINED_RANDOM_LOOPS);
         return 1; // stop test
      }
      
   } // for write
   
   
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
   for (index=0; index<ACTUAL_QUEUE_DEPTH; index++) {
   
      // read the values from the DUT 
      status = read_queue (&a, &b);
      if (status != 0) {
         printf("TB Error: read_queue() returned an error. a=%d, b=%d\n", a, b);
         tb_meta_errors++;
      }
      
      // compare against known good data
      if ((tb_array[tb_rptr].a != a) || (tb_array[tb_rptr].b != b)) {
         printf("TB Error: read_queue() value mismatch. tb_rptr=%d, a expected=%d, a got=%d \
                 b expected=%d, b got=%d\n", 
                 tb_rptr, tb_array[tb_rptr].a, a, tb_array[tb_rptr].b, b);
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


   // ----------------------------------------------------------------
   // fill the queue to 1/2 full
   // ----------------------------------------------------------------
   printf("TB: write %d entries...\n", QUEUE_DEPTH/2);
   for (index=0; index<QUEUE_DEPTH/2; index++) {
   
      a = (uint8_t)  rand();
      b = (uint16_t) rand();
      
      // write the data to tb array
      tb_array[tb_wptr].a = a;
      tb_array[tb_wptr].b = b;
      
      // write the data to the DUT 
      status = write_queue (a, b);
      if (status != 0) {
         printf("TB Error: write_queue() returned an error. tb_wptr=%d, a=%d, b=%d\n", 
                 tb_wptr, a, b);
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
   for (index=0; index<QUEUE_DEPTH/2; index++) {
   
      // read the values from the DUT 
      status = read_queue (&a, &b);
      if (status != 0) {
         printf("TB Error: read_queue() returned an error. a=%d, b=%d\n", a, b);
         tb_meta_errors++;
      }
      
      // compare against known good data
      if ((tb_array[tb_rptr].a != a) || (tb_array[tb_rptr].b != b)) {
         printf("TB Error: read_queue() value mismatch. tb_rptr=%d, a expected=%d, a got=%d \
                 b expected=%d, b got=%d\n", 
                 tb_rptr, tb_array[tb_rptr].a, a, tb_array[tb_rptr].b, b);
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
   // so the wptr/rptr values are 1/2 way
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


   int            flag;
   

   if (argc == 2) { // assume argv[1] is seed value
       sscanf (argv[1], "%d", &seed);
       srand (seed); // init the random seed, rand() returns an integer type between 0 and RAND_MAX
   } else {
	   srand (100); // default seed 
   }
   printf ("seed=%d\n", seed);

/*
   // Example usage:
   
   int a = 5;
   int b = 10;
   int a2;
   int b2;

   write_queue (a, b); // write an entry to the queue

   a2 = b2 = 0;
   read_queue (&a2, &b2); // read an entry from the queue
   printf("a=%d, b=%d\n", a2, b2); 
   
   // compare the written values against the read-back values
   
*/


      
   
  
   flag = fill_and_empty();
   if (flag) goto tests_done;
   
   flag = fill_to_half_and_empty();
   if (flag) goto tests_done;

   flag = fill_and_empty();
   if (flag) goto tests_done;
   
   
   
   

   
   // ----------------------------------------------------------------
   // The auto-grading testbench performs constrained random testing here
   // ----------------------------------------------------------------
   
  
   
   
   
tests_done:
   //printf ("\n%d test cases completed. %d errors reported.\n", NUMBER_OF_TEST_CASES, tb_errors);
   //printf ("Score = %d\n", (NUMBER_OF_TEST_CASES - tb_errors)*100/NUMBER_OF_TEST_CASES);
   
   // if the number of tb_errors > tb_wptr, clamp tb_errors to tb_wptr + meta errors
   if ((tb_errors + tb_meta_errors) > tb_wptr) {
      tb_errors = tb_wptr;
      printf ("Warning: High error count. Clamping error count to number of test cases\n");
   }
            
//   printf ("\n%d test cases completed. %d data mismatch errors reported. %d meta errors reported.\n", 
//            tb_wptr, tb_errors, tb_meta_errors);           
            
   // score should be a number from 0 to 100 
   //score = ((tb_wptr - (tb_errors + tb_meta_errors))*100/tb_wptr); // % of read errors and meta errors
   score = ((tb_wptr - tb_errors)*100/tb_wptr); // % of read errors and meta errors

   // clamp score to 50 if it is < 50, 
   // this is partial credit for a poor implementation 
   printf("Pre-clamped score = %d \n", score);
   if (score < 50) {
     score = 50; 
     printf ("Warning: Low score. Clamping score to 50\n");
   }
   printf ("\nScore = %d\n\n", score);
   
   return (0);
   
} // main() 


