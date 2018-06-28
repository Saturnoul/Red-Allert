#pragma once
#define ASIO_STANDALLONE
#include<boost/asio.hpp>
#include<functional>
#include <boost/thread.hpp>
#include<chrono>
#include<deque>
#include<iostream>
#include"GameMessageWrap.h"
#include"SocketMessage.h"
using boost::asio::ip::tcp;
using namespace boost;
class SocketClient
{
public:

	static SocketClient* create(std::string ip = "192.168.0.11", int port = 8008);
	void close();

	void start()
	{
		start_connect();
	};
	int camp_, total_;
	[[deprecated("just for test")]]
	std::vector<GameMessage>  get_game_messages();

	[[deprecated("just for test")]]
	void send_game_message(const std::vector<GameMessage>& vec_game_msg);

	void send_string(std::string s);


	std::string get_string();;

	void do_close();

	bool started() const { return start_flag_; }

	bool error()const { return error_flag_; }

	int camp() const;

	int total() const;

private:
	SocketClient(std::string ip, int port) : socket_(io_service_),
		endpoint_(boost::asio::ip::address_v4::from_string(ip), port)
	{
		start();
	}

	void write_data(std::string s);

	void start_connect();

	void handle_connect(const boost::system::error_code& error);

	void handle_read_header(const system::error_code& error);

	void handle_read_body(const boost::system::error_code& error);

	std::string read_data();
private:

	asio::io_service io_service_;
	tcp::socket socket_;
	tcp::endpoint endpoint_;
	std::deque<SocketMessage> read_msg_deque_;
	SocketMessage read_msg_;

	bool start_flag_{ false }, error_flag_{ false };

	std::thread *thread_, *read_thread_;
	

	std::condition_variable data_cond_;
	std::mutex mut;
};
