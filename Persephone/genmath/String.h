#pragma once
#include <string>
#include <exception>
#include "ObjectBase.h"

namespace genmath {

	class String : public ObjectBase {

	public:
		String();
		String(const String& orig);
		String(std::string data);
		String(const char* data);
		virtual ~String();

		bool Empty();
		bool operator=(std::string data)override;

		String& operator=(const String& orig);
		void operator+=(String& data);
		void operator+=(String data);
		bool operator!=(String& data);

		bool operator<(const String operand) const;

		operator std::string() const override;
		std::string& GetData();
		size_t Size();

	private:
		std::string data_;
	};
}

