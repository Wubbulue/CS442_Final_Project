# Parallelizing Game of Life
### CS 442/542 Final Project

<img src="https://github.com/Wubbulue/CS442_Final_Project/blob/main/example_video.gif" title="Simulation rendered using our code!" width="256" height="256" />

By Anthony Sharma, Jack Wickstrom, Ben Ogden, and Jack Ringer

## Overview

This project examines how Conway's Game of Life (GoL) can be improved through parallelization in MPI. We examine three versions of GoL:
1) Serial implementation
   * One process
   * Calculating next state cell-by-cell

3) Naive Parallel
   * Split into subgrids
   * Each process updates its subgrid 
   * Communicate at end of every iteration

3) Improved parallel
   * Same as Naive Parallel, but using shared memory windows to reduce communication costs

Our codebase also supports saving simulation data to a binary file and a renderer to visualize said simulation. For more details and information, please see the project report.

## Acknowledgement

We would like to thank the UNM Center for Advanced Research Computing, supported in part by the National Science Foundation, for providing the high performance computing resources used in this work.
