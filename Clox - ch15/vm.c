#include <stdio.h>

#include "common.h"
#include "debug.h"
#include "memory.h" //ch 15 q3
#include "vm.h"

VM vm;

//ch 15 q3
static void resetStack() {
  vm.stackCount = 0;
}

void initVM() {
  vm.stack = NULL;       //ch 15 q3
  vm.stackCapacity = 0;  //ch 15 q3
  resetStack();          //ch 15 q3
}

void freeVM() {
  FREE_ARRAY(Value, vm.stack, vm.stackCapacity); //ch 15 q3
  vm.stack = NULL;                               //ch 15 q3
  vm.stackCapacity = 0;                          //ch 15 q3
  vm.stackCount = 0;                             //ch 15 q3
}

//ch 15 q3
void push(Value value) {
  if (vm.stackCapacity < vm.stackCount + 1) {
    int oldCapacity = vm.stackCapacity;
    vm.stackCapacity = GROW_CAPACITY(oldCapacity);
    vm.stack = GROW_ARRAY(Value, vm.stack,
                          oldCapacity, vm.stackCapacity);
  }

  vm.stack[vm.stackCount] = value;
  vm.stackCount++;
}

//ch 15 q3
Value pop() {
  vm.stackCount--;
  return vm.stack[vm.stackCount];
}

static InterpretResult run() {
#define READ_BYTE() (*vm.ip++)
#define READ_CONSTANT() (vm.chunk->constants.values[READ_BYTE()])

  for (;;) {
#ifdef DEBUG_TRACE_EXECUTION
    //ch 15 q3: print the stack contents
    printf("          ");
    for (Value *slot = vm.stack; slot < vm.stack + vm.stackCount; slot++) {
      printf("[ ");
      printValue(*slot);
      printf(" ]");
    }
    printf("\n");
    disassembleInstruction(vm.chunk,
                           (int)(vm.ip - vm.chunk->code));
#endif
    uint8_t instruction;
    switch (instruction = READ_BYTE()) {
      case OP_CONSTANT: {
        Value constant = READ_CONSTANT();
        push(constant); //ch 15 q3 (replaces printValue/printf)
        break;
      }
      case OP_NEGATE: //ch 15 q4
        // negate in place on the stack; no pop/push so stackCount is untouched
        vm.stack[vm.stackCount - 1] = -vm.stack[vm.stackCount - 1]; //ch 15 q4
        break; //ch 15 q4
      case OP_RETURN: {
        printValue(pop()); //ch 15 q3
        printf("\n");       //ch 15 q3
        return INTERPRET_OK;
      }
    }
  }

#undef READ_BYTE
#undef READ_CONSTANT
}

InterpretResult interpret(Chunk* chunk) {
  vm.chunk = chunk;
  vm.ip = vm.chunk->code;
  resetStack(); //ch 15 q3
  return run();
}
