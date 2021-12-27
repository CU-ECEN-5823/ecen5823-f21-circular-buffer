/***********************************************************************
 * @file      circular_buffer.c
 * @version   0.1
 * @brief     Function implementation file.
 *
 * @author    Awesome Student, Awesome.Student@Colorado.edu
 * @date      Dec 22, 2021
 *
 * @institution University of Colorado Boulder (UCB)
 * @course      ECEN 5823-001: IoT Embedded Firmware
 * @instructor  David Sluiter
 *
 * @assignment Assignment 1.5 - Circular Buffer
 * @due        
 *
 * @resources   
 * 
 */

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>


#include "circular_buffer.h"


// Declare memory for the queue/buffer/fifo, and the write and read pointers
queue_struct_t   my_queue[QUEUE_DEPTH]; // the queue
uint32_t         wptr = 0;              // write pointer
uint32_t         rptr = 0;              // read pointer
// Decide how will you handle the full condition. 



// ---------------------------------------------------------------------
// Private function used only by this .c file. 
// compute next ptr value
// Isolation of functionality: This defines "how" a pointer advances. 
// ---------------------------------------------------------------------
static uint32_t nextPtr(uint32_t ptr) {

  // Student edit:
  // Create this function




} // nextPtr()


// ---------------------------------------------------------------------
// Public function
// This function writes an entry to the queue.
// Returns false if successful or true if writing to a full fifo.
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
// Returns false if successful or true if reading from an empty fifo. 
// ---------------------------------------------------------------------
bool read_queue (uint8_t *a, uint16_t *b) {

  // Student edit:
  // Create this function
  
  // Isolation of functionality: 
  //     Create the logic for "when" a pointer advances
  
  
  
} // read_queue() 



// ---------------------------------------------------------------------
// Public function
// This function returns the wptr, rptr, full and empty values.
// ---------------------------------------------------------------------
void get_queue_status (uint32_t *_wptr, uint32_t *_rptr, bool *_full, bool *_empty) {

  // Student edit:
  // Create this function
  
  
  
  

} // get_queue_status() 



// ---------------------------------------------------------------------
// Public function
// Function that computes the number of entries in the queue
// ---------------------------------------------------------------------
uint32_t get_queue_depth() {

  // Student edit:
  // Create this function
  
  
  
  

} // get_queue_depth() 








