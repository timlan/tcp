// Not Transmission Control Protocol

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <cmath>
#include <limits>

class block
{
public:
	block * parent;
	std::vector<block*> blocks;
	std::string value;

	block()
	{
		parent = this;
		value = "";
	}

	block(block * parentN)
	{
		parent = parentN;
		value = "";
	}

	block(block * parentN, std::string valueN)
	{
		parent = parentN;
		value = valueN;
	}

	void destroyMe()
	{
		for (int i = 0; i < blocks.size(); i++)
		{
			blocks[i]->destroyMe();
			delete blocks[i];
		}
	}

	void coutValue()
	{
		if (value != "")
			std::cout << value << " ";
		else
		{
			std::cout << "( ";
			if (blocks.size() == 3)
			{
    			blocks[2]->coutValue();
    			blocks[0]->coutValue();
    			blocks[1]->coutValue();
			}
            else
            {
                for (int i = 0; i < blocks.size(); i++)
			    {
			    	blocks[i]->coutValue();
			     }
            }
			std::cout << ") ";
		}
	}
};

class dictWord
{
public:
	std::string eng, timlan;

	dictWord(std::string timlanN, std::string engN)
	{
		eng = engN;
		timlan = timlanN;
	}
};

std::string pres[] = { "soho", "coko", "naha", "peve", "ceve", "wuhi", "rela", "relo", "pece", "toko" };
std::string posts[] = { "ah", "amo", "eta", "ame", "ami", "eht", "oht", "aht", "elem" };
std::vector<dictWord> dictionary;

std::string parse(std::string);
int getNum(char);
int getNumDigits(int);
std::vector<std::string> split(std::string, std::string);
std::vector<std::string> getPres(std::string&);
std::vector<std::string> getPosts(std::string&);
void eatDB();

int main()
{
	eatDB();
	std::string input;
	std::cout.precision(50);
	std::cout << "Welcome! Epsilon is " << std::numeric_limits<double>::epsilon() << std::endl << " $ ";
	while (!std::cin.eof())
	{
		std::getline(std::cin, input);
		if (input == "exit")
			return 0;
		parse(input);
		std::cout << std::endl << " $ ";
	}
}

void eatDB()
{
	std::ifstream file("DB.dat");
	std::string input;
	if (file.is_open())
	{
		while (file.good())
		{
			std::getline(file, input);
			std::vector<std::string> data = split(input, "<br />");
			for (int i = 0; i < data.size(); i++)
			{
				std::string line = data[i];
				int d0 = line.find('=');
				if (d0 == std::string::npos)
					return;
				int d1 = line.find('(');
				if (d1 == std::string::npos)
					return;
				dictionary.push_back(dictWord(line.substr(0, d0 - 1), line.substr(d0 + 2, d1 - d0 - 3)));
			}
		}
		file.close();
	}
	else
		std::cout << "file not openabled :/" << std::endl << std::endl;
}

double getFullNumber(std::string timlan)
{
	double num = 0;
	int len = timlan.length() / 2;
	double pwr = pow(16.0, len-1);
	bool neg = false;
	for (int i = 0; i < len; i++)
	{
		num += getNum(timlan[i * 2]) * pwr;
		if(timlan[i * 2] == 'w')
		{
			if(pwr >= 1)
				num /= 16;
			else
				pwr *= 16;
			neg = !neg;
		}
		if(timlan[i * 2] == 'z')
		{
			num /= pow(16.0,len - i);
			pwr = 1;
		}
		if(timlan[i * 2] == 'x')
		{
			if(pwr >= 1)
				num /= pow(16.0,len - i);
			num *= pow(16,getFullNumber(timlan.substr(i*2+2)));
			break;
		}
		pwr /= 16;

	}
	if(neg)
		num = -num;
	return num;
}

