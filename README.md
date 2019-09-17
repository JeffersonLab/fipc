# FIPC
Fast inter-process communication package.

### Shared memory structure
The source service creates a shared memory and populates it with a 
data, read from a data storage or DAQ system.
Shared memory consists of

a) *N_THREADS* equal size data-blocks, 
where *N_THREADS* are number of parallel workers processing 
different data-blocks.

b) *block_id_flag* atomic integer that informs source of the shared memory 
about the end of the data processing  of a specific data-block.

c) *S_Flags* array of atomic integers. The position in this array defines 
the link position of a service-branches in the data processing application service chain. 
The value defines the number of consumer services on a branch. E.g.

```` 
s1 - s2 - s3 - s4 - s5
|         |         |
s11      s31       s51
|                   |
s12                s52
                    |
                   s53
[3]  [1] [2]  [1]  [4]
S_Flags
 ````