#pragma once

#if defined(BOARD)
  #include BOARD
#else
  #error "Board macro not set to a correct option"
#endif
