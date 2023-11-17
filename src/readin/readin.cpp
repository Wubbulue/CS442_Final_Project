#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>


#include "readin.hpp"


#define LINESIZE 20000
#define SKEW 2


// UNUSED, UNTESTED
/*
char *getline(char *line, int maxlinelen) {
   int i = 0;
   while (i < maxlinelen) {
      int ch = mgetchar();
      if (isaborted()) return NULL;
      switch (ch) {
         case CR:
            prevchar = CR;
            line[i] = 0;
            return line;
         case LF:
            if (prevchar != CR) {
               prevchar = LF;
               line[i] = 0;
               return line;
            }
            // if CR+LF (DOS) then ignore the LF
            break;
         case EOF:
            if (i == 0) return NULL;
            line[i] = 0;
            return line;
         default:
            prevchar = ch;
            line[i++] = (char) ch;
            break;
      }
   }
   line[i] = 0;      // silently truncate long line
   return line;
}
*/

bool isPattern(const char *line) {

   // Find end of line, or terminating '!' character, whichever comes first:
   const char *end = line;
   while (*end && *end != '!') ++end;

   // Verify that '!' (if present) is the final printable character:
   if (*end == '!') {
      for (const char *p = end + 1; *p; ++p) {
         if ((unsigned)*p > ' ') {
            return false;
         }
      }
   }

   // Ensure line consists of valid tokens:
   bool prev_digit = false, have_digit = false;
   for (const char *p = line; p != end; ++p) {
      if ((unsigned)*p <= ' ') {
         if (prev_digit) return false;  // space inside token!
      } else if (*p >= '0' && *p <= '9') {
         prev_digit = have_digit = true;
      } else if (*p == 'b' || *p == 'o' || *p == '$') {
         prev_digit = false;
      } else {
         return false;  // unsupported printable character encountered!
      }
   }
   if (prev_digit) return false;  // end of line inside token!

   // Everything seems parseable; assume this is RLE if either we saw some
   // digits, or the pattern ends with a '!', both of which are unlikely to
   // occur in plain text patterns:
   return have_digit || *end == '!';
}


void readPattern(bool* A, int nrows, int ncols, const char *line)
{
    int n = 0, x = 0, y = 0;
    int xoff = SKEW, yoff = SKEW;

    const char *p;

    for (p = line; *p; p++) 
    {
        char c = *p;
        if ('0' <= c && c <= '9') 
        {
            n = n * 10 + c - '0' ;
        }
        else 
        {
            if (n == 0)
                n = 1;
            if (c == 'b' || c == '.') 
            {
                x += n;
            }
            else if (c == '$') 
            {
                x = 0;
                y += n;
            }
            else if (c == '!') {
                return;
            } 
            else if (('o' <= c && c <= 'y') || ('A' <= c && c <= 'X')) {
                int state = -1;
                if (c == 'o')
                    state = 1;
                else if (c < 'o') 
                {
                    state = c - 'A' + 1;
                } 
                else 
                {
                    state = 24 * (c - 'p' + 1);
                    p++;
                    c = *p;
                    if ('A' <= c && c <= 'X') 
                    {
                        state = state + c - 'A' + 1;
                    } 
                    else 
                    {
                       state = 1;
                       p--;
                    }
                }
                // write run of cells to grid checking cells are within any bounded grid
                if (nrows == 0 || y < nrows) 
                {
                    while (n-- > 0) 
                    {
                        if (ncols == 0 || x < ncols) 
                        {
                            A[y*ncols + x] = (bool)state;
                        }
                        x++;
                    }
                }
            }
            n = 0;
        }
    }

}

bool * loadPattern(int nrows, int ncols, const char *filename)
{
    std::ifstream in{filename};
    std::string contents{(std::istreambuf_iterator<char>(in)),
                     std::istreambuf_iterator<char>()};

    const char* chars {contents.c_str()};
    
    unsigned int size_A = nrows*ncols;
    bool* A = new bool[size_A];

    if (isPattern(chars)) 
    {
        readPattern(A, nrows, ncols, chars);
    }
    else
    {
        std::cout << "Not a valid pattern";
    }
    return A;
}



