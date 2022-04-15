EXECS=send_recv_nonblk
MPICC?=mpicc

all: ${EXECS}

send_recv_nonblk: send_recv_nonblk.c
	${MPICC} -o send_recv_nonblk send_recv_nonblk.c

clean:
	rm -f ${EXECS}
