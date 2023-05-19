/***********************************************************************
 * @file      circular_buffer.c
 * @version   0.2
 * @brief     Function implementation file.
 *
 * @author    Awesome Student, Awesome.Student@Colorado.edu
 * @date      Dec 22, 2021. Initial file.
 *
 *            Sept 2, 2022. Updated some of the function header comments with 
 *            some additional clarifying statements. Changed assignment to 0.5.
*
 *            Jan 24, 2023. Added/updated some comments to add clarity for students.
 *            
 * @institution University of Colorado Boulder (UCB)
 * @course      ECEN 5823-001: IoT Embedded Firmware
 * @instructor  David Sluiter
 *
 * @assignment Assignment 0.5 - Circular Buffer
 * @due        
 *
 * @resources   
 * 
 */

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>


#include "circular_buffer.h"


// Global variable for this assignment
// Declare memory for the queue/buffer/fifo, 
// and the write and read pointers
queue_struct_t   my_queue[QUEUE_DEPTH]; // This is the storage for your queue

uint32_t         wptr = 0;              // write pointer
uint32_t         rptr = 0;              // read pointer

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
// Public function
// This function writes an entry to the queue if the the queue is not full.
// Input parameter "a" should be written to queue_struct_t element "a"
// Input parameter "b" should be written to queue_struct_t element "b"
// Returns bool false if successful or true if writing to a full fifo.
// i.e. false means no error, true means an error occurred. 
// ---------------------------------------------------------------------
bool write_queue (uint8_t a, uint16_t b) {

  // Student edit:
  // Create this function
  // Decide how you want to handle the "full" condition.

  // Isolation of functionality: 
  //     Create the logic for "when" a pointer advances



} // write_queue() 


// ---------------------------------------------------------------------
// Public function
// This function reads an entry from the queue.
// Write the values of a and b from my_queue[rptr] to the memory addresses
// pointed at by *a and *b. In this implementation, we do it this way because
// standard C does not provide a mechanism for a C function to return multiple
// values, as is common in perl or python.
// Returns bool false if successful or true if reading from an empty fifo. 
// i.e. false means no error, true means an error occurred. 
// ---------------------------------------------------------------------
bool read_queue (uint8_t *a, uint16_t *b) {

  // Student edit:
  // Create this function
  
  // Isolation of functionality: 
  //     Create the logic for "when" a pointer advances
  
  
  
} // read_queue() 



// ---------------------------------------------------------------------
// Public function
// This function returns the wptr, rptr, full and empty values, writing
// to memory using the pointer values passed in, same rationale as read_queue()
// ---------------------------------------------------------------------
void get_queue_status (uint32_t *_wptr, uint32_t *_rptr, bool *_full, bool *_empty) {

  // Student edit:
  // Create this function
  
  
  
  

} // get_queue_status() 



// ---------------------------------------------------------------------
// Public function
// Function that computes the number of written entries currently in the queue. If there
// are 3 entries in the queue, it should return 3. If the queue is empty it should 
// return 0. If the queue is full it should return either QUEUE_DEPTH if
// USE_ALL_ENTRIES==1 otherwise returns QUEUE_DEPTH-1. 
// ---------------------------------------------------------------------
uint32_t get_queue_depth() {

  // Student edit:
  // Create this function
  
  
  
  

} // get_queue_depth() 








