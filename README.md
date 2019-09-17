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
and and positive number, set by the worker service will indicate processing is done.

c) *S_Flags* array of atomic integers. Each service will have its own position in this array, 
indicating the data processing status: 1 being busy processing and 0 being ready to process a 
block of data.  