#include "lexical_analysis.h"

lexical_analysis::lexical_analysis() = default;

lexical_analysis::lexical_analysis(const string &file_name)
{
	input_file_name_ = file_name;
	input_file_.open(file_name, ios::in);
}

void lexical_analysis::input_file_name(const string &file_name)
{
	input_file_name_ = file_name;
	input_file_.open(file_name);
}

void lexical_analysis::read_file()
{
	string line;
	lines_.clear();
	while (getline(input_file_, line))
	{
		lines_.push_back(line);
	}
}

void lexical_analysis::read_line()
{
	string line;
	lines_.clear();
	getline(cin, line);
	lines_.push_back(line);
}

void lexical_analysis::analysis_word()
{
	int                          state         = 0;
	constexpr unsigned long long mark_line     = 0;
	constexpr unsigned long long mark_position = 0;
	for (unsigned long long i = 0; i < lines_.size(); i++)
	{
		string line = lines_[i];
		string word;
		for (unsigned long long j = 0; j < line.size(); j++)
		{
			const char now_char = line[j];
			switch (state)
			{
				case 0 : handle_initial_state(j, i, word, state);
					break;
				case 1 : handle_identifier_state(now_char, word, j, state);
					break;
				case 2 : handle_digit_state(now_char, word, j, state);
					break;
				case 8 : handle_less_than_state(now_char, word, j, state);
					break;
				case 9 : handle_greater_than_state(now_char, word, j, state);
					break;
				case 11 : handle_comment_start_state(now_char, j, word, state);
					break;
				default : ;
				// 继续处理其他状态...
			}
			// 每行末尾状态处理逻辑
			if (j >= line.size() - 1)
			{
				if (state == 1)
				{
					words.push_back(word);
					types.emplace_back(is_keyword(word) ? IS_KEYWORD : IS_WORD);
				}
				if (state >= 2 && state <= 7)
				{
					words.push_back(word);
					types.emplace_back(IS_NUMBER);
				}
				if (state >= 8 && state <= 10)
				{
					words.push_back(word);
					types.emplace_back(IS_SYMBOL);
				}
			}
		}
	}
	// 文件末尾的状态处理
	if (state == 12)
	{
		error(mark_line, mark_position, NOTE_NOT_MAP);
	}
	if (state == 13 || state == 14)
	{
		error(mark_line, mark_position, QUOTATION_NOW_MAP);
	}
}

// 处理初始状态的函数
void lexical_analysis::handle_initial_state(unsigned long long &j, const unsigned long long i,
                                            string &            word,
                                            int &               state)
{
	word                = "";
	const char now_char = is_space(j, lines_[i]);
	if (j >= lines_[i].size()) return;

	word.push_back(now_char);
	if (is_letter(now_char))
	{
		state = 1;
		return;
	}
	if (is_digit(now_char))
	{
		state = 2;
		return;
	}

	switch (now_char)
	{
		case '<' : state = 8;
			break;
		case '>' : state = 9;
			break;
		case ':' : state = 10;
			break;
		case '/' : state = 11;
			break;
		case '=' : words.push_back(word);
			types.emplace_back(IS_SYMBOL);
			state = 0;
			break;
		case '+' : words.push_back(word);
			types.emplace_back(IS_ADDITION);
			state = 0;
			break;
		case '-' : words.push_back(word);
			types.emplace_back(IS_SUBTRACTION);
			state = 0;
			break;
		case '*' : words.push_back(word);
			types.emplace_back(IS_MULTIPLY);
			state = 0;
			break;
		case '(' : words.push_back(word);
			types.emplace_back(IS_LEFT_BRACKET);
			state = 0;
			break;
		case ')' : words.push_back(word);
			types.emplace_back(IS_RIGHT_BRACKET);
			state = 0;
			break;
		case ';' : words.push_back(word);
			types.emplace_back(IS_SYMBOL);
			state = 0;
			break;
		case '\'' : state = 13;
			break;
		case '\"' : state = 14;
			break;
		default : state = 15;
			word.pop_back();
			break;
	}
}

