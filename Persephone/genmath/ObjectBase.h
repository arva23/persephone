#ifndef OBJECTBASE_H_INCLUDED
#define OBJECTBASE_H_INCLUDED
#include <string>
#include <stdint.h>
#include <vector>
#include <initializer_list>
#include <exception>

namespace genmath {

	class ObjectBase {
	
	public:
		ObjectBase();
		ObjectBase(const ObjectBase& orig);
		ObjectBase& operator=(const ObjectBase& orig);
		virtual bool operator=(std::string param);
		virtual ~ObjectBase();
		virtual operator std::string() const;
	};
}

#endif// OBJECTBASE_H_INCLUDED