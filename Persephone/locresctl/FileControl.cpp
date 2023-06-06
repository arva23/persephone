#include "FileControl.h"


FileControl::FileControl() {

	source_ = "";
	mode_ = 0;
	skip_empty_lines_ = true;
	file_desc_ = std::fstream();
	initialized_ = false;
	suffix_buffer_ = std::stringstream();
	erase_used_ = false;
	recent_line_index_ = 0;
	program_console_ = ConsoleUI();
}

FileControl::FileControl(ConsoleUI& program_console, std::string file_source, uint8_t mode,
	bool skip_empty_lines) {
	
	// modes
	//  0 : read - write - append
	//  1 : read - write - overwrite
	//  2 : read
	//  3 : write - append
	//  4 : write - overwrite
	file_desc_ = std::fstream();

	switch (mode) {

	case 0:
		file_desc_.open(file_source, std::ios::in | std::ios::out);
		if (file_desc_.fail()) throw std::exception("Error at file open (r-w) (FileControl).");
		break;
	case 1:
		file_desc_.open(file_source, std::ios::in | std::ios::out | std::ios::trunc);
		if (file_desc_.fail()) throw std::exception("Error at file open (r-w-trunc) (FileControl).");
		break;
	case 2:
		file_desc_.open(file_source, std::ios::in);
		if (file_desc_.fail()) throw std::exception("Error at file open (r) (FileControl).");
		break;
	case 3:
		file_desc_.open(file_source, std::ios::out);
		if (file_desc_.fail()) throw std::exception("Error at file open (w) (FileControl).");
		break;
	case 4:
		file_desc_.open(file_source, std::ios::out | std::ios::trunc);
		if (file_desc_.fail()) throw std::exception("Error at file open (w-trunc) (FileControl).");
		break;
	default: throw std::exception("Error at file open (unidentified opening mode) (FileControl).");
	}

	source_ = file_source;
	mode_ = mode;
	skip_empty_lines_ = skip_empty_lines;
	initialized_ = true;
	suffix_buffer_ = std::stringstream();
	erase_used_ = false;
	recent_line_index_ = 0;
	program_console_ = program_console;
}

FileControl::FileControl(const FileControl& orig) {

	this->ChangeSource(orig.source_, orig.mode_, orig.skip_empty_lines_);
	this->SeekLine(orig.SeekLineInfo());
}

FileControl& FileControl::operator=(const FileControl& orig) {

	this->ChangeSource(orig.source_, orig.mode_, orig.skip_empty_lines_);
	this->SeekLine(orig.SeekLineInfo());

	return *this;
}

FileControl::~FileControl() {

	// correction due to use of deletion function and overwrites
	if (erase_used_)
		this->CleanUp();
}

std::string FileControl::GetSource() const {

	return source_;
}

uint8_t FileControl::GetMode() const {

	return mode_;
}

void FileControl::ChangeSource(std::string source, uint8_t mode, bool skip_empty_lines) {

	if (initialized_) [[likely]]
		file_desc_ = std::fstream();

	switch (mode) {

	case 0:
		file_desc_.open(source, std::ios::in | std::ios::out);
		if (file_desc_.fail()) throw std::exception("Error at file open (r-w) (FileControl).");
		break;
	case 1:
		file_desc_.open(source, std::ios::in | std::ios::out | std::ios::trunc);
		if (file_desc_.fail()) throw std::exception("Error at file open (r-w-trunc) (FileControl).");
		break;
	case 2:
		file_desc_.open(source, std::ios::in);
		if (file_desc_.fail()) throw std::exception("Error at file open (r) (FileControl).");
		break;
	case 3:
		file_desc_.open(source, std::ios::out);
		if (file_desc_.fail()) throw std::exception("Error at file open (w) (FileControl).");
		break;
	case 4:
		file_desc_.open(source, std::ios::out | std::ios::trunc);
		if (file_desc_.fail()) throw std::exception("Error at file open (w-trunc) (FileControl).");
		break;
	default: throw std::exception("Error at file open (unidentified opening mode) (FileControl).");
	}

	source_ = source;
	mode_ = mode;
	skip_empty_lines_ = skip_empty_lines;
	initialized_ = true;
	suffix_buffer_.clear();

	// managing unused, removed lines
	this->CleanUp();
	erase_used_ = false;
}

