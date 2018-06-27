#pragma once
#define ASIO_STANDALONE
#define ASIO_HAS_STD_CHRONO
#include<boost\asio.hpp>
#include <thread>
#include <iostream>
#include <condition_variable>
#include <deque>
#include "SocketMessage.h"
#include "GameMessageWrap.h"

using boost::asio::ip::tcp;

typedef std::shared_ptr<tcp::socket> socket_ptr;

class SocketServer;
class TcpConnection: public std::enable_shared_from_this<TcpConnection>
{
public:
	typedef std::shared_ptr<TcpConnection> pointer;
	//	~TcpConnection();
	static pointer create(boost::asio::io_service& io_service, SocketServer* parent);
	tcp::socket& socket();

	void start();
	void write_data(std::string s);


	std::string read_data();
	bool error()const { return error_flag_; }


	void do_close();
private:

	void handle_read_header(const boost::system::error_code& error);

	void handle_read_body(const boost::system::error_code& error);


	TcpConnection(boost::asio::io_service& io_service, SocketServer* parent);;

	void check_timer();
	void delete_from_parent();

	tcp::socket socket_;
	SocketServer* parent;
	bool error_flag_{ false };

	SocketMessage read_msg_;
	std::deque<SocketMessage> read_msg_deque_;
	std::condition_variable data_cond_;
	std::mutex mut_;
	//	asio::steady_timer steady_timer_;

};

class SocketServer
{
public:


	static SocketServer* create(int port = 8008);

	void close();

	std::vector<TcpConnection::pointer> get_connection() const;


	void remove_connection(TcpConnection::pointer p);

	void button_start();


	bool error() const;
	 boost::asio::io_service* get_io_service()const
	{
		 return io_service_;
	}
	int connection_num() const;
private:
	SocketServer(int port);
	void start_accept();

	void handle_accept(TcpConnection::pointer new_connection,
		const boost::system::error_code& error);

	void loop_process();


	boost::asio::ip::tcp::acceptor acceptor_;
	std::vector<TcpConnection::pointer> connections_;
	int connection_num_;

	static boost::asio::io_service* io_service_;

	std::thread *thread_, *button_thread_{ nullptr };
	std::mutex delete_mutex_;
	bool error_flag_{ false };
	std::condition_variable data_cond_;
};