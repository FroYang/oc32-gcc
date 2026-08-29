// enable-execute-stack-empty.c
// Empty implementation of __enable_execute_stack for targets that don't need it

extern void __enable_execute_stack (void *);

void
__enable_execute_stack (void *addr __attribute__((unused)))
{
  /* Do nothing.  */
}