Program parameters:

-t [address space mean]: The mean of a normal distriubtion,
used to randomly virtual address space size = 2^t
-v [address size standard deviation]: The standard deviation of a normal distriubtion,
used to randomly virtual address space size

-n [page size]: Size of each page = 2^n

-l [process lifetime average]: Mean of a normal distribution, used to randomly
select lifetimes (how many references a program can make)

-p [locality probability]: Probability used for a coin flip test when a process
is in the OPEN phase, which determines how likely will the next reference be local

-w [length of a phase]: Describes how many references should be many before a program
has a chance of switching phases

-o [output file]: Output file, default is stdout
-j [number of processes]: Number of processes to simulate