std::string getEng(std::string timlan)
{
	if (timlan.substr(0, 3) == "eli" && timlan.length() > 3)
	{
		int len = timlan.length();
		std::string heh = "";
		heh += timlan[3] ^ 0x20;
		for (int i = 5; i < len; i += 2)
		{
			heh += timlan[i];
		}
		return heh;
	}
	if ((timlan.substr(0, 2) == "ta" || timlan.substr(0, 2) == "to") && timlan.length() > 3)
	{
		int len = timlan.length();
		std::string heh = "prev.";
		bool bad = false;
		for (int i = 1; i < len; i += 2)
		{
			if(timlan[i-1] != 't')
			{
				bad = true;
				break;
			}
			switch(timlan[i])
			{
			case 'o':
				heh += " obj's";
				break;
			case 'a':
				heh += " sbj's";
				break;
			case 'q':
				heh += " clause's";
				break;
			default:
				bad = true;
				break;
			}
			if(bad)
				break;
		}
		if(!bad)
		{
			heh.erase(heh.length()-2,2);
			return heh;
		}
	}
	for (int i = 0; i < dictionary.size(); i++)
	{
		if (dictionary[i].timlan == timlan)
			return dictionary[i].eng;
	}
	if (timlan.substr(1, 1) == "-")
	{
		// NUMBR!!!
		double num = getFullNumber(timlan);
		std::stringstream ss;
		std::string meh;
		ss.precision(50);
		ss << num;
		ss >> meh;
		return meh;
	}
	return timlan;
}

int getNumDigits(int num)
{
	int check = 1;
	int i = 0;
	while (check < num)
	{
		i++;
		check *= 10;
	}
	return i;
}

int getNum(char chr)
{
	switch(chr)
	{
	case 'b':
		return 0;
	case 'c':
		return 1;
	case 'd':
		return 2;
	case 'f':
		return 3;
	case 'g':
		return 4;
	case 'h':
		return 5;
	case 'j':
		return 6;
	case 'k':
		return 7;
	case 'l':
		return 8;
	case 'm':
		return 9;
	case 'n':
		return 10;
	case 'p':
		return 11;
	case 'r':
		return 12;
	case 's':
		return 13;
	case 't':
		return 14;
	case 'v':
		return 15;
	}
	return 0;
}

