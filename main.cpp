#include <QCoreApplication>
#include <fstream>
#include <iostream>
#include <QFile>
#include <QTextStream>
#include <set>

std::ostream & operator << (std::ostream &os, const QString & toOut)
{
	os << toOut.toStdString();
	return os;
}

struct Word
{
	QString init;
	QString alpab;
	int num;
	Word(QString a, QString b, int c) : init{a}, alpab{b}, num{c}
	{

	}
	Word() {}
};

bool operator< (const Word & in1, const Word & in2)
{

//	std::cout << in1.alpab << "   " << in2.alpab << std::endl;
	for(int i = 0; i < std::min(in1.alpab.size(), in2.alpab.size()); ++i)
	{
		if(in1.alpab[i].unicode() < in2.alpab[i].unicode())
		{
			return true;
		}
		else if(in1.alpab[i].unicode() > in2.alpab[i].unicode())
		{
			return false;
		}
	}
	return false;
}

template <typename T>
void eraseItems(std::vector<T> & inVect,
				const std::vector<int> & indices)
{
	const int initSize = inVect.size();
	std::set<int, std::less<int>> excludeSet; // less first
	for(auto item : indices)
	{
		excludeSet.emplace(item);
	}
	std::vector<int> excludeVector;
	for(auto a : excludeSet)
	{
		excludeVector.push_back(a);
	}
	excludeVector.push_back(initSize);

	for(int i = 0; i < int(excludeVector.size()) - 1; ++i)
	{
		for(int j = excludeVector[i] - i; j < excludeVector[i + 1] - i - 1; ++j)
		{
			inVect[j] = std::move(inVect[j + 1 + i]);
		}
	}
	inVect.resize(initSize - excludeSet.size());
}
template void eraseItems(std::vector<Word> & inVect, const std::vector<int> & indices);

void cleanDoubleSolution(std::vector<Word> & words)
{

	/// sort by alpab
	std::sort(std::begin(words), std::end(words));

	/// set to erase
	std::vector<int> eraseIndices;
	for(int i = 0; i < words.size() - 1; ++i)
	{
		if(words[i + 1].alpab == words[i].alpab)
		{
			eraseIndices.push_back(i);
			eraseIndices.push_back(i + 1);
			int c = 2;
			while(words[i + c].alpab == words[i].alpab)
			{
				eraseIndices.push_back(i + c);
				++c;
			}
			i += c;
		}
	}

	eraseItems(words, eraseIndices);

	std::sort(std::begin(words), std::end(words),
			  [](const Word & in1, const Word & in2)
	{
		return in1.num < in2.num;
	});
}

void cleanByRegExp(std::vector<Word> & words, QRegExp reg)
{

	std::vector<int> eraseIndices;

	for(int i = 0; i < words.size(); ++i)
	{
		if(words[i].init.contains(reg)) eraseIndices.push_back(i);
	}
	eraseItems(words, eraseIndices);
}

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);
	const QString projPath = "../FreqVocabular";



	QFile inFile(projPath + "/singleSolutionCleaned2.txt");
	inFile.open(QIODevice::ReadOnly);
	QTextStream inStream(&inFile);

	QFile outFile(projPath + "/singleSolutionCleaned3.txt");
	outFile.open(QIODevice::WriteOnly);
	QTextStream outStream(&outFile);



	std::vector<Word> words;

	/// read file
	QString str;
	int num = 0;
	while(!inStream.atEnd())
	{
		str = inStream.readLine();
		if(str.size() != 6) break;
		QString tmp = str;
		std::sort(std::begin(tmp), std::end(tmp));
		words.push_back(Word(str, tmp, num++));
	}
	inFile.close();


	cleanByRegExp(words, QRegExp("[шх]"));




	/// write file
	for(const Word & in : words)
	{
		outStream  << in.init << "\r\n";
	}
	outFile.close();

	return 0;
}
