#pragma once
#include <algorithm>
#include <iostream>
#include <istream>
#include <ostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <exception>
#include "../genmath/ObjectBase.h"
#include <numeric>
#include "../cli/ConsoleUI.h"

// suggested improvement: exclusive file access through only object
// forward additive file mangement, middle insertion is not solved due to REAL TIME file write
//  without rare memory buffer maintenance
// line specific io management
//  LINEAR FILE OPERATIONS without metadata
//  suggestion: logarithmic data management with metadata

// semi bidirectional file manager (writings are forward specific only)
class FileControl {

public:
	FileControl();

	/// <summary>
	/// file control
	/// </summary>
	/// <param name="file_source">source of the file (file path and file name)</param>
	/// <param name="mode">file management mode (see global access attributes)</param>
	/// <param name="skip_empty_lines">skipping empty lines in the source</param>
	FileControl(ConsoleUI& program_console, std::string file_source, uint8_t mode = 0, 
		bool skip_empty_lines = true);

	FileControl(const FileControl& orig);

	FileControl& operator=(const FileControl& orig);

	virtual ~FileControl();

	/// <summary>
	/// returns the source file
	/// </summary>
	/// <returns>string value of source file</returns>
	std::string GetSource() const;

	/// <summary>
	/// closes file source
	/// </summary>
	void CloseSource();

	/// <summary>
	/// returns the open mode of file
	/// </summary>
	/// <returns>integer value of open mode (class specific, not STL)</returns>
	uint8_t GetMode() const;

	/// <summary>
	/// changing file source
	/// </summary>
	/// <param name="source">source of the file (file path and file name)</param>
	/// <param name="mode">file management mode (see global access attributes)</param>
	/// <param name="skip_empty_lines">skipping empty lines in the source</param>
	void ChangeSource(std::string source, uint8_t mode, bool skip_empty_lines = true);

	/// <summary>
	/// seeking for given line in the file character by character, returns last line index 
	/// in case of out of bounds - index overflow, HIGH OPERATION COST
	/// </summary>
	/// <param name="no_line">the index of desired line from 0</param>
	void SeekLine(size_t no_line);

	/// <summary>
	/// seeking to end of file, HIGH OPERATION COST
	/// </summary>
	void SeekEnd();

	/// <summary>
	/// returns the current position of cursor line by line
	/// </summary>
	/// <returns>returns the line index where the cursor stands at currently</returns>
	size_t SeekLineInfo() const;

	/// <summary>
	/// finds (forward iteration from recent line position) the first occurrence of line containing 
	/// the given literal, overwrites its content toward eof
	/// </summary>
	/// <param name="object">the given literal for search</param>
	/// <returns>returns the cursor position (beginning of the line) of the found line containing
	/// the given object for the first time, returns eof if no line found</returns>
	size_t FindLineContains(const std::string object);

	/// <summary>
	/// finds (forward iteration from the recent line position) the first occurrence of line 
	/// containing the given object, overwrites its content toward eof
	/// </summary>
	/// <param name="object">the given object for search</param>
	/// <returns>returns the cursor position (beginning of the line) of the found line containing
	/// the given object for the first time, returns eof if no line found</returns>
	size_t FindLineContains(const genmath::ObjectBase* object);

	/// <summary>
	/// finds (forward iteration from the recent line position) all occurrence of given object 
	/// among lines, overwrites the last occurrence toward eof
	/// </summary>
	/// <param name="object">the given object for search</param>
	/// <returns>returns the cursor positions (beginning of the line) of the found lines containing
	/// the given object, returns eof if no line found</returns>
	std::vector<size_t> FindLinesContains(const std::string object);

	/// <summary>
	/// finds (forward iteration from the recent line position) all occurrence of given object 
	/// among lines, overwrites the last occurrence toward eof
	/// </summary>
	/// <param name="object">the given object for search</param>
	/// <returns>returns the cursor positions (beginnig of the line) of the found lines containing
	/// the given object, returns eof if no line found</returns>
	std::vector<size_t> FindLinesContains(const genmath::ObjectBase* object);

	/// <summary>
	/// reading undetermined amount of source lines only one at a time with implicit conversion
	/// </summary>
	/// <param name="object">pointer of the object to be assigned</param>
	/// <returns>bool flag about reading operation (true is the error status)</returns>
	bool ReadLineUndefined(std::string& object);

	/// <summary>
	/// reading undetermined amount of source lines only one at a time with implicit conversion
	/// </summary>
	/// <param name="object">pointer of the object to be assigned</param>
	/// <returns>bool flag about reading operation (true is the error status)</returns>
	bool ReadLineUndefined(genmath::ObjectBase* object);
	// via implicit conversion to object data (dynamic linkage to a vector)

