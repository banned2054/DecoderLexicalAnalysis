#include "Lexical_analysis.h"

int main()
{
	Lexical_analysis lexical_analysis;
	lexical_analysis.Read_line();
	lexical_analysis.analysis_word();

	for (int i = 0; i < lexical_analysis.words.size(); i++)
	{
		cout << lexical_analysis.words[i] << "\t" << lexical_analysis.types[i] << endl;
	}
	return 0;
}