// 处理标识符状态的函数
void lexical_analysis::handle_identifier_state(const char now_char, string &word, unsigned long long &j, int &state)
{
	if (is_letter(now_char) || is_digit(now_char))
	{
		state = 1;
		word.push_back(now_char);
	}
	else
	{
		state = 0;
		j--;
		words.push_back(word);
		if (is_keyword(word))
		{
			types.emplace_back(IS_KEYWORD);
		}
		else
		{
			types.emplace_back(IS_WORD);
		}
	}
}

// 处理数字状态的函数
void lexical_analysis::handle_digit_state(const char now_char, string &word, unsigned long long &j, int &state)
{
	if (is_digit(now_char))
	{
		word.push_back(now_char);
		state = 2;
	}
	else if (now_char == '.')
	{
		word.push_back(now_char);
		state = 3;
	}
	else if (now_char == 'E')
	{
		word.push_back(now_char);
		state = 5;
	}
	else
	{
		j--;
		words.push_back(word);
		types.emplace_back(IS_NUMBER);
		state = 0;
	}
}

// 处理小于号的函数
void lexical_analysis::handle_less_than_state(const char now_char, string &word, unsigned long long &j, int &state)
{
	if (now_char == '=' || now_char == '>')
	{
		word.push_back(now_char);
	}
	else
	{
		j--;
	}
	words.push_back(word);
	types.emplace_back(IS_SYMBOL);
	state = 0;
}

// 处理大于号的函数
void lexical_analysis::handle_greater_than_state(const char now_char, string &word, unsigned long long &j, int &state)
{
	if (now_char == '=')
	{
		word.push_back(now_char);
	}
	else
	{
		j--;
	}
	words.push_back(word);
	types.emplace_back(IS_SYMBOL);
	state = 0;
}

// 处理注释开始的函数
void lexical_analysis::handle_comment_start_state(const char now_char, unsigned long long &j, const string &word,
                                                  int &      state)
{
	if (now_char == '/')
	{
		state = 16; // 单行注释
	}
	else if (now_char == '*')
	{
		state = 12; // 多行注释
	}
	else
	{
		j--;
		words.push_back(word);
		types.emplace_back(IS_DIVIDER);
		state = 0;
	}
}

// 其他辅助函数
bool lexical_analysis::is_digit(const char character)
{
	return (character >= '0') && (character <= '9');
}

bool lexical_analysis::is_letter(const char character)
{
	return ((character >= 'a') && (character <= 'z')) || ((character >= 'A') && (character <= 'Z')) || character == '_';
}

bool lexical_analysis::is_keyword(const string &word) const
{
	for (const auto &keyword : keywords_)
	{
		if (word == keyword) return true;
	}
	return false;
}

char lexical_analysis::is_space(unsigned long long &j, const string &line)
{
	char now = line[j];
	while (now == ' ')
	{
		j++;
		if (j >= line.size())
		{
			return ' ';
		}
		now = line[j];
	}
	return now;
}

void lexical_analysis::error(const unsigned long long line_number, const unsigned long long position,
                             const int                wrong_type)
{
	string wrong_reason = "there is an error in line " + std::to_string(line_number) + " on " + std::to_string(position)
			+ "\n\tThe reason is:";
	switch (wrong_type)
	{
		case NUMBER_ERROR_P : wrong_reason += WRONG_NUMBER_POINT;
			break;
		case NUMBER_ERROR_E : wrong_reason += WRONG_NUMBER_E;
			break;
		case CHAR_ERROR : wrong_reason += WRONG_CHAR;
			break;
		case OPERATOR_ERROR : wrong_reason += WRONG_OPERATOR;
			break;
		case NOTE_NOT_MAP : wrong_reason += WRONG_NOTE;
			break;
		case QUOTATION_NOW_MAP : wrong_reason += WRONG_QUOTATION;
			break;
		default : break;
	}
	error_list.push_back(wrong_reason);
}