void FileControl::CloseSource() {

	if (initialized_) {

		file_desc_.close();
		initialized_ = false;
	}
	else {

		throw std::exception("No opened source file (FileControl).");
	}
}

void FileControl::SeekLine(size_t no_line) {

	recent_line_index_ = 0;

	while (!file_desc_.eof() && recent_line_index_ < no_line) {

		file_desc_.ignore(std::numeric_limits<size_t>::max(), '\n');
		++recent_line_index_;
	}

	if (file_desc_.eof())
		throw std::exception("Line seek out of bounds (FileControl).");
}

void FileControl::SeekEnd() {

	file_desc_.seekp(0, std::ios_base::beg);
	recent_line_index_ = 0;

	while (file_desc_.eof()) {

		file_desc_.ignore(std::numeric_limits<size_t>::max(), '\n');
		++recent_line_index_;
	}
}

size_t FileControl::SeekLineInfo() const {

	return recent_line_index_;
}

size_t FileControl::FindLineContains(const std::string object) {

	std::string buffer;
	uint64_t ret_pos = 0;

	while (!file_desc_.eof()) {

		std::getline(file_desc_, buffer, '\n');

		if ((ret_pos = buffer.find(object)) != std::string::npos) [[likely]]
			return ret_pos;

		++recent_line_index_;
	}

	return std::string::npos;
}

size_t FileControl::FindLineContains(const genmath::ObjectBase* object) {

	std::string buffer;
	uint64_t ret_pos = 0;

	while (!file_desc_.eof()) {

		std::getline(file_desc_, buffer, '\n');

		if ((ret_pos = buffer.find(*object)) != std::string::npos) [[likely]]
			return ret_pos;

		++recent_line_index_;
	}

	return std::string::npos;
}

std::vector<size_t> FileControl::FindLinesContains(const std::string object) {

	std::string buffer;
	std::vector<size_t> ret_poss;
	uint64_t ret_pos = 0;

	while (!file_desc_.eof()) {

		std::getline(file_desc_, buffer, '\n');

		if ((ret_pos = buffer.find(object)) != std::string::npos) [[likely]]
			ret_poss.push_back(ret_pos);

		++recent_line_index_;
	}

	return ret_poss;
}

std::vector<size_t> FileControl::FindLinesContains(const genmath::ObjectBase* object) {

	std::string buffer;
	std::vector<size_t> ret_poss;
	uint64_t ret_pos = 0;

	while (!file_desc_.eof()) {

		std::getline(file_desc_, buffer, '\n');

		if ((ret_pos = buffer.find(std::string(*object))) != std::string::npos) [[likely]]
			ret_poss.push_back(ret_pos);

		++recent_line_index_;
	}

	return ret_poss;
}

bool FileControl::ReadLineUndefined(std::string& object) {

	if (initialized_) [[likely]] {

		if (file_desc_.eof()) [[unlikely]] {

			program_console_.Println("Reached end of file (eof() in undefined read) (FileControl).");
			return true;
		}

		if (skip_empty_lines_) [[likely]] {

			while (object.empty()) {

				if (file_desc_.eof()) [[unlikely]] {

					file_desc_.seekp(0);
					return true;
				}

				std::getline(file_desc_, object);
				++recent_line_index_;
			}
		}
	}
	else {

		throw std::exception("Uninitialized file object (FileControl).");
	}

	return false;
}

