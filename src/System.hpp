#pragma once
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <array>
#include <algorithm>
#include <fstream>
#include <iterator>
#include <stack>
#include <cassert>
#include <typeinfo>
#include <stdexcept>
#include <iomanip>
#include <set>
#include <unordered_set>
#include <functional>
#include <cctype>
#include "rlutil.h"

namespace System {

	class Vector {
	public:
		template<typename type>
		static void push_range(std::vector<type> &subject, std::vector<type> range) {
			subject.reserve(subject.size() + range.size());
			subject.insert(subject.end(), range.begin(), range.end());
		}
		static std::vector<std::string> GetEachLine(std::string subject) {
			std::vector<std::string> ret;
			std::string tmp;
			for (char chr : subject)
				if (chr == '\n') {
					ret.push_back(tmp);
					tmp.clear();
				}
				else
					tmp += chr;
			return ret;
		}
		template<typename type> static bool Contains(std::vector<type> subject, type search) {
			if (std::find(subject.begin(), subject.end(), search) != subject.end())
				return true;
			return false;
		}
		static std::vector<std::string> split(std::string text, std::string sep) {
			std::vector<std::string> tokens;
			std::size_t start = 0, end = 0;
			while ((end = text.find(sep, start)) != std::string::npos) {
				tokens.push_back(text.substr(start, end - start));
				start = end + 1;
			}
			tokens.push_back(text.substr(start));
			return tokens;
		}
		static std::vector<std::string> MultiSplit(std::string str, std::string separators, bool KeepSeparators) {
			std::vector<std::string> ret;
			std::stringstream stringStream(str);
			std::string line;
			while (std::getline(stringStream, line)) {
				std::size_t prev = 0, pos;
				while ((pos = line.find_first_of(separators, prev)) != std::string::npos) {
					if (pos > prev) {
						ret.push_back(line.substr(prev, pos - prev));
						if (KeepSeparators) {
							std::stringstream ss;
							std::string s;
							ss << str[pos];
							ss >> s;
							ret.push_back(s);
						}
					}
					prev = pos + 1;
				}
				if (prev < line.length())
					ret.push_back(line.substr(prev, std::string::npos));
			}
			return ret;
		}
		static std::vector<std::string> EachItemBetweenDelims(std::string str, char Delim1, char Delim2, char separatedby = '\0') {
			std::vector<std::string> ret;
			std::istringstream iss_str(str);
			std::string skip, item;

			while (std::getline(std::getline(iss_str, skip, Delim1), item, Delim2)) {
				std::vector<std::string> v;
				std::istringstream iss_item(item);

				while (std::getline(iss_item, item, separatedby))
					v.push_back(item);
				for (auto i : v)
					ret.push_back(i);
			}

			return ret;
		}
		template<typename t> static int IndexOf(std::vector<t> vec, t item) {
			return std::find(vec.begin(), vec.end(), item) - vec.begin();
		}
		template<typename t> static void RemoveDuplicates(std::vector<t> &vec) {
			std::vector<t>::iterator itr = vec.begin();
			std::unordered_set<t> s;
			for (auto curr = vec.begin(); curr != vec.end(); ++curr)
				if (s.insert(*curr).second)
					*itr++ = *curr;
			vec.erase(itr, vec.end());
		}
	};
	class Map {
	public:
		static bool Contains(std::map<std::string, std::string> dico, std::string key) {
			std::map<std::string, std::string>::iterator it = dico.find(key);
			if (it != dico.end())
				return true;
			return false;
		}
		template<typename key, typename value>
		static std::vector<key> EachKey(std::map<key, value> m) {
			std::vector<key> ret;
			std::map<key, value>::iterator it;
			for (it = m.begin(); it != m.end(); ++it)
				ret.push_back(it->first);
			return ret;
		}
		template<typename key, typename value>
		static std::vector<value> EachValue(std::map<key, value> m) {
			std::vector<value> ret;
			std::map<key, value>::iterator it;
			for (it = m.begin(); it != m.end(); ++it)
				ret.push_back(it->second);
			return ret;
		}
		template<typename key, typename value>
		static value GetItem(std::map<key, value> m, key key) {
			return m[key];
		}
		template<typename key, typename value>
		static value GetValueByIndex(std::map<key, value> m, int index) {
			std::map<key, value>::iterator it = m.begin();
			for (int i = 1; i < index; ++i)
				++it;
			return it->second;
		}
		template<typename key, typename value>
		static key GetKeyByIndex(std::map<key, value> m, int index) {
			std::map<key, value>::iterator it = m.begin();
			for (int i = 1; i < index; ++i)
				++it;
			return it->first;
		}
		//template<typename key_t, typename value_t>
		//std::map<key_t, value_t> ConcatMap(std::map<key_t, value_t> map1, std::map<key_t, value_t> map2) {
		//	for (std::map<key_t, value_t>::const_iterator it = map2.begin(), it_end = map2.end(); it != it_end; ++it)
		//		map1.insert(*it);
		//	return map1;
		//}
	};
	class Text {
	private:
		static bool BothAreSpaces(char lhs, char rhs) {
			return (lhs == rhs) && (lhs == ' ');
		}
		static bool ArePair(char opening, char closing) {
			if (opening == '(' && closing == ')') return true;
			else if (opening == '{' && closing == '}') return true;
			else if (opening == '[' && closing == ']') return true;
			return false;
		}
	public:
		static const char newline = '\n';
		static const int space = 1;

