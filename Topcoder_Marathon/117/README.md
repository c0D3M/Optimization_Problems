Challenege Details: https://www.topcoder.com/challenges/30122730?tab=details  
Given an NxN grid of numbers your task is to move the numbers to their target locations. A number n at row r and column c (both 0-based) is in its target location if n = r*N + c + 1. A move consists of selecting a square subgrid and rotating all its numbers by 90 degrees clockwise or counter clockwise. Rotating a subgrid of size S (2 <= S <= N) incurs a **penalty of floor((S-1)^1.5)**. A number that does not finish in its target location is penalized by P*dist, where P is a weighting input parameter and dist is the Manhattan distance to its target location. Your task is to minimize the total penalty.

[Sample] (Opt2.gif)  