bool FileControl::ReadLineUndefined(genmath::ObjectBase* object) {

	if (initialized_) [[likely]] {

		std::string raw_data;

		if (file_desc_.eof()) [[unlikely]] {

			program_console_.Println("Reached end of file (eof() in undefined read) (FileControl).");
			return true;
		}

		if (skip_empty_lines_) [[likely]] {

			while (raw_data.empty()) {

				if (file_desc_.eof()) [[unlikely]] {

					file_desc_.seekp(0);
					return true;
				}

				std::getline(file_desc_, raw_data);
				++recent_line_index_;
			}
		}

		if (*object = raw_data) [[unlikely]]
			throw std::exception("Conversion error at data assignment to object (FileControl).");
	}
	else {

		throw std::exception("Uninitialized file object (FileControl).");
	}

	return false;
}

void FileControl::ReadLinesDefined(std::vector<std::string>& objects) {

	if (initialized_) [[likely]] {

		std::for_each(objects.begin(), objects.end(), [this](std::string& curr_obj) {


			if (file_desc_.eof()) [[unlikely]]
				throw std::exception("Reached end of file (eof() in definite multiple read) (FileControl).");

			if (skip_empty_lines_) [[likely]] {

				while (curr_obj.empty()) {

					if (file_desc_.eof()) [[unlikely]]
						throw std::exception("Reached end of file (eof() in definite multiple read) (FileControl).");

					std::getline(file_desc_, curr_obj);
					++recent_line_index_;
				}
			}
		});
	}
	else {

		throw std::exception("Uninitialized file object (FileControl).");
	}
}

void FileControl::ReadLinesDefined(std::vector<genmath::ObjectBase*>& objects) {

	if (initialized_) [[likely]] {

		std::string raw_data;

		std::for_each(objects.begin(), objects.end(), [this, &raw_data](genmath::ObjectBase* curr_obj) {


			if (file_desc_.eof()) [[unlikely]]
				throw std::exception("Reached end of file (eof() in definite multiple read) (FileControl).");

			if (skip_empty_lines_) [[likely]] {

				while (raw_data.empty()) {

					if (file_desc_.eof()) [[unlikely]]
						throw std::exception("Reached end of file (eof() in definite multiple read) (FileControl).");

					std::getline(file_desc_, raw_data);
					++recent_line_index_;
				}
			}

			if (*curr_obj = raw_data) [[unlikely]]
				throw std::exception("Conversion error at data assignment to object (FileControl).");
		});
	}
	else {

		throw std::exception("Uninitialized file object (FileControl).");
	}
}

bool FileControl::ReadLineUndefinedBackward(std::string& object) {

	if (initialized_) [[likely]] {

		if (!file_desc_.tellp()) [[unlikely]] {

			program_console_.Println("Reached beginning of file in undefined read) (FileControl).");
			return true;
		}

		std::streampos ch_off = 1;
		char ch = ' ';

		// bacward reading manually
		// ignoring recent line ending
		file_desc_.seekp(file_desc_.tellp() - ch_off);

		if (skip_empty_lines_) [[likely]] {

			ch_off = 2;
			while (ch == '\n') {

				file_desc_.seekp(file_desc_.tellp() - ch_off);

				if (file_desc_.fail()) [[unlikely]]
					throw std::exception("Reached beginning of file (in defined multiple read) (FileControl).");

				file_desc_.get(ch);

				--recent_line_index_;
			}
		}

		// reading line
		// seeking to the after the last character
		ch_off = 1;
		file_desc_.seekp(file_desc_.tellp() + ch_off);
		ch = 2;

		do {

			file_desc_.seekp(file_desc_.tellp() - ch_off);

			if (file_desc_.fail()) [[unlikely]]
				throw std::exception("Reached beginning of file (in defined read) (FileControl).");

			file_desc_.get(ch);
			object.push_back(ch);
			std::reverse(object.begin(), object.end());

			--recent_line_index_;
		} while (ch != '\n');
	}
	else {

		throw std::exception("Uninitialized file object (FileControl).");
	}

	return false;
}