		static std::string replace(std::string subject, std::string search, std::string replace) {
			size_t pos = 0;
			while ((pos = subject.find(search, pos)) != std::string::npos) {
				subject.replace(pos, search.length(), replace);
				pos += replace.length();
			}
			return subject;
		}
		static bool contains(std::string subject, std::string search) {
			if (subject.find(search) != std::string::npos)
				return true;
			return false;
		}
		static bool EndsWith(std::string subject, std::string ending) {
			if (subject.length() >= ending.length())
				return (0 == subject.compare(subject.length() - ending.length(), ending.length(), ending));
			return false;
		}
		static bool StartsWih(std::string subject, std::string starting) {
			return (subject.rfind(starting, 0) == 0);
		}
		static int occurrence(std::string subject, std::string search) {
			int occurrences = 0;
			std::string::size_type pos = 0;
			while ((pos = subject.find(search, pos)) != std::string::npos) {
				++occurrences;
				pos += search.length();
			}
			return occurrences;
		}
		static std::string RemoveFirstSpaces(std::string subject) {
			std::string ret;
			for (size_t i = 0; i < subject.length(); ++i)
				if (subject[i] == ' ' || subject[i] == '\t')
					continue;
				else {
					ret = subject.substr(i, subject.length());
					goto here;
				}
			here:
				return ret;
		}
		static std::string ReplaceMultiSpace(std::string subject) {
			std::string::iterator new_end = std::unique(subject.begin(), subject.end(), BothAreSpaces);
			subject.erase(new_end, subject.end());
			return subject;
		}
		static bool AreParanthesesBalanced(std::string exp) {
			std::stack<char> S;
			for (size_t i = 0; i<exp.length(); i++) {
				if (exp[i] == '(' || exp[i] == '{' || exp[i] == '[')
					S.push(exp[i]);
				else if (exp[i] == ')' || exp[i] == '}' || exp[i] == ']') {
					if (S.empty() || !ArePair(S.top(), exp[i]))
						return false;
					else
						S.pop();
				}
			}
			return S.empty() ? true : false;
		}
		static bool IsNumeric(std::string subject, std::string suffix = "", bool KeepDec = true) {
			subject = Text::replace(subject, " ", "");
			// decimal
			if (KeepDec) if (Text::occurrence(subject, ".") > 1 ||
				Text::EndsWith(subject, ".") ||
				Text::StartsWih(subject, ".")) return false;
			// positive / negative
			bool IsPosOrNeg = false;
			if (subject.find_first_not_of("+-") && !(Text::StartsWih(subject, "+") || Text::StartsWih(subject, "-"))) return false;
			else if (subject.find_first_not_of("+-")) IsPosOrNeg = true;
			// suffix
			if (!suffix.empty()) {
				for (char chr : suffix) if (Text::occurrence(subject, Text::CharToString(chr)) > 1 ||
					Text::contains(subject.substr(0, subject.size() - 1), Text::CharToString(chr))) return false;
			}
			else if (!std::isdigit(subject.back())) return false;
			if (!isdigit(subject.back())) subject.pop_back();
			if (IsPosOrNeg) subject.erase(0, 1);
			if (subject.empty()) return false;
			if (KeepDec) return (subject.find_first_not_of("0123456789.") == std::string::npos);
			else return (subject.find_first_not_of("0123456789") == std::string::npos);
		}
		static bool IsString(std::string subject) {
			return (Text::StartsWih(subject, "\"") && Text::EndsWith(subject, "\"") && Text::occurrence(subject, "\"") == 2);
		}
		static bool IsChar(std::string subject) {
			if ((StartsWih(subject, "'") && EndsWith(subject, "'")) && (subject.size() == 3 || subject[1] == '\\' && subject.size() == 4))
				return true;
			return false;
		}
		static std::vector<std::string> EachItemInDelimiters(std::string subject, char Delem1, char Delem2) {
			std::stack<std::string> st;
			std::vector<std::string> ret;
			for (char const c : subject) {
				if (c == Delem1) {
					st.push(std::string(&c, 1));
					continue;
				}
				st.top() += c;
				if (c == Delem2) {
					ret.push_back(st.top());
					st.pop();
				}
			}
			return ret;
		}
		static std::string CharToString(char chr) {
			std::stringstream ss;
			std::string s;
			ss << chr;
			ss >> s;
			return s;
		}
		static char StringToChar(std::string subject) {
			if (subject == "'\\n'") return '\n';
			if (subject == "'\\t'") return '\t';
			if (subject == "'\\v'") return '\v';
			if (subject == "'\\b'") return '\b';
			if (subject == "'\\r'") return '\r';
			if (subject == "'\\f'") return '\f';
			if (subject == "'\\a'") return '\a';
			if (subject == "'\\'") return '\\';
			if (subject == "'\\?'") return '\?';
			if (subject == "'\\''") return '\'';
			if (subject == "'\\\"'") return '\"';
			if (subject == "'\\0'") return '\0';
			return subject[1];
		}
		static inline void ltrim(std::string &s) {
			s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
				return !std::isspace(ch);
			}));
		}
		static inline void rtrim(std::string &s) {
			s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
				return !std::isspace(ch);
			}).base(), s.end());
		}
		static inline void trim(std::string &s) {
			ltrim(s);
			rtrim(s);
		}
		static bool AreFollowed(std::string subject, char Follower) {
			std::string Fol = CharToString(Follower) + CharToString(Follower);
			if (contains(subject, Fol))
				return true;
			return false;
		}
		static std::string GetLeftUntil(std::string subject, std::string until) {
			if (!contains(subject, until))
				return subject;
			return subject.substr(0, subject.find(until));
		}
		static std::string GetRightUntil(std::string subject, std::string until) {
			if (!contains(subject, until))
				return subject;
			return subject.substr(subject.find(until) + until.length(), subject.length());
		}
		static std::string ReplaceXByYIntoDelimiters(std::string line, std::string X, std::string Y, char Delim1, char Delim2, char separatedby = '\0') {
			std::vector<std::string> into = Vector::EachItemBetweenDelims(line, Delim1, Delim2, separatedby);
			std::string ret;
			for (std::string str : into)
				ret += replace(line, str, replace(str, X, Y));
			return ret;
		}
		static char *StringToCharArray(std::string subject) {
			char *ret = new char[subject.length() + 1];
			ret[subject.size()] = 0;
			memcpy(ret, subject.c_str(), subject.size());
			return ret;
		}
		static std::string CharArrayToString(char subject[]) {
			return subject;
		}
		static std::vector<std::string> GetEachWord(std::string subject) {
			std::vector<std::string> ret;
			std::istringstream iss(subject);
			std::string tmp;
			while (iss >> tmp)
				ret.push_back(tmp);
			return ret;
		}
		static std::vector<std::string> GetEachLine(std::string subject, bool KeepWhiteSpace = false) {
			std::vector<std::string> ret;
			std::istringstream f(subject);
			std::string line;
			if (KeepWhiteSpace)
				while (std::getline(f, line))
					ret.push_back(line);
			else
				while (std::getline(f, line))
					if (!System::Text::replace(line, " ", "").empty())
						ret.push_back(line);
			return ret;
		}
		static bool IsVirgin(std::string subject) {
			return System::Text::RemoveFirstSpaces(subject).empty();
		}
		static std::string DeleteUnnecessaryZeros(std::string subject) {
			for (std::string::size_type s = subject.length() - 1; s > 0; --s) {
				if (subject[s] == '0') subject.erase(s, 1);
				else break;
			}
			if (EndsWith(subject, "."))
				return subject.substr(0, subject.length() - 1);
			return subject;
		}
		static bool IsNothing(std::string subject) {
			return IsVirgin(subject);
		}
		static bool ContainsSpecialChar(std::string subject, std::string without = "") {
			return (subject.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ" + without) != std::string::npos);
		}
		static bool ContainsCharWithout(std::string subject, std::string without = "") {
			return (subject.find_first_not_of(without) != std::string::npos);
		}
		static std::string VectorToString(std::vector<std::string> vec, bool KeepCRLF = true) {
			std::string ret;
			for (std::string line : vec)
				if (KeepCRLF) ret += line + "\n";
				else ret += line;
				return ret;
		}
	};
	class Display {
	public:
		enum color {
			Black,
			Blue,
			Green,
			Cyan,
			Red,
			Magenta,
			Brown,
			Normal,
			Grey,
			LightBlue,
			LightGreen,
			LightCyan,
			LightRed,
			LightMagenta,
			Yellow,
			White
		};
		static void ColoredMessage(std::string text, color color = color::Normal) {

			rlutil::setColor(color);
			std::cout << text;
			rlutil::setColor(color::Normal);
		}
		static void ErrorMessage(std::string text, int line = -1, int chr_pos = -1) {
			if (System::Text::EndsWith(text, "."))
				text = text.substr(0, text.size() - 1);
			if (line >= 0)
				if (chr_pos >= 0) ColoredMessage("ERROR [" + std::to_string(line) + ":" + std::to_string(chr_pos) + "]: " + text + "." + "\n", color::Yellow);
				else ColoredMessage("ERROR [" + std::to_string(line) + "]: " + text + "." + "\n", color::Yellow);
			else ColoredMessage("ERROR: " + text + "." + "\n", color::Yellow);
		}
	public:
		static void UnknownToken(std::string token, int line = -1) {
			ErrorMessage("Unknown token : [" + token + "]", line);
		}
		static void FileDoesNotExit(std::string filename) {
			Display::ErrorMessage("File given doesn't exist : [" + filename + "]");
		}
	public:
		static void xtime(std::string str, int time) {
			for (int i = 0; i < time; ++i)
				std::cout << str;
			std::cout << std::endl;
		}
		static void StartProgram(std::string AppName,
			std::string by,
			float AppVersion = 00.00,
			std::string MiniDescription = "") {
			if (_WIN32) system("cls"); else system("clear");
			std::cout << AppName << ": " << MiniDescription << ", version [" << AppVersion << "], by " << by << std::endl << std::endl;
		}
		static void ExitProgram(int time = 54) {
			std::cout << std::endl;
			Display::xtime("_", time);
			std::cout << std::endl << std::endl;
		}
		static void LogInfo(std::string info, color color = color::White) {
			ColoredMessage("> " + info + "\n", color);
		}
	};
	class cpp {
	public:
		template<int index>
		class ArgumentManager {
		public:
			static std::string get(char *argv[], bool ShowErr = true, std::string ErrMsg = "An expected argument has not been given in program entry.") {
				if (argv[index] != NULL)
					return argv[index];
				if (ShowErr) System::Display::ErrorMessage(ErrMsg);
				return "";
			}
		};
		static void clear() {
			if (_WIN32) system("cls"); else system("clear");
		}
		enum OS {
			Windows,
			Unix,
			Linux,
			Apple
		};
		static OS GetOS() {
#ifdef _WIN32
			return OS::Windows;
#elif __unix__ 
			return OS::Unix;
#elif __linux__
			return OS::Linux;
#elif __APPLE__
			return OS::Apple;
#endif
		}
	};
	class File {
	public:
		static double GetFileSize(std::string filename) {
			struct stat stat_buf;
			int rc = stat(filename.c_str(), &stat_buf);
			return rc == 0 ? stat_buf.st_size : -1;
		}
		static bool exist(std::string fileName) {
			std::ifstream infile(fileName);
			return infile.good();
		}
		static std::string WithoutExtention(std::string filename) {
			size_t lastindex = filename.find_last_of(".");
			return filename.substr(0, lastindex);
		}
		static std::vector<std::string> GetEachLine(std::string filename) {
			std::vector<std::string> ret;
			std::ifstream in(filename.c_str());
			std::string str;
			while (std::getline(in, str))
				if (str.size() > 0)
					ret.push_back(str);
			in.close();
			return ret;
		}
		static std::string GetLine(int index, std::string filename) {
			std::ifstream file(filename);
			std::string ret;
			for (int i = 0; i < index; ++i) std::getline(file, ret);
			return ret;
		}
		static void write(std::string filename, std::string text) {
			std::ofstream file;
			file.open(filename);
			file << text;
			file.close();
		}
		static void WriteAppend(std::string filename, std::string text) {
			std::ofstream file;
			file.open(filename, std::ios_base::app);
			file << text;
			file.close();
		}
		static std::string GetExtention(std::string filename) {
			if (System::Text::contains(filename, "."))
				return System::Text::GetRightUntil(filename, ".");
			return "";
		}
		static std::string GetAllText(std::string filename, bool KeepCRLF = true) {
			std::string buff;
			if (KeepCRLF)
				for (std::string line : GetEachLine(filename))
					buff += line;
			else
				for (std::string line : GetEachLine(filename))
					buff += line + '\n';
			return buff;
		}
	};
	class Math {
	private:
		static void changeCharacters(std::string chars, std::string toChange, std::string &str) {
			int i = -1;
			std::for_each(str.begin(), str.end(), [chars, toChange, &i, &str](char c) {
				i++;
				for (auto &j : chars) {
					if (c == j) {
						str.erase(str.begin() + i);
						str.insert(i, toChange);
						break;
					}
				}
			});
		}
		class MathSolver {
		private:
			std::string exp;
			std::vector<double> nums;
			double result;
			std::function<bool(char)> isPlusMinus = [](char c) {return c == '+' || c == '-'; };
			std::function<bool(char)> isMultDiv = [](char c) {return c == '*' || c == '/'; };
		public:
			MathSolver(std::string exp) {
				exp.erase(std::remove(exp.begin(), exp.end(), ' '), exp.end());
				if (exp.find_first_of("+-") != 0)
					exp.insert(0, "+");
				for (size_t i = 0; i < exp.length(); i++) {
					if (isPlusMinus(exp[i]))
						exp.insert(i + 1, "1*");
				}
				changeCharacters("[{", "(", exp);
				changeCharacters("]}", ")", exp);
				changeCharacters(":", "/", exp);
				this->exp = exp;
				this->processBrackets();
				this->parse();
			}
			void countBracks(std::vector< std::pair<int, int>> &bracks) {
				int parOC = 0;
				for (size_t i = 0; i < exp.length(); i++) {
					if (exp[i] == '(') {
						if (parOC == 0)
							bracks.push_back(std::make_pair(i, 0));
						parOC++;
					}
					else if (exp[i] == ')') {
						parOC--;
						if (parOC == 0)
							bracks[bracks.size() - 1].second = i;
					}
				}
			}
			void processBrackets() {
				std::vector< std::pair<int, int>> bracks;
				countBracks(bracks);
				int count = bracks.size();
				for (int i = 0; i < count; i++) {
					std::pair<int, int> j = bracks[0];
					MathSolver solve(exp.substr(j.first + 1, j.second - 1 - j.first));
					double res = solve.getResult();
					std::stringstream ss;
					ss << res;
					exp.erase(exp.begin() + j.first, exp.begin() + j.second + 1);
					exp.insert(j.first, ss.str());

					bracks.clear();
					countBracks(bracks);
				}
			}
			void parse() {
				std::function<void(double&, std::istringstream&)> searchPow = [](double &num, std::istringstream &iss) {
					if (iss.peek() == '^') {
						char tmp2;
						double tmp3;
						iss >> tmp2 >> tmp3;
						num = pow(num, tmp3);

					}
					if (iss.peek() == '%') {
						char tmp2;
						double tmp3;
						iss >> tmp2 >> tmp3;
						num = (static_cast<int>(num) % static_cast<int>(tmp3));
					}
				};
				double num;
				char tmp;
				std::istringstream iss(exp);
				while ((int)iss.tellg() != EOF) {
					if (isPlusMinus(iss.peek()) && isdigit(exp[(int)iss.tellg() + 1])) {
						iss >> num;
						searchPow(num, iss);
						nums.push_back(num);
					}
					else if (isMultDiv(iss.peek()) && (isdigit(exp[(int)iss.tellg() + 1]) || isdigit(exp[(int)iss.tellg() + 2]))) {
						iss >> tmp >> num;
						searchPow(num, iss);
						nums.push_back(num);

						if (tmp == '/')
							nums[nums.size() - 1] = 1 / nums[nums.size() - 1];
						nums[nums.size() - 1] *= nums[nums.size() - 2];
						nums[nums.size() - 2] = 0;
					}
				}
				nums.erase(remove(nums.begin(), nums.end(), 0), nums.end());

				for (auto i : nums)
					result += i;
			}
			double getResult() {
				return result;
			}
		};
	public:
		static bool IsMathSymbol(std::string symb) {
			return
				(
					symb == "+" ||
					symb == "-" ||
					symb == "*" ||
					symb == "/" ||
					symb == "%" ||
					symb == "++" ||
					symb == "--"
					);
		}
		static bool IsMathExpression(std::string exp) {
			for (size_t i = 0; i < exp.length(); ++i) {
				if (i % 2 == 0 && i > 0)
					if (exp[i] != '+' || exp[i] != '-' || exp[i] != '*' || exp[i] != '/' || exp[i] != '%') return false;
				if (i % 2 != 0)
					std::cout << exp[i];//if (!std::isdigit(exp[i]) || !std::isalnum(exp[i])) return false;
			}
			return true;
		}
		static double evaluate(std::string exp) {
			MathSolver MS(exp);
			return MS.getResult();
		}
		static std::string ToHex(std::string number) {
			return "A revoir -> hex!";
		}
	};

#ifdef _WIN32
#include <windows.h>
#pragma comment(lib,"urlmon.lib")
	class Web {
	public:
		static void Download(std::string url, std::string path) {
			URLDownloadToFile(0, System::Text::StringToCharArray(url), System::Text::StringToCharArray(path), 0, 0);
		}
		static void OpenPage(std::string url) {
			ShellExecute(0, 0, System::Text::StringToCharArray(url), 0, 0, SW_SHOWNORMAL);
		}
	};
#endif

}