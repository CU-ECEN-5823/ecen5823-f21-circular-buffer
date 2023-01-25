/***********************************************************************
 * @file      circular_buffer.h
 * @version   0.1
 * @brief     Function header/interface file.
 *
 * @author    Awesome Student, Awesome.Student@Colorado.edu
 * @date      Dec 22, 2021
 *            Sept 2, 2022. Changed assignment to 0.5.
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

// Define everything that a caller needs to know



#ifndef __MY_CIRCULAR_BUFFER__
#define __MY_CIRCULAR_BUFFER__




// This is the number of entries in the queue. Please leave
// this value set to 16.
#define QUEUE_DEPTH      (16)
// Student edit: 
//   define this to 1 if your design uses all array entries
//   define this to 0 if your design leaves 1 array entry empty
#define USE_ALL_ENTRIES  (1)




// Modern C (circa 2021 does it this way)
// This is referred to as an anonymous struct definition.
// This is the structure of 1 queue/buffer/FIFO entry.
// Please do not change this definition. 
typedef struct {
  uint8_t       a;
  uint16_t      b;
} queue_struct_t;





// Function prototypes. The autograder (i.e. the testbench) only uses these
// functions to test your design. Please do not change these definitions.
bool     write_queue (uint8_t a, uint16_t b); 
bool     read_queue (uint8_t *a, uint16_t *b);
void     get_queue_status (uint32_t *_wptr, uint32_t *_rptr, bool *_full, bool *_empty);
uint32_t get_queue_depth (void);



#endif // __MY_CIRCULAR_BUFFER__