bool FileControl::ReadLineUndefinedBackward(genmath::ObjectBase* object) {

	if (initialized_) [[likely]] {

		std::string raw_data;

		if (!file_desc_.tellp()) [[unlikely]] {

			program_console_.Println("Reached beginning of file in undefined read) (FileControl).");
			return true;
		}

		std::streampos ch_off = 1;
		char ch = ' ';

		// bacward reading manually
		// ignoring recent line ending
		file_desc_.seekp(file_desc_.tellp() - ch_off);

		if (skip_empty_lines_) [[likely]] {

			ch_off = 2;
			while (ch == '\n') {

				file_desc_.seekp(file_desc_.tellp() - ch_off);

				if (file_desc_.fail()) [[unlikely]]
					throw std::exception("Reached beginning of file (in defined multiple read) (FileControl).");

				file_desc_.get(ch);

				--recent_line_index_;
			}
		}

		// reading line
		// seeking to the after the last character
		ch_off = 1;
		file_desc_.seekp(file_desc_.tellp() + ch_off);
		ch = 2;

		do {

			file_desc_.seekp(file_desc_.tellp() - ch_off);

			if (file_desc_.fail()) [[unlikely]]
				throw std::exception("Reached beginning of file (in defined read) (FileControl).");

			file_desc_.get(ch);
			raw_data.push_back(ch);
			std::reverse(raw_data.begin(), raw_data.end());

			--recent_line_index_;
		} while (ch != '\n');

		if (*object = raw_data) [[unlikely]]
			throw std::exception("Conversion error at data assignment to object (FileControl).");
	}
	else {

		throw std::exception("Uninitialized file object (FileControl).");
	}

	return false;
}

void FileControl::ReadLinesDefinedBackward(std::vector<std::string>& objects) {

	if (initialized_) [[likely]] {

		std::string raw_data;

		std::for_each(objects.begin(), objects.end(), [this, &raw_data](std::string& curr_obj) {

			if (!file_desc_.tellp()) [[unlikely]]
				throw std::exception("Reached beginning of file (in defined multiple read) (FileControl).");

			std::streampos ch_off = 1;
			char ch = ' ';

			// backward reading manually
			// ignoring recent line ending
			file_desc_.seekp(file_desc_.tellp() - ch_off);

			if (skip_empty_lines_) [[likely]] {

				ch_off = 2;
				raw_data.clear();

				while (ch == '\n') {

					file_desc_.seekp(file_desc_.tellp() - ch_off);

					if (file_desc_.fail()) [[unlikely]]
						throw std::exception("Reached beginning of file (in defined multiple read) (FileControl).");

					file_desc_.get(ch);

					--recent_line_index_;
				}
			}

			// reading line
			// seeking to after the last character
			ch_off = 1;
			file_desc_.seekp(file_desc_.tellp() + ch_off);
			ch_off = 2;

			do {

				file_desc_.seekp(file_desc_.tellp() - ch_off);

				if (file_desc_.fail()) [[unlikely]]
					throw std::exception("Reached beginning of file (in defined multiple read) (FileControl).");

				file_desc_.get(ch);
				raw_data.push_back(ch);
				std::reverse(raw_data.begin(), raw_data.end());

				--recent_line_index_;

			} while (ch != '\n');

			curr_obj = raw_data;
			});
	}
	else {

		throw std::exception("Uninitialized file object (FileControl).");
	}
}

