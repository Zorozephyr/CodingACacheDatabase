ltrace -f for debugging 
strace -f bash -c "time ls" 2>&1 | grep -i time

getrusage(int who, struct rusage *usage);

wait system call suspends thread execution untill one of its kid terminates
sort -R wl.txt | head -1000 > wl.1000.txt

for x in $(sort -R wl.txt | head -1000);do echo "${x}kd";done > wl.50000.txt
root@VISHNUNA01:/mnt/c/ProjectsToPushToGit/C_Projects/CodingADatabase/cache22# sort -R wl.txt | head -49000 >> wl.50000.txt

wc -l wl.50k.txt


1k results:
    Duration: 0.89750
    ret:     0
    tv_sec   0
    tv_usec  897500

    Duration: 1.04509
    ret:     0
    tv_sec   0
    tv_usec  147588