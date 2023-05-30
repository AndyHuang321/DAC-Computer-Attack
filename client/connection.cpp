#include "connection.h"

bool Connection::recv() {
	while (true) {
		SSIZE_T rc = ::recv(socket, &in[cursor], messageLen - cursor, 0);

		if (rc < 0) {
			if (errno == EWOULDBLOCK || errno == EAGAIN) {
				return true;
			}
			return false;
		}
		if (rc == 0) {
			return false;
		}
		cursor += (int)rc;

		if (cursor == sizeof(messageLen)) {
			if (in.size() < sizeof(messageLen)) {
				in.resize(sizeof(messageLen));
			}
			messageLen = *(uint32_t*)&in[0];
			messageLen = ntohl(messageLen);
			messageLen += sizeof(messageLen);
			in.resize(messageLen);
		}

		if (cursor == messageLen) {
			messageLen = sizeof(messageLen);
			cursor = 0;
			in.erase(0, sizeof(messageLen));
			complete = true;
			return true;
		}
	}
}

const std::string& Connection::message()
{
	if (complete) {
		complete = false;
		return in;
	}
	return "";
}

bool Connection::send() {
	SSIZE_T rc = ::send(socket, &out[0], out.size(), 0);
	if (rc < 0) {
		if (errno == EWOULDBLOCK || errno == EAGAIN) {
			return true;
		}
		return false;
	}
	out.erase(0, rc);
	return true;
}