void FileControl::ReadLinesDefinedBackward(std::vector<genmath::ObjectBase*>& objects) {

	if (initialized_) [[likely]] {

		std::string raw_data;

		std::for_each(objects.begin(), objects.end(), [this, &raw_data](genmath::ObjectBase* curr_obj) {

			if (!file_desc_.tellp()) [[unlikely]]
				throw std::exception("Reached beginning of file (in defined multiple read) (FileControl).");

			std::streampos ch_off = 1;
			char ch = ' ';

			// backward reading manually
			// ignoring recent line ending
			file_desc_.seekp(file_desc_.tellp() - ch_off);

			if (skip_empty_lines_) [[likely]] {

				ch_off = 2;
				raw_data.clear();

				while (ch == '\n') {

					file_desc_.seekp(file_desc_.tellp() - ch_off);

					if (file_desc_.fail()) [[unlikely]]
						throw std::exception("Reached beginning of file (in defined multiple read) (FileControl).");

					file_desc_.get(ch);

					--recent_line_index_;
				}
			}

			// reading line
			// seeking to after the last character
			ch_off = 1;
			file_desc_.seekp(file_desc_.tellp() + ch_off);
			ch_off = 2;

			do {

				file_desc_.seekp(file_desc_.tellp() - ch_off);

				if (file_desc_.fail()) [[unlikely]]
					throw std::exception("Reached beginning of file (in defined multiple read) (FileControl).");

				file_desc_.get(ch);
				raw_data.push_back(ch);
				std::reverse(raw_data.begin(), raw_data.end());

				--recent_line_index_;

			} while (ch != '\n');

			if (*curr_obj = raw_data) [[unlikely]]
				throw std::exception("Conversion error at data assignment to object (FileControl).");
		});
	}
	else {

		throw std::exception("Uninitialized file object (FileControl).");
	}
}

// fstream::seekp(size_t) should be used to specify the insertion place in the output stream
// further improvement
void FileControl::WriteLineUndefined(genmath::ObjectBase* object) {

	if (initialized_ && (mode_ == 0 || mode_ == 1 || mode_ == 3 || mode_ == 4)) [[likely]] {

		std::string raw_data;

		raw_data = *object;
		file_desc_ << (raw_data + "\n");
		++recent_line_index_;

		file_desc_.flush();
	}
	else {

		throw std::exception("Unauthorized file operation (FileControl).");
	}
}

void FileControl::WriteLineUndefined(std::string object) {

	if (initialized_ && (mode_ == 0 || mode_ == 1 || mode_ == 3 || mode_ == 4)) [[likely]] {

		file_desc_ << object;
		++recent_line_index_;

		file_desc_.flush();
	}
	else {

		throw std::exception("Unauthorized file operation (FileControl).");
	}
}

void FileControl::WriteLinesDefined(std::vector<genmath::ObjectBase*>& objects) {

	if (initialized_ && (mode_ == 0 || mode_ == 1 || mode_ == 3 || mode_ == 4)) [[likely]] {

		std::string raw_data;

		std::for_each(objects.begin(), objects.end(), [this, &raw_data](genmath::ObjectBase* curr_obj) {

			raw_data = *curr_obj;
			file_desc_ << (raw_data + "\n");
			++recent_line_index_;
			});

		if (file_desc_.fail()) [[unlikely]]
			throw std::exception("File operation error (FileControl).");

		file_desc_.flush();
	}
	else {

		throw std::exception("Unauthorized file operation (FileControl).");
	}
}

void FileControl::InsertLineUndefined(size_t from, std::string object) {

	// skipping prefix
	file_desc_.seekp(0);
	size_t record_no = 0;

	while (!file_desc_.eof() && from < record_no) {

		file_desc_.ignore(std::numeric_limits<size_t>::max(), '\n');
		++record_no;
	}

	if (file_desc_.eof()) [[unlikely]]
		throw std::exception("Reached end of file (FileControl).");

	std::streampos insertion_start = file_desc_.tellp();

	// buffering insertion line
	suffix_buffer_.clear();
	suffix_buffer_ << object + '\n';

	// buffering suffix data
	std::string buffer;

	while (!file_desc_.eof()) {

		std::getline(file_desc_, buffer, '\n');
		suffix_buffer_ << buffer;
	}

	file_desc_.seekp(insertion_start);

	if (file_desc_.fail()) [[unlikely]]
		throw std::exception("Error at seekiking write position (FileControl).");

	// writing back insertion line and suffix data
	while (!suffix_buffer_.eof()) {

		std::getline(suffix_buffer_, buffer, '\n');
		file_desc_ << buffer + '\n';
	}

	// seek after the inserted line
	file_desc_.seekp(insertion_start);
	file_desc_.ignore(std::numeric_limits<size_t>::max(), '\n');
	recent_line_index_ = from + 1;

	file_desc_.flush();
}

