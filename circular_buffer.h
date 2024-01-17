/***********************************************************************
 * @file      circular_buffer.h
 * @version   0.3
 * @brief     Function header (interface) file.
 *
 * @author    Awesome Student, Awesome.Student@Colorado.edu
 *
 * @institution University of Colorado Boulder (UCB)
 * @course      ECEN 5823 - IoT Embedded Firmware
 * @instructor  David Sluiter
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
 */

// ----------------------------------------------
// Header file.
// Include header files that a caller needs to know.
// Define everything that a caller needs to know.
// ----------------------------------------------


#ifndef __MY_CIRCULAR_BUFFER__
#define __MY_CIRCULAR_BUFFER__


// These are included here so that data types used below, and therefore are
// needed by the caller, are defined and available.
#include <stdint.h>    // for uint8_t etc.
#include <stdbool.h>   // for bool



// This is the number of entries in the queue. Please leave
// this value set to 16.
#define QUEUE_DEPTH      (16)
// Student edit: 
//   define this to 1 if your design uses all array entries
//   define this to 0 if your design leaves 1 array entry empty
#define USE_ALL_ENTRIES  (1)




// Modern C (circa 2021 does it this way)
// This is referred to as an anonymous struct definition.
//
// This is the structure of 1 queue/buffer/FIFO entry.
// When you write an entry to the queue, you write 3 
// values to the queue entry, 1 value for each field (element) of the struct.
// The rationale for declaring 1 queue entry in this 
// manner will become clear later in Assignment 8. 
//
// *** Please do not change this definition! 
// *** The autograder (i.e. the testbench) uses this struct definition!
// *** Changing this struct definiton will cause the autograder to fail. 

// typedef struct {
//   uint8_t       a; // An 8-bit user value in 1 queue entry
//   uint16_t      b; // A 16-bit user value in 1 queue entry
// } queue_struct_t;

#define MAX_BUFFER_LENGTH  (5)
#define MIN_BUFFER_LENGTH  (1)

//typedef uint8_t fixedArray[MAX_BUFFER_LENGTH];

typedef struct {

  uint16_t       charHandle;                 // GATT DB handle from gatt_db.h
  uint32_t       bufLength;                  // Number of bytes written to field buffer[5]
  uint8_t        buffer[MAX_BUFFER_LENGTH];  // The actual data buffer for the indication,
                                             //   need 5-bytes for HTM and 1-byte for button_state.
                                             //   For testing, test lengths 1 through 5,
                                             //   a length of 0 shall be considered an
                                             //   error, as well as as lengths > 5
                                             
} queue_struct_t;






// Function prototypes. The autograder (i.e. the testbench) only uses these
// functions to test your design. Please do not change these definitions or
// the autograder will fail.
void     reset_queue      (void);
bool     write_queue      (uint16_t  charHandle, uint32_t  bufLength, uint8_t *buffer); 
bool     read_queue       (uint16_t *charHandle, uint32_t *bufLength, uint8_t *buffer);
void     get_queue_status (uint32_t *wptr, uint32_t *rptr, bool *full, bool *empty);
uint32_t get_queue_depth  (void);



#endif // __MY_CIRCULAR_BUFFER__

