`lockfree::spsc_queue`

- implemented with cached cursors

- write test and benchmark

`lockfree::mpmc_stack_1` 

- write test and benchmark

- simple to_be_deleted list (freed when only one thread is in pop() function)

`lockfree::mpmc_stack_2` (planned) 

- use hazard pointers

`lockfree::mpmc_stack_3` (planned) 

- use reference counting
