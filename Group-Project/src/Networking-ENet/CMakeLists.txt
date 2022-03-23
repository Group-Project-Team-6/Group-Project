cmake_minimum_required(VERSION 3.22.1)
project(Group-Project)


#Common Component
add_library(ENet STATIC
	callbacks.c
	compress.c
	host.c
	list.c
	packet.c
	peer.c
	protocol.c
	win32.c	

	enet/callbacks.h
	enet/enet.h
	enet/list.h
	enet/protocol.h
	enet/time.h
	enet/types.h
	enet/utility.h
	enet/win32.h	
)