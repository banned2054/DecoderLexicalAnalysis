#include "lexical_analysis.h"

lexical_analysis::lexical_analysis()
= default;

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
	string             line;
	int                state         = 0;
	unsigned long long mark_line     = 0;
	unsigned long long mark_position = 0;
	for (unsigned long long i = 0; i < lines_.size(); i++)
	{
		line = lines_[i];
		string word;
		for (unsigned long long j = 0; j < line.size(); j++)
		{
			char now_char = line[j];
			switch (state)
			{
				case 0 :
				{
					word     = "";
					now_char = is_space(j, line);
					if (j >= line.size())
					{
						break;
					}
					word.push_back(now_char);
					if (is_letter(now_char))
					{
						state = 1;
						break;
					}
					if (is_digit(now_char))
					{
						state = 2;
						break;
					}
					switch (now_char)
					{
						case '<' :
						{
							state = 8;
							break;
						}
						case '>' :
						{
							state = 9;
							break;
						}
						case ':' :
						{
							state = 10;
							break;
						}
						case '/' :
						{
							state         = 11;
							mark_line     = i;
							mark_position = j;
							break;
						}
						case '=' :
						{
							state = 0;
							words.push_back(word);
							types.emplace_back(IS_SYMBOL);
							break;
						}
						case '+' :
						{
							state = 0;
							words.push_back(word);
							types.emplace_back(IS_ADDTION);
							break;
						}
						case '-' :
						{
							state = 0;
							words.push_back(word);
							types.emplace_back(IS_SUBTRATION);
							break;
						}
						case '*' :
						{
							state = 0;
							words.push_back(word);
							types.emplace_back(IS_MULTPLY);
							break;
						}
						case '(' :
						{
							state = 0;
							words.push_back(word);
							types.emplace_back(IS_LEFT_BRACKET);
							break;
						}
						case ')' :
						{
							state = 0;
							words.push_back(word);
							types.emplace_back(IS_RIGHT_BRACKET);
							break;
						}
						case ';' :
						{
							state = 0;
							words.push_back(word);
							types.emplace_back(IS_SYMBOL);
							break;
						}
						case '\'' :
						{
							state         = 13;
							mark_line     = i;
							mark_position = j;
							break;
						}
						case '\"' :
						{
							state         = 14;
							mark_line     = i;
							mark_position = j;
							break;
						}
						default :
						{
							state = 15;
							word.pop_back();
							break;
						}
					}
					break;
				}
				case 1 :
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
					break;
				}
				case 2 :
				{
					if (is_digit(now_char))
					{
						word.push_back(now_char);
						state = 2;
						break;
					}
					switch (now_char)
					{
						case '.' :
						{
							state = 3;
							word.push_back(now_char);
							break;
						}
						case 'E' :
						{
							state = 5;
							word.push_back(now_char);
							break;
						}
						default :
						{
							j--;
							state = 0;
							words.push_back(word);
							types.emplace_back(IS_NUMBER);
							break;
						}
					}
					break;
				}
				case 3 :
				{
					if (is_digit(now_char))
					{
						state = 4;
						word.push_back(now_char);
					}
					else
					{
						state = 0;
						words.push_back(word);
						types.emplace_back(IS_NUMBER);
						error(i, j, NUMBER_ERROR_P);
					}
					break;
				}
				case 4 :
				{
					if (is_digit(now_char))
					{
						state = 4;
						word.push_back(now_char);
					}
					else if (now_char == 'E')
					{
						state = 5;
						word.push_back(now_char);
					}
					else
					{
						j--;
						state = 0;
						words.push_back(word);
						types.emplace_back(IS_NUMBER);
					}
					break;
				}
				case 5 :
				{
					if (is_digit(now_char))
					{
						state = 7;
						word.push_back(now_char);
					}
					else if (now_char == '+' || now_char == '-')
					{
						state = 6;
						word.push_back(now_char);
					}
					else
					{
						j--;
						words.push_back(word);
						types.emplace_back(IS_NUMBER);
						error(i, j, NUMBER_ERROR_E);
					}
					break;
				}
				case 6 :
				{
					if (is_digit(now_char))
					{
						state = 7;
						word.push_back(now_char);
					}
					else
					{
						j--;
						words.push_back(word);
						types.emplace_back(IS_NUMBER);
						error(i, j, OPERATOR_ERROR);
					}
					break;
				}
				case 7 :
				{
					if (is_digit(now_char))
					{
						state = 7;
						word.push_back(now_char);
					}
					else
					{
						j--;
						words.push_back(word);
						types.emplace_back(IS_NUMBER);
						state = 0;
					}
					break;
				}
				case 8 : //"<"
				{
					state = 0;
					switch (now_char)
					{
						case '=' :
						{
							word.push_back(now_char);
							break;
						}
						case '>' :
						{
							word.push_back(now_char);
							break;
						}
						default :
						{
							j--;
							break;
						}
					}
					types.emplace_back(IS_SYMBOL);
					words.push_back(word);
					break;
				}
				case 9 : //">"
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
					break;
				}
				case 10 : //"��"
				{
					if (now_char == '=')
					{
						word.push_back(now_char);
					}
					else
					{
						j--;
					}
					state = 0;
					words.push_back(word);
					types.emplace_back(IS_SYMBOL);
					break;
				}
				case 11 : //ע�Ϳ�ʼ
				{
					if (now_char == '/') state = 16;
					if (now_char == '*') state = 12;
					else
					{
						j--;
						state = 0;
						words.push_back(word);
						types.emplace_back(IS_DIVIDER);
					}
					break;
				}
				case 12 : //ע�ͽ�β
				{
					if (now_char == '*')
					{
						if (j < line.size() - 1)
						{
							if (line[j + 1] == '/')
							{
								j++;
								state = 0;
							}
						}
					}
					else state = 12;
					break;
				}
				case 13 : //����ƥ��
				{
					if (now_char == '\\')
					{
						word.push_back(now_char);
						word.push_back(line[j + 1]);
						j += 2;
					}
					else
					{
						if (now_char == '\'')
						{
							word.push_back(now_char);
							words.push_back(word);
							types.emplace_back(IS_CHARACTER);
							state = 0;
						}
						else
						{
							word.push_back(now_char);
						}
					}
					break;
				}
				case 14 : //����ƥ��
				{
					if (now_char == '\\')
					{
						word.push_back(now_char);
						word.push_back(line[j + 1]);
						j += 2;
					}
					else
					{
						if (now_char == '\"')
						{
							word.push_back(now_char);
							words.push_back(word);
							types.emplace_back(IS_SENTENCE);
							state = 0;
						}
						else
						{
							word.push_back(now_char);
						}
					}
					break;
				}
				case 15 : //��������
				{
					error(i, --j, CHAR_ERROR);
					state = 0;
					break;
				}
				case 16 : //"//"����ע��
				{
					if (j == line.size() - 1) state = 0;
					break;
				}
				default : ;
			}
			if (j >= line.size() - 1)
			{
				if (state == 1)
				{
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
	if (state == 12)
	{
		error(mark_line, mark_position, NOTE_NOT_MAP);
	}
	if (state == 13 || state == 14)
	{
		error(mark_line, mark_position, QUOTATION_NOW_MAP);
	}
}

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
		case NUMBER_ERROR_P :
		{
			wrong_reason += WRONG_NUMBER_POINT;
			break;
		}
		case NUMBER_ERROR_E :
		{
			wrong_reason += WRONG_NUMBER_E;
			break;
		}
		case CHAR_ERROR :
		{
			wrong_reason += WRONG_CHAR;
			break;
		}
		case OPERATOR_ERROR :
		{
			wrong_reason += WRONG_OPERATOR;
			break;
		}
		case NOTE_NOT_MAP :
		{
			wrong_reason += WRONG_NOTE;
			break;
		}
		case QUOTATION_NOW_MAP :
		{
			wrong_reason += WRONG_QUOTATION;
			break;
		}
		default : ;
	}
	error_list.push_back(wrong_reason);
}
