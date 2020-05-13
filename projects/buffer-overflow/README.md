In this assignment, you will explore buffer overflows. Make sure
that you test everything using the official course docker image.

## Security Mechanisms

 * The C compiler uses a security mechanism called "Stack Guard",
   which detects and prevents buffer overflows. We have provided
   you with a Makefile that compiles your code with this disabled
   by default.

 * Additionally, the operating system performs address space
   randomization (ASLR), which does not prevent buffer overflows,
   but makes it much harder for them to result in a successful
   exploit. You will need to work around this for this project!
   As a hint, ASLR will move things around in blocks, so stack and
   heap offsets will differ between runs, but the fundamental
   structure will not be changed.

## Exploiting a Vulnerable Program

We have provided you with a vulnerable program in `stack.c` -- you are
not to modify this. The Makefile will compile this as needed to allow
us to exploit it, including making it setuid-root. To compile, you
should run the `build.sh` script, which will do the compilation in a
container running the `baseline` image.

This program has a function `bof()`, which includes a buffer-overflow
vulnerability. The program reads from the network, does some useless
work with the provided data, and echoes back what you sent it. You
will be attacking it over the network.

We have provided you with an initial skeleton for `exploit_1.c`.  The
term shellcode literally refers to code that starts a command shell,
like bash. However, nowadays shellcode is considered to be any byte
code that can be inserted into an exploit to accomplish a particular
objective. The shellcode provided in `exploit_1.c` is designed to
execute the command `cat /var/secret/token`. When run normally, this
command results in permission denied since `/var/secret` is accessible
only to root. You will attempt to execute this command by exploiting
the buffer overflow vulnerability in `stack.c`

### What to do:

Write a program named `exploit_1.c` that creates and sends a buffer
overflow to `stack.c`. The message you send should include:

 - shellcode
 - target address in the stack to which control should go
   when bof() returns
 - NOP instructions to increase the chance of success

`exploit_1` takes no command line arguments. If your exploit is
implemented correctly, then when `bof()` returns it will execute your
shellcode, printing the contents of `/var/secret/token`.

Running the code in a container is a bit more complex than running it
locally. To help with this, we have provided scripts `run_stack.sh`
and `run_exploit.sh`. You can also start an interactive container, so
that you can run the programs multiple times without restarts, as well
as using `gdb`. To do this, run:

    docker run -ti --rm -v "$(pwd):/opt" baseline

Because the container captures the output from your programs, if you've used
the `run_stack.sh` script you can view stdout and stderr with:

    docker logs stack

You will also need to clean up the container when you are done:

    docker rm stack

### How to submit your assignment:

The only file you should need to modify is `exploit_1.c`. Add, commit, and
push your solution, so that it is uploaded to the server:

    git add exploit_1.c
    git commit
    git push origin master

