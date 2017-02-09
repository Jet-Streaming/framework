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
	// ��jhttp_t��Ҫterminateʱ��ֻ����øö������������������Ҫ�ͷ��ڴ档���ڴ�һ����ҵ�������GC����C++������ָ�룩����
	//object *be_object;

	uv_handle_t close_handle;
	uint32_t closing : 1; // ���ٴ����κ�mbuf
	uint32_t terminated : 1;
};

} // namespace jet