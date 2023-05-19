#include "connection.h"

void Connection::recv() {
	while(true) {
		SSIZE_T rc = ::recv(socket, &in[cursor], messageLen - cursor, 0);

		if (rc < 0) {
			if (errno == EWOULDBLOCK || errno == EAGAIN) {
				return;
			}
		}
		if (rc == 0) {
			return;
		}
		cursor += (int)rc;

		if (cursor == sizeof(messageLen)) {
			messageLen = *(uint16_t*)&in[0];
			messageLen = ntohs(messageLen);
			messageLen += sizeof(messageLen);
			in.resize(messageLen);
		}

		if (cursor == messageLen) {
			parse();
			messageLen = sizeof(messageLen);
			cursor = 0;
			in.resize(messageLen);
		}
	}
}

void Connection::send() {
	SSIZE_T rc = ::send(socket, &out[0], out.size(), 0);
	if (rc < 0) {
		if (errno == EWOULDBLOCK || errno == EAGAIN) {
			return;
		}
	}

	out.erase(0, rc);
}
