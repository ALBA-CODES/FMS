/*
 * Pool.cpp
 * 
 * Copyright 2021 André Luis <André Luis@DESKTOP-HDL2CBS>
 * 
 * 
 */
 
#include "../include/Pool.h"


Pool::Pool(){}
Pool::Pool(Pool&& obj){}
Pool& Pool::operator=(const Pool& obj){
 return *this;
}
Pool::~Pool(){}
		
size_t Pool::size(){
	const std::lock_guard<std::mutex> lock(mtx);
	size_t result = data.size();
	return result;
}


void Pool::push(Node* item){
	const std::lock_guard<std::mutex> lock(mtx);
	data.push(item);
}

Node* Pool::pop()
{
	const std::lock_guard<std::mutex> lock(mtx);

		if (data.size() <= 0)
		{
			return NULL;
		}
		Node* item = std::move(data.front()); data.pop();
		return item;
}