std::string parse(std::string input)
{
	std::vector<std::string> sentences = split(input, ".");
	int scnt;
	for(scnt=0;scnt<sentences.size();scnt++)
	{
		std::vector<std::string> data = split(sentences.at(scnt), " ");
		block * topBlock = new block();
		block * curBlock = topBlock;
		bool isVerb, isSup;
		std::string sup, temp;
		bool nospacey = false;
		for (int j = 0; j < data.size(); j++)
		{
			std::string word = data[j];

			if (word.substr(0, 1) == "/")
			{
				std::string bracketType = word.substr(1, 1);
				if (word.substr(word.length() - 1, 1) == "/" && word.substr(word.length() - 2, 1) == bracketType)
				{
					word = word.substr(2, word.length() - 4);
					goto miaw;
				}
				word = word.substr(2, data[j].length() - 2) + " ";
			more:
				j++;
				if(j>=data.size())
				{
					if(scnt<sentences.size()-1)
					{
						scnt++;
						std::vector<std::string> data2 = split(sentences.at(scnt), " ");
						data.insert(data.end(), data2.begin(), data2.end());
						data[j] = "." + data[j];
						nospacey = true;
					} else {
						std::cout << "Y'Software scale!";
						return "";
					}
				}
				if (data[j].length() > 1 && data[j].substr(data[j].length() - 1, 1) == "/" && data[j].substr(data[j].length() - 2, 1) == bracketType)
				{
	                                if(!nospacey)
        	                                word += " ";
					else
						word.erase(word.length()-1,1);
					word += data[j].substr(0, data[j].length() - 2);
					nospacey = false;
					// somehwat duplicate code maybe?
				miaw:
					if (bracketType == "*")
						continue;
					curBlock->blocks.push_back(new block(curBlock, word));
					sup = "";

					while (curBlock->blocks.size() == 3)
					{
						if (curBlock == curBlock->parent)
						{
							std::cout << "Y'Swale!";
							return "";
						}
						else
							curBlock = curBlock->parent;
					}

					continue;
				}
				if(nospacey)
					word.erase(word.length()-1,1);
				word += data[j] + " ";
				nospacey = false;
				goto more;
			}

			if (word == "")
				continue;

			isVerb = false;
			isSup = false;
			temp = "";
			std::vector<std::string> pre = getPres(word);
			std::vector<std::string> post = getPosts(word);

			if (word == "puc")
			{
				temp += "[puc]";
				isVerb = true;
			}
			else
				temp += getEng(word);

			for (int i = 0; i < pre.size(); i++)
			{
				if (pre[i] == "soho")
					temp += " [plural]";
				if (pre[i] == "coko")
					temp += " [question]";
				if (pre[i] == "naha")
					temp += " [negative]";
				if (pre[i] == "peve")
				{
					isVerb = true;
					temp += " [preposition]";
				}
				if (pre[i] == "ceve")
				{
					isVerb = true;
					temp += " [conjunction]";
				}
				if (pre[i] == "wuhi")
					temp += " [unknown]";
				if (pre[i] == "rela")
					temp += " [relative sbj]";
				if (pre[i] == "relo")
					temp += " [relative obj]";
				if (pre[i] == "pece")
					temp += " [perfect]";
				if (pre[i] == "toko")
					temp += " [imperative]";
			}

			for (int i = 0; i < post.size(); i++)
			{
				if (post[i] == "ah")
				{
					isSup = true;
					temp += " [descriptive] ";
				}
				if (post[i] == "eta")
				{
					isSup = true;
					temp += " [determiner] ";
				}
				if (post[i] == "ami")
				{
					isSup = true;
					temp += " [possesive] ";
				}
				if (post[i] == "amo")
				{
					isSup = true;
					temp += " [modal] ";
				}
				if (post[i] == "ame")
				{
					isSup = true;
					temp += " [descriptive cl] ";
				}
				if (post[i] == "elem")
				{
					isVerb = true;
					temp += " [generic]";
				}
				if (post[i] == "eht")
				{
					isVerb = true;
					temp += " [present]";
				}
				if (post[i] == "oht")
				{
					isVerb = true;
					temp += " [past]";
				}
				if (post[i] == "aht")
				{
					isVerb = true;
					temp += " [future]";
				}
			}

			if (isVerb)
			{
				block * newBlock = new block(curBlock);
				curBlock->blocks.push_back(newBlock);
				curBlock = newBlock;
			}

			if (isSup)
				sup += temp;
			else
			{
				curBlock->blocks.push_back(new block(curBlock, sup + temp));
				sup = "";
			}

			while (curBlock->blocks.size() == 3)
			{
				if (curBlock == curBlock->parent)
				{
					std::cout << "Y'Swale!";
					return "";
				}
				else
					curBlock = curBlock->parent;
			}

		}
		topBlock->coutValue();
		topBlock->destroyMe();
		delete topBlock;
		std::cout << " . ";
	}
	return "";
}

std::vector<std::string> split(std::string str, std::string s)
{
	std::vector<std::string> output;
	int ni;
	while ((ni = str.find(s)) != std::string::npos)
	{
		output.push_back(str.substr(0, ni));
		str = str.erase(0, ni + s.length());
	}
	output.push_back(str);
	return output;
}

std::vector<std::string> getPres(std::string& str)
{
	std::vector<std::string> output;
restart:
	for (int i = 0; i < sizeof(pres)/sizeof(pres[0]); i++)
	{
		if (str.length() > pres[i].length())
		{
			if (str.substr(0, pres[i].length()) == pres[i])
			{
				output.push_back(pres[i]);
				str.erase(0, pres[i].length());
				goto restart;
			}
		}
	}
	return output;
}

std::vector<std::string> getPosts(std::string& str)
{
	std::vector<std::string> output;
restart:
	for (int i = 0; i < sizeof(posts)/sizeof(posts[0]); i++)
	{
		if (str.length() > posts[i].length())
		{
			if (str.substr(str.length() - posts[i].length(), posts[i].length()) == posts[i])
			{
				output.push_back(posts[i]);
				str.erase(str.length() - posts[i].length(), posts[i].length());
				goto restart;
			}
		}
	}
	return output;
}
