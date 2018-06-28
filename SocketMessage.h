#pragma once
#ifndef CHAT_MESSAGE_HPP
#define CHAT_MESSAGE_HPP
#include<cstdio>
#include<string>
#include<cstdlib>
class SocketMessage
{
public:
	enum { header_length = 4 };//the header is use to recognize the real length of the message body
	enum { max_body_length = 8192 };//the longest message you can send
	SocketMessage()
	{
		m_body_length = 0;
	}
	const char *data()const
	{
		return m_data;
	}
	char* data()
	{
		return m_data;
	}
	size_t length()const
	{
		return m_body_length + header_length;
	}
	 char*body()
	{
		return m_data + header_length;
	}
	size_t body_length()
	{
		return m_body_length;
	}
	void body_length(size_t new_length)
	{
		m_body_length = new_length;
		if (m_body_length > max_body_length)
			m_body_length = max_body_length;
	}
	bool decodeHeader()
	{
		char header[header_length + 1] = "";
		std::strncat(header, m_data, header_length);
		m_body_length = atoi(header);
		if (m_body_length > max_body_length)
		{
			m_body_length = 0;
			return false;
		}
		return true;
	}
	void encodeHeader()
	{
		char header[header_length + 1] = "";
		sprintf(header, "%4d", static_cast<int>(m_body_length));//get the length of the message and add it to the header_length variable
		std::memcpy(m_data, header, header_length);
	}
private:
	size_t m_body_length;
	char m_data[header_length + max_body_length];//message header and messagebody header，
};
#endif // !CHAT_MESSAGE_HPP
