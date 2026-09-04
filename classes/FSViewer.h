#pragma once

#include <iostream>
#include <string>
#include <vector>

struct file_viewer {
	file_viewer(std::vector<std::string*> _t) : tokens(_t), position(0), string_flag(false) {}
	~file_viewer() {}
	std::vector<std::string*> tokens;
	int position;
	bool string_flag;
	std::string peek(int ahead = 1) const
	{
		if (position + ahead > tokens.size()) {
			return "\0";
		}
		else {
			return *tokens[position];
		}
	}
	std::string consume() {
		return *tokens[position++];
	}
	void forward()
	{
		position++;
		if (peek() == "\"") {
			string_flag = !string_flag;
		}
	}
	std::string while_peek(std::string start, std::string end) 
	{
		std::string buffer;
		if (peek() == start) {
			forward();
			while (peek() != end)
			{
				buffer += consume();
			}
		}
		else {
			return {};
		}
		return buffer;
	}
	std::string detect_string() 
	{
		std::string buffer;
		if (peek() == "\"") {
			forward();
			while (string_flag)
			{
				buffer += consume();
			}
		}
		else {
			return {};
		}
		return buffer;
	}
	void continue_if(std::string to_find, bool is, std::string at) 
	{
		while ((peek() == to_find) == is) {
			forward();
			if (position >= tokens.size())
			{
				std::cout << "ERROR: Missing " << to_find << " at " << position << " " << at << std::endl;
				exit(EXIT_FAILURE);
			}
		}
	}
};
struct string_viewer {
	string_viewer(std::string _t) : tokens(_t), position(0), string_flag(false) {}
	~string_viewer() {}
	std::string tokens;
	int position;
	bool string_flag;
	char peek(int ahead = 1) const
	{
		if (position + ahead > tokens.size()) {
			return '\0';
		}
		else {
			return tokens[position];
		}
	}
	char consume() {
		return tokens[position++];
	}
	void forward()
	{
		position++;
		if (peek() == '\"') {
			string_flag = !string_flag;
		}
	}
	std::string while_peek(char start, char end)
	{
		std::string buffer;
		if (peek() == start) {
			forward();
			while (peek() != end)
			{
				buffer += consume();
			}
		}
		else {
			return {};
		}
		return buffer;
	}
	std::string detect_string()
	{
		std::string buffer;
		if (peek() == '\"') {
			forward();
			while (string_flag)
			{
				buffer += consume();
			}
		}
		else {
			return {};
		}
		return buffer;
	}
	void continue_if(char to_find, bool is, char at)
	{
		while ((peek() == to_find) == is) {
			forward();
			if (position >= tokens.size())
			{
				std::cout << "ERROR: Missing " << to_find << " at " << position << " " << at << std::endl;
				exit(EXIT_FAILURE);
			}
		}
	}
};
static std::vector<std::string> tokenize_string(std::string str) 
{
	std::vector<std::string> result;
	for (auto& s : str) {
		result.push_back(str);
	}
	return result;
}