server: server.o res_head.o resource.o exit_msg.o serv_req.o 
	gcc -o server server.o res_head.o resource.o exit_msg.o serv_req.o

server.o: server.c
	gcc -o server.o server.c -c -ansi -pedantic

serv_req.o: serv_req.c serv_req.h
	gcc -o serv_req.o serv_req.c -c -ansi -pedantic

res_head.o: res_head.c res_head.h
	gcc -o res_head.o res_head.c -c -ansi -pedantic

resource.o: resource.c resource.h
	gcc -o resource.o resource.c -c -ansi -pedantic

exit_msg.o: exit_msg.c exit_msg.h
	gcc -o exit_msg.o exit_msg.c -c -ansi -pedantic

clean:
	@rm -f *.o server core
	
