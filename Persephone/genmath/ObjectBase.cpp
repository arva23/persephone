#include "ObjectBase.h"

genmath::ObjectBase::ObjectBase() {

}

genmath::ObjectBase::ObjectBase(const ObjectBase& orig) {

}

genmath::ObjectBase& genmath::ObjectBase::operator=(const ObjectBase& orig) {

	return *this;
}

bool genmath::ObjectBase::operator=(std::string param) {
	
	return false;
}

genmath::ObjectBase::~ObjectBase() {

}

genmath::ObjectBase::operator std::string() const {

	return std::string("");
}
