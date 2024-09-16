#pragma once
#include "Global.h"

class lexical_analysis
{
private:
	#define KEYWORDS_SIZE 22

	#define NUMBER_ERROR_P 1
	#define NUMBER_ERROR_E 2
	#define CHAR_ERROR 3
	#define OPERATOR_ERROR 4
	#define NOTE_NOT_MAP 5
	#define QUOTATION_NOW_MAP 6

	#define WRONG_CHAR			"There is a wrong character."
	#define WRONG_NUMBER_E		"A number can't have char except a E."
	#define WRONG_NUMBER_POINT	"The point's behind can only be number."
	#define WRONG_OPERATOR		"The operator's behind can only be number."
	#define WRONG_NOTE			"The note is not end."
	#define WRONG_QUOTATION		"The quotation is not end."

	ifstream input_file_;

	string         input_file_name_;
	vector<string> lines_;

	string keywords_[KEYWORDS_SIZE] = {
				"break", "case", "char", "continue", "do", "default", "double", "else",
				"float", "for", "if", "int", "include", "long", "main", "return", "switch",
				"typedef", "void", "unsigned", "while", "iostream"
			};

public:
	vector<string> words;
	vector<string> types;
	vector<string> error_list;

	lexical_analysis();
	explicit lexical_analysis(const string &);
	void     input_file_name(const string &);
	void     read_file();
	void     read_line();
	void     analysis_word();
	void     error(unsigned long long, unsigned long long, int);

	static bool is_letter(char);
	static bool is_digit(char);
	bool        is_keyword(const string &) const;
	static char is_space(unsigned long long &, const string &);

private:
	// 新增的用于处理不同状态的私有函数
	void handle_initial_state(unsigned long long &, unsigned long long, string &, int &);
	void handle_identifier_state(char, string &, unsigned long long &, int &);
	void handle_digit_state(char, string &, unsigned long long &, int &);
	void handle_less_than_state(char, string &, unsigned long long &, int &);
	void handle_greater_than_state(char, string &, unsigned long long &, int &);
	void handle_comment_start_state(char, unsigned long long &, const string &, int &);
};
