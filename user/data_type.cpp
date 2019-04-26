#include "data_type.h"

template<typename T>
abstract_type<T>::abstract_type()
{
	va = 0;
}

template<typename T>
abstract_type<T>::abstract_type(size_t va)
{
	this->va = va;
}

template<typename T>
abstract_type<T>&
abstract_type<T>::operator = (const T &val)
{
	
}

template<typename T>
abstract_type<T>&
abstract_type<T>::operator = (const abstract_type<T> &val)
{
	
}

template<typename T>
abstract_type<T>::operator T() const
{
	
}

template<typename T>
void abstract_type<T>::intr_hook()
{
	
}
