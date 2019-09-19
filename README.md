# FIPC
Fast inter-process communication package.

### Shared memory structure
The source service creates a shared memory and populates it with a 
data, read from a data storage or DAQ system.
Shared memory consists of

a) *N_BLOCKS* equal size data-blocks, 
where *N_BLOCKS* are number of parallel workers processing 
different data-blocks.

b) *B_Flags* array of atomic integers. Index corresponds to the block id and value 
indicates the block processing status. E.g. 0 means data is fresh updated by the source service,
and and positive number: 1, set by the worker service will indicate processing is done.

c) *S_Flags* array of atomic integers. Each service will have its own position in this array, 
indicating the data processing status: -1 being busy processing and position of the buffer 
indicates that service is done processing.

Each worker service knows the it's position: *chain_id* in the *S_Flags*. 
*S_Flags chain_id -1* value indicates the previous service processing status: -1 processing and 
nBuff processing is done. *chain_id = 0* indicates that worker service is not linked to any other 
services (terminal service).
