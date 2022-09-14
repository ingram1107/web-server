#include "stack.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

Stack stackInit(void) {
  Stack newStack = malloc(sizeof(struct StackNode));
  if (!newStack) {
    errno = ENOMEM;
    perror("stack");
    exit(EXIT_FAILURE);
  }

  newStack->next = NULL;
  return newStack;
}

void stackPop(Stack stack, void (freeMethod)(Element)) {
  NextStackNode top;

  if (stack->next) {
    top = stack->next;
    stack->next = stack->next->next;
    freeMethod(top->element);
    free(top);
  } else fprintf(stderr, "stack: empty stack\n");
}

void stackFree(Stack stack, void (freeMethod)(Element)) {
  do {
    stackPop(stack, freeMethod);
  } while (stack->next);
}

void stackPush(Element element, Stack stack) {
  NextStackNode tmp;

  tmp = malloc(sizeof(struct StackNode));
  if (!tmp) {
    errno = ENOMEM;
    perror("stack");
    exit(EXIT_FAILURE);
  }

  tmp->element = element;
  tmp->next = stack->next;
  stack->next = tmp;
}