void FileControl::InsertLineUndefined(size_t from, genmath::ObjectBase* object) {

	// skipping prefix
	file_desc_.seekp(0);
	size_t record_no = 0;

	while (!file_desc_.eof() && from < record_no) {

		file_desc_.ignore(std::numeric_limits<size_t>::max(), '\n');
		++record_no;
	}

	if (file_desc_.eof()) [[unlikely]]
		throw std::exception("Reached end of file (FileControl).");

	std::streampos insertion_start = file_desc_.tellp();

	// buffering insertion line
	suffix_buffer_.clear();
	suffix_buffer_ << std::string(*object) + '\n';

	// buffering suffix data
	std::string buffer;

	while (!file_desc_.eof()) {

		std::getline(file_desc_, buffer, '\n');
		suffix_buffer_ << buffer;
	}

	file_desc_.seekp(insertion_start);

	if (file_desc_.fail()) [[unlikely]]
		throw std::exception("Error at seekking write position (FileControl).");

	// writing back insertion line and suffix data
	while (!suffix_buffer_.eof()) {

		std::getline(suffix_buffer_, buffer, '\n');
		file_desc_ << buffer + '\n';
	}

	// seek after the inserted line
	file_desc_.seekp(insertion_start);
	file_desc_.ignore(std::numeric_limits<size_t>::max(), '\n');
	recent_line_index_ = from + 1;

	file_desc_.flush();
}

void FileControl::InsertLinesDefined(size_t from, std::vector<genmath::ObjectBase*>& objects) {

	// skipping prefix
	file_desc_.seekp(0);
	size_t record_no = 0;

	while (!file_desc_.eof() && from < record_no) {

		file_desc_.ignore(std::numeric_limits<size_t>::max(), '\n');
		++record_no;
	}

	if (file_desc_.eof()) [[unlikely]]
		throw std::exception("Reached end of file (FileControl).");

	std::streampos insertion_start = file_desc_.tellp();

	// buffering insertion line
	std::string buffer;
	suffix_buffer_.clear();

	std::for_each(objects.begin(), objects.end(), [this](genmath::ObjectBase* curr_object) {

		suffix_buffer_ << std::string(*curr_object) + '\n';
		});

	// buffering suffix data
	while (!file_desc_.eof()) {

		std::getline(file_desc_, buffer, '\n');
		suffix_buffer_ << buffer;
	}

	file_desc_.seekp(insertion_start);

	if (file_desc_.fail()) [[unlikely]]
		throw std::exception("Error at seeking write position (FileControl).");

	// writing back insertion lines and suffix data
	while (!suffix_buffer_.eof()) {

		std::getline(suffix_buffer_, buffer, '\n');
		file_desc_ << buffer + '\n';
	}

	// seek after the inserted line
	file_desc_.seekp(insertion_start);
	size_t num_of_insertion_lines = objects.size();

	for (size_t i = 0; i < num_of_insertion_lines; ++i)
		file_desc_.ignore(std::numeric_limits<size_t>::max(), '\n');

	recent_line_index_ = from + num_of_insertion_lines + 1;

	file_desc_.flush();
}

