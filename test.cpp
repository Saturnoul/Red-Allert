#include<boost/asio.hpp>
#include"cocos2d.h"
#include"SocketClient.h"
int main()
{
	SocketClient* client = SocketClient::create("127.0.0.1",38);
}