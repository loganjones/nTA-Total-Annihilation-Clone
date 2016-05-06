//
//  std_Platform_macOS.cpp
//  nTA
//
//  Created by Logan Jones on 5/5/16.
//  Copyright © 2016 Logan Jones. All rights reserved.
//

#include "std_Platform.h"

char * itoa( int value, char * buffer, int radix ) {
    if (radix == 10) {
        sprintf(buffer, "%d", value);
    }
    else {
        buffer[0] = '#';
        buffer[1] = '\0';
    }
    return buffer;
}

char * ltoa( long value, char * buffer, int radix ) {
    if (radix == 10) {
        sprintf(buffer, "%ld", value);
    }
    else {
        buffer[0] = '#';
        buffer[1] = '\0';
    }
    return buffer;
}