void FileControl::EraseLine(size_t from) {

	// skipping prefix
	file_desc_.seekp(0);
	size_t record_no = 0;

	while (!file_desc_.eof() && from < record_no) {

		file_desc_.ignore(std::numeric_limits<size_t>::max(), '\n');
		++record_no;
	}

	if (file_desc_.eof()) [[unlikely]]
		throw std::exception("Reached end of file (FileControl).");

	std::streampos deletion_start = file_desc_.tellp();

	// skipping removal line
	if (!file_desc_.eof()) [[unlikely]]
		file_desc_.ignore(std::numeric_limits<size_t>::max(), '\n');
	else
		throw std::exception("Reached end of file (FileControl).");

	// buffering suffix data
	std::string buffer;
	suffix_buffer_.clear();

	while (!file_desc_.eof()) {

		std::getline(file_desc_, buffer, '\n');
		suffix_buffer_ << buffer;
	}

	file_desc_.seekp(deletion_start);

	if (file_desc_.fail()) [[unlikely]]
		throw std::exception("Error at seeking write position (FileControl).");

	// writing suffix data back
	while (!suffix_buffer_.eof()) {

		std::getline(suffix_buffer_, buffer, '\n');
		file_desc_ << buffer + "\n";
	}

	// nulling occupied space (removal side effect)
	file_desc_ << ">>!<<\n";

	// seek after erased line
	file_desc_.seekp(deletion_start);
	file_desc_.ignore(std::numeric_limits<size_t>::max(), '\n');
	recent_line_index_ = from + 1;

	file_desc_.flush();

	erase_used_ = true;
}

void FileControl::EraseLines(size_t from, size_t num_of_lines) {

	// skipping prefix data
	file_desc_.seekp(0);
	size_t record_no = 0;

	while (!file_desc_.eof() && from < record_no) {

		file_desc_.ignore(std::numeric_limits<size_t>::max(), '\n');
		++record_no;
	}

	if (file_desc_.eof()) [[unlikely]]
		throw std::exception("Reached end of file (FileControl).");

	std::streampos deletion_start = file_desc_.tellp();

	// skipping removal lines
	while (!file_desc_.eof() && from + num_of_lines < record_no) {

		file_desc_.ignore(std::numeric_limits<size_t>::max(), '\n');
		++record_no;
	}

	if (file_desc_.eof()) [[unlikely]]
		throw std::exception("Reached end of file (FileControl).");

	// buffering suffix data
	// at deletion, new file size is less than the previous due to removal of one or more lines
	std::string buffer;
	suffix_buffer_.clear();

	while (!file_desc_.eof()) {

		std::getline(file_desc_, buffer, '\n');
		suffix_buffer_ << buffer + "\n";
	}

	file_desc_.seekp(deletion_start);

	if (file_desc_.fail()) [[unlikely]]
		throw std::exception("Error at seeking write position (FileControl).");

	// writing suffix data back
	while (!suffix_buffer_.eof()) {

		std::getline(suffix_buffer_, buffer, '\n');
		file_desc_ << buffer + "\n";
	}

	// nulling occupied space (removal side effect)
	while (!file_desc_.eof())
		file_desc_ << ">>!<<\n";

	// seek after erased lines
	file_desc_.seekp(deletion_start);

	for (size_t i = 0; i < num_of_lines; ++i)
		file_desc_.ignore(std::numeric_limits<size_t>::max(), '\n');

	recent_line_index_ = from + num_of_lines + 1;

	file_desc_.flush();

	erase_used_ = true;
}

void FileControl::CleanUp() {

	// buffering the entire file content
	suffix_buffer_.clear();
	file_desc_.seekp(0);
	std::string buffer;

	while (file_desc_.eof()) {

		std::getline(file_desc_, buffer, '\n');

		if (buffer != ">>!<<") [[likely]]
			suffix_buffer_ << buffer + '\n';
		else
			break;
	}

	// reopening file in truncation mode
	this->ChangeSource(source_, FileControl::WT, skip_empty_lines_);

	// writing content back into file
	while (!suffix_buffer_.eof()) {

		std::getline(suffix_buffer_, buffer, '\n');
		file_desc_ << buffer + '\n';
	}
}
