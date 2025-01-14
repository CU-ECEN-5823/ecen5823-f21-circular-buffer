/***********************************************************************
 * @file      circular_buffer.c
 * @version   0.3
 * @brief     Function implementation file.
 *
 * @author    Awesome Student, Awesome.Student@Colorado.edu
 *
 * @institution University of Colorado Boulder (UCB)
 * @course      ECEN 5823 - IoT Embedded Firmware
 * @instructor  Chris Choi 
 * @source      Adopted from David Sluiter
 *
 * @date      0.1 Dec 22, 2021
 *            0.2 Sept 2, 2022. Changed assignment to 0.5.
 *            0.3 Nov 19, 2023. Updated the queue element struct to match what's needed 
 *                              for A8.
 *
 * @assignment Assignment 0.5 - Circular Buffer
 * @due        
 *
 * @resources   
 * 
 *   Revision history:
 *   0.1 Initial release. Example queue entry struct contains uint8_t a, and uint16_t b.
 *   0.2 Added USE_ALL_ENTRIES, implemented both options:
 *       a: Leave 1 entry empty
 *       b: Use all entries
 *   0.3 Updated the queue entry struct definition to match what's needed for A8.
 *
 */

// ----------------------------------------------
// Implementation file.
// Declare private variables here.
// Declare global variables here.
// Create the implementation for all private functions.
// Create the implementation for all public functions.
// ----------------------------------------------

#include <stdio.h>  // for printf()
#include <string.h> // for memcpy()

#include "circular_buffer.h"


// Declare memory for the queue/buffer, and our write and read pointers.
queue_struct_t   my_queue[QUEUE_DEPTH]; // the queue
uint32_t         wptr = 0;              // write pointer
uint32_t         rptr = 0;              // read pointer

// Student edit:
// Decide how will you handle the full condition.




// ---------------------------------------------------------------------
// Private function used only by this .c file. 
// Compute the next ptr value. Given a valid ptr value, compute the next valid
// value of the ptr and return it.
// Isolation of functionality: This defines "how" a pointer advances. 
// ---------------------------------------------------------------------
static uint32_t nextPtr(uint32_t ptr) {

  // Student edit:
  // Create this function
  


} // nextPtr()





// ---------------------------------------------------------------------
// Public function. 
// This function resets the queue.
// ---------------------------------------------------------------------
void reset_queue (void) {

  // Student edit:
  // Create this function
  



} // reset_queue()




// ---------------------------------------------------------------------
// Public function.
// This function writes an entry to the queue if the the queue is not full.
// Input parameter "charHandle" should be written to queue_struct_t element "charHandle".
// Input parameter "bufLength" should be written to queue_struct_t element "bufLength"
// The bytes pointed at by input parameter "buffer" should be written to queue_struct_t element "buffer"
// Returns bool false if successful or true if writing to a full fifo.
// i.e. false means no error, true means an error occurred. 
// ---------------------------------------------------------------------
bool write_queue (uint16_t charHandle, uint32_t bufLength, uint8_t *buffer) {

  // Student edit:
  // Create this function
  // Decide how you want to handle the "full" condition.

  // Don't forget to range check bufLength.
  // Isolation of functionality: 
  //     Create the logic for "when" a pointer advances.









} // write_queue() 





// ---------------------------------------------------------------------
// Public function.
// This function reads an entry from the queue, and returns values to the
// caller. The values from the queue entry are returned by writing 
// the values to variables declared by the caller, where the caller is passing
// in pointers to charHandle, bufLength and buffer. The caller's code will look like this:
//
//   uint16_t     charHandle;
//   uint32_t     bufLength;
//   uint8_t      buffer[5];
//
//   status = read_queue (&charHandle, &bufLength, &buffer[0]);
//
// *** If the code above doesn't make sense to you, you probably lack the 
// necessary prerequisite knowledge to be successful in this course.
//   
// Write the values of charHandle, bufLength, and buffer from my_queue[rptr] to 
// the memory addresses pointed at by charHandle, bufLength and buffer, like this :
//      *charHandle = <something>;
//      *bufLength  = <something_else>;
//      *buffer     = <something_else_again>; // perhaps memcpy() would be useful?
//
// In this implementation, we do it this way because
// standard C does not provide a mechanism for a C function to return multiple
// values, as is common in perl or python.
// Returns bool false if successful or true if reading from an empty fifo. 
// i.e. false means no error, true means an error occurred. 
// ---------------------------------------------------------------------
bool read_queue (uint16_t *charHandle, uint32_t *bufLength, uint8_t *buffer) {

  // Student edit:
  // Create this function
  
  // Isolation of functionality: 
  //     Create the logic for "when" a pointer advances
  
  
  

  
} // read_queue() 






// ---------------------------------------------------------------------
// Public function.
// This function returns the wptr, rptr, full and empty values, writing
// to memory using the pointer values passed in, same rationale as read_queue()
// The "_" characters are used to disambiguate the global variable names from
// the input parameter names, such that there is no room for the compiler to make a
// mistake in interpreting your intentions.
// ---------------------------------------------------------------------
void get_queue_status (uint32_t *_wptr, uint32_t *_rptr, bool *_full, bool *_empty) {

  // Student edit:
  // Create this function
  





} // get_queue_status() 



// ---------------------------------------------------------------------
// Public function.
// Function that computes the number of written entries currently in the queue. If there
// are 3 entries in the queue, it should return 3. If the queue is empty it should 
// return 0. If the queue is full it should return either QUEUE_DEPTH if
// USE_ALL_ENTRIES==1 otherwise returns QUEUE_DEPTH-1. 
// ---------------------------------------------------------------------
uint32_t get_queue_depth() {

  // Student edit:
  // Create this function
  




} // get_queue_depth() 