	/// <summary>
	/// reading determined amount of multiple source lines at a time with implicit conversions,
	/// if not enough lines to be read, throws exception
	/// </summary>
	/// <param name="objects">container of referred objects to be assigned</param>
	void ReadLinesDefined(std::vector<std::string>& objects);

	/// <summary>
	/// reading determined amount of multiple source lines at a time with implicit conversions,
	/// if not enough lines to be read, throws exception
	/// </summary>
	/// <param name="objects">container of referred objects to be assigned</param>
	void ReadLinesDefined(std::vector<genmath::ObjectBase*>& objects);

	/// <summary>
	/// reading undetermined amount of source lines from the end of file toward the first line,
	/// only one at a time with implicit conversion, SLOWER THAN FORWARD READ
	/// </summary>
	/// <param name="object">pointer of the object to be assigned</param>
	/// <returns>bool flag about reading operation (true is the erro status)</returns>
	bool ReadLineUndefinedBackward(std::string& object);

	/// <summary>
	/// reading undetermined amount of source lines from the end of file toward the first line,
	/// only one at a time with implicit conversion, SLOWER THAN FORWARD READ
	/// </summary>
	/// <param name="object">pointer of the object to be assigned</param>
	/// <returns>bool flag about reading operation (true is the erro status)</returns>
	bool ReadLineUndefinedBackward(genmath::ObjectBase* object);

	/// <summary>
	/// reading determined amoutn of multiple source lines at a time, starting from the end of file
	///  toward the first line, with impliciti conversion, SLOWER THAN FORWARD READ
	/// </summary>
	/// <param name="objects">container of referred objects to be assigned</param>
	void ReadLinesDefinedBackward(std::vector<std::string>& objects);

	/// <summary>
	/// reading determined amoutn of multiple source lines at a time, starting from the end of file
	///  toward the first line, with impliciti conversion, SLOWER THAN FORWARD READ
	/// </summary>
	/// <param name="objects">container of referred objects to be assigned</param>
	void ReadLinesDefinedBackward(std::vector<genmath::ObjectBase*>& objects);

	/// <summary>
	/// writing undefined number of lines into the source, one at a time via implicit conversion
	/// </summary>
	/// <param name="object">reference to the object to write its content</param>
	void WriteLineUndefined(genmath::ObjectBase* object);

	/// <summary>
	/// writing undefined number of lines into source, one at a time via raw literal source
	/// </summary>
	/// <param name="object"></param>
	void WriteLineUndefined(std::string object);

	/// <summary>
	/// writing defined number of lines into the source, multiple items at a time
	/// </summary>
	/// <param name="objects">reference container to the objects to write their contents</param>
	void WriteLinesDefined(std::vector<genmath::ObjectBase*>& objects);

	/// <summary>
	/// inserts a line from a given line position, HIGH OPERATION COST
	/// </summary>
	/// <param name="from">inserting from this position</param>
	/// <param name="object">insert this content</param>
	void InsertLineUndefined(size_t from, std::string object);

	/// <summary>
	/// inserts a line from a given line position, HIGH OPERATION COST
	/// </summary>
	/// <param name="from">inserting from this position</param>
	/// <param name="object">insert this content</param>
	void InsertLineUndefined(size_t from, genmath::ObjectBase* object);

	/// <summary>
	/// inserts lines from desired line position, HIGH OPERATION COST
	/// </summary>
	/// <param name="from">insert lines from this line position</param>
	/// <param name="objects">insert the following contents</param>
	void InsertLinesDefined(size_t from, std::vector<genmath::ObjectBase*>& objects);

	/// <summary>
	/// erases a single line from a given position, HIGH OPERATION COST
	/// </summary>
	/// <param name="from">the line index to execure the erase from</param>
	void EraseLine(size_t from);

	/// <summary>
	/// erases lines from a given position, HIGH OPERATION COST
	/// </summary>
	/// <param name="from">desired position where the deletion startswithin the source</param>
	/// <param name="num_of_lines">number of lines to be erased</param>
	void EraseLines(size_t from, size_t num_of_lines);

	bool skip_empty_lines_;

	// file access flags
	/// read-write
	static const uint8_t RW = 0;
	/// read-write-truncate
	static const uint8_t RWT = 1;
	/// read
	static const uint8_t R = 2;
	/// write
	static const uint8_t W = 3;
	/// write-truncate
	static const uint8_t WT = 4;

	static constexpr size_t no_line = std::string::npos;
private:

	void CleanUp();

	ConsoleUI program_console_;

	std::string source_;
	uint8_t mode_;
	std::fstream file_desc_;
	volatile bool initialized_;

	std::stringstream suffix_buffer_;
	bool erase_used_;
	size_t recent_line_index_;
};
