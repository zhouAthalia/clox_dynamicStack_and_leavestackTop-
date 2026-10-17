#ifndef clox_vm_h
#define clox_vm_h

#include "chunk.h"
#include "value.h" //ch 15 q3

typedef struct {
  Chunk* chunk;
  uint8_t* ip;
  Value* stack;      //ch 15 q3
  int stackCount;    //ch 15 q3
  int stackCapacity; //ch 15 q3
} VM;

typedef enum {
  INTERPRET_OK,
  INTERPRET_COMPILE_ERROR,
  INTERPRET_RUNTIME_ERROR
} InterpretResult;

void initVM();
void freeVM();
InterpretResult interpret(Chunk* chunk);
void push(Value value); //ch 15 q3
Value pop();            //ch 15 q3

#endif
