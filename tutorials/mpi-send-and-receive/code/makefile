EXECS=send_recv send_recv_nonblk ping_pong ring
MPICC?=mpicc

all: ${EXECS}

send_recv: send_recv.c
	${MPICC} -o send_recv send_recv.c

send_recv_nonblk: send_recv_nonblk.c
	${MPICC} -o send_recv_nonblk send_recv_nonblk.c

ping_pong: ping_pong.c
	${MPICC} -o ping_pong ping_pong.c

ring: ring.c
	${MPICC} -o ring ring.c

clean:
	rm -f ${EXECS}
