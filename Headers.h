#pragma once
// include all the header files here
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <iostream>
#include <algorithm>
#include <random>
#include <vector>
#include <fstream>
#include <string>



// check for windows compiler
#ifdef __WIN32__			
#include <Windows.h>  
#include <crtdbg.h>
#include <climits>
#endif // __WIN32__

// check for gcc compiler : __GNUC__
