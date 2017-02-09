#pragma once

namespace jet {

class element_private : public object_private
{
	J_DECLARE_PUBLIC(element);

public:
	element_private();
	virtual ~element_private();

	const char *name;
	element *parent;

	std::vector<pad *> pads;
	//LIST_HEAD(, pad) pads;

	// eg: jlhttp_t
	// 当jhttp_t需要terminate时，只需调用该对象的析构函数，不需要释放内存。该内存一般由业务引擎的GC（或C++的智能指针）管理
	//object *be_object;

	uv_handle_t close_handle;
	uint32_t closing : 1; // 不再处理任何mbuf
	uint32_t terminated : 1;
};

} // namespace jet