#include "lexical_analysis.h"

int main()
{
	lexical_analysis lexical_analysis;
	lexical_analysis.read_line();
	lexical_analysis.analysis_word();

	for (unsigned long long i = 0; i < lexical_analysis.words.size(); i++)
	{
		cout << lexical_analysis.words[i] << "\t" << lexical_analysis.types[i] << endl;
	}
	return 0;
}
