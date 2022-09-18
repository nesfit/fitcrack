#ifndef SERVER_INPUT_DICT_H
#define SERVER_INPUT_DICT_H

#include "Dictionary.h"

class InputDict
{
public:
	InputDict(const std::string path);
	virtual ~InputDict() = default;
	/**
	 * @brief Writes @p passCount passwords to file at @p dst
	 * 
	 * @param passCount The number of passwords to write
	 * @param dst The file to write the passwords to
	 * @return Number of successfully written passwords. May be less than @p passCount
	 *   due to no more passwords being available in this dictionary
	 * @throws InputDict::Exception in case of a hard error
	 */
	virtual uint64_t WritePasswordsTo(uint64_t passCount, const std::string &dst);
	virtual void CopyTo(const std::string &dst);
	virtual std::ifstream::pos_type GetCurrentDictPos();
	virtual void SetCurrentDictPos(std::ifstream::pos_type pos);
	class Exception: public std::runtime_error
	{
		using std::runtime_error::runtime_error;
	};
private:
	std::ifstream m_file;
};

class InputDictBenchmark: public InputDict
{
public:
	using InputDict::InputDict;
	virtual uint64_t WritePasswordsTo(uint64_t passCount, const std::string &dst) override {std::ofstream{dst}; return passCount;}
	virtual void CopyTo(const std::string &dst) override {std::ofstream{dst};}
	virtual void SetCurrentDictPos(std::ifstream::pos_type pos) { (void)pos; }
};

#endif //SERVER_INPUT_DICT_H
