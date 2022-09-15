/** @file
 *  @author Little Clover
 *  @date 2022-09-14 14:11:46 PM +0800
 */
#ifndef STACK_H
#define STACK_H

#include "http-header.h"

/**
 * Define stack as a pointer to a StackNode
 */
typedef struct StackNode* Stack;
/**
 * Define the next pointed stack node as a StackNode
 */
typedef struct StackNode* NextStackNode;

/**
 * A stack contain the header information and pointer to next node using linked
 * list implementation
 *
 * @attention Most of the linked stack operation defined in this library assume
 *            a header stack unless specified.
 */
struct StackNode {
  /** HTTP header */
  Element element;
  /** next node */
  NextStackNode next;
};

/**
 * Initialise an empty stack
 *
 * @return An empty stack
 */
Stack stackInit(void);

/**
 * Free a stack
 *
 * @param stack A stack to be freed
 */
void stackFree(Stack stack, void (freeMethod)(Element));

/**
 * Push an element to the top a stack
 *
 * @param element Element to be pushed
 * @param stack   A stack
 */
void stackPush(Element element, Stack stack);

/**
 * Pop the top element to a stack
 *
 * @param stack A stack
 */
void stackPop(Stack stack, void (freeMethod)(Element));

#endif
