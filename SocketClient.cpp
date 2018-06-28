#include "SocketClient.h"

SocketClient * SocketClient::create(std::string ip, int port)
{
	auto s = new SocketClient(ip,port);
	s->thread_ = new std::thread(
		std::bind(static_cast<std::size_t(asio::io_service::*)()>(&asio::io_service::run),
			&s->io_service_));
	return s;
}

void SocketClient::close()
{
	do_close();
}

std::vector<GameMessage>  SocketClient::get_game_messages()
{
	auto game_message_set_string = read_data();
	return GameMessageWrap::set_string_to_vector(game_message_set_string);
}

void SocketClient::send_game_message(const std::vector<GameMessage>& vec_game_msg)
{
	auto set_string = GameMessageWrap::vector_to_set_stirng(vec_game_msg);
	write_data(set_string);
}

void SocketClient::send_string(std::string s)
{
	if (error_flag_)
		return;
	write_data(s);
}

std::string SocketClient::get_string()
{
	 return read_data();
}

void SocketClient::do_close()
{
	try {
		std::lock_guard<std::mutex> lk{ mut };//创建锁名1k？
		error_flag_ = true;//将错误标识换成true
		SocketMessage empty_msg;
		memcpy(empty_msg.data(), "0001\0", 5);
		read_msg_deque_.push_back(empty_msg);
		data_cond_.notify_one();//环境变量
		io_service_.stop();
		boost::system::error_code ec;
		socket_.shutdown(asio::ip::tcp::socket::shutdown_both, ec);
		if (!ec)
			throw boost::system::system_error(ec);
		socket_.close();
		thread_->join();

	}
	catch (std::exception&e)
	{
		std::cout<<"Close Exception:"<<e.what()<<std::endl;
	}
}

int SocketClient::camp() const
{
	while (!start_flag_);
	return camp_;
}

int SocketClient::total() const
{
	while(!start_flag_)
	return total_;
}

void SocketClient::write_data(std::string s)
{
	SocketMessage msg;
	if (s.size() == 0)
	{
		s = std::string("\0");
		msg.body_length(1);
	}
	else
	msg.body_length(s.size());
	memcpy(msg.body(), &s[0u], msg.body_length());
	msg.encodeHeader();
	asio::write(socket_,asio::buffer(msg.data(), msg.length()));//同步写入
}

void SocketClient::start_connect()
{
	socket_.async_connect(endpoint_, std::bind(&SocketClient::handle_connect, this, std::placeholders::_1));
}

void SocketClient::handle_connect(const boost::system::error_code & error)
{
	try
	{
		if (!error)
		{

			std::cout << "connected\n";
			log("conected already");
			char data[30] = { 0 };
			char write[30] = { 0 };
			std::cin >> write;
			boost::system::error_code error;
			
			size_t length = socket_.read_some(asio::buffer(data, 30), error);
			std::cout << "huohuo" << std::endl;
			if (error || length < 10)
				throw boost::system::system_error(error);
			char header[4 + 1] = "";
			strncat(header, data+10, 4);//这是在编辑header，但是这个data+10真的无法理解
			total_ = atoi(header);//嚯嚯嚯，这是下
			camp_ = atoi(data + 14);//获取阵营参数
			log("%d %d ", total_, camp_);
			log("%s",data);
			start_flag_ = true;
			asio::async_read(socket_, asio::buffer(read_msg_.data(), SocketMessage::header_length), std::bind(&SocketClient::handle_read_header, this, std::placeholders::_1));
			
		}
		else
		{
			std::cerr << "failed to connect" << std::endl;
			//			throw asio::system_error(error);
			error_flag_ = true;

		}
	}
	catch (std::exception& e)
	{
		//		std::terminate();
		//		do_close();
		std::cerr << "Exception in connection: " << e.what() << "\n";
	}
}

void SocketClient::handle_read_header(const system::error_code & error)
{
	if (!error && read_msg_.decodeHeader())
	{
		asio::async_read(socket_,asio::buffer(read_msg_.body(), read_msg_.body_length()),std::bind(&SocketClient::handle_read_body, this, std::placeholders::_1));
	}
	else
	{
		do_close();
	}
}

void SocketClient::handle_read_body(const boost::system::error_code & error)
{
	if (!error)
	{
		std::lock_guard<std::mutex> lk{ mut };
		read_msg_deque_.push_back(read_msg_);
		data_cond_.notify_one();
		std::cout << "read completed\n";

		asio::async_read(socket_,
			asio::buffer(read_msg_.data(), SocketMessage::header_length),
			std::bind(&SocketClient::handle_read_header, this,
				std::placeholders::_1));
	}
	else
	{
		do_close();
	}
}

std::string SocketClient::read_data()
{
	if (error_flag_)
		return "";
	std::unique_lock<std::mutex> lk{ mut };
	while (read_msg_deque_.empty())
		data_cond_.wait(lk);
	auto read_msg = read_msg_deque_.front();
	read_msg_deque_.pop_front();
	lk.unlock();
	auto ret = std::string(read_msg.body(), read_msg.body_length());
	return ret;
}
