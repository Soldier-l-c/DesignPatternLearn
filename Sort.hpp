#pragma once
namespace Sort
{
	//ð��
	/*
	�����Ҳ��Ͻ������������Ԫ�أ���һ�ֵ�ѭ��֮�󣬿�����δ��������Ԫ���ϸ����Ҳࡣ
	��һ��ѭ���У����û�з�����������ô˵�������Ѿ�������ģ���ʱ����ֱ���˳���
	*/
	template<class Value, class Cmp = std::less<Value>>
	void Bubble(std::vector<Value>& value_list, Cmp cmp = Cmp())
	{
		if (value_list.empty())return;
		bool swapped{ true };

		for (auto i = value_list.size() - 1; i >0 && swapped; --i)
		{
			swapped = false;
			for (auto j = 0; j < i; ++j)
			{
				if (cmp(value_list[j + 1], value_list[j]))
				{
					std::swap(value_list[j], value_list[j + 1]);
					swapped = true;
				}
			}
		}
	}

	//ѡ��
	/*
	��������ѡ����СԪ�أ�����������ĵ�һ��Ԫ�ؽ���λ�á��ٴ�����ʣ�µ�Ԫ����ѡ�����С��Ԫ�أ�
	����������ĵڶ���Ԫ�ؽ���λ�á����Ͻ��������Ĳ�����ֱ����������������
	ѡ��������Ҫ ~N2/2 �αȽϺ� ~N �ν�������������ʱ���������޹أ�
	����ص�ʹ������һ���Ѿ����������Ҳ��Ҫ��ô��ıȽϺͽ���������
	*/
	template<class Value, class Cmp = std::less<Value>>
	void Selection(std::vector<Value>& value_list, Cmp cmp = Cmp())
	{
		if (value_list.empty())return;

		for (auto i = 0; i < value_list.size() - 1; ++i)
		{
			auto index = i;
			for (auto j = i + 1; j < value_list.size(); ++j)
			{
				if (cmp(value_list[j], value_list[index]))
				{
					index = j;
				}
			}
			std::swap(value_list[index], value_list[i]);
		}
	}

	//��������
	/*
	ÿ�ζ�����ǰԪ�ز��뵽����Ѿ�����������У�ʹ�ò���֮�����������Ȼ����
	�������� {3, 5, 2, 4, 1}����������������(3, 2), (3, 1), (5, 2), (5, 4), (5, 1), (2, 1), (4, 1)��
	��������ÿ��ֻ�ܽ�������Ԫ�أ��������������� 1����˲���������Ҫ�����Ĵ���Ϊ����������
	���������ʱ�临�Ӷ�ȡ��������ĳ�ʼ˳����������Ѿ����������ˣ���ô������٣���Ҫ�Ľ�������Ҳ�ͽ��٣�ʱ�临�ӶȽϵ͡�
	ƽ������²���������Ҫ ~N2/4 �Ƚ��Լ� ~N2/4 �ν�����
	����������Ҫ ~N2/2 �Ƚ��Լ� ~N2/2 �ν������������������ǵ���ģ�
	��õ��������Ҫ N-1 �αȽϺ� 0 �ν�������õ�������������Ѿ������ˡ�
	*/
	template<class Value, class Cmp = std::less<Value>>
	void Insertion(std::vector<Value>& value_list, Cmp cmp = Cmp())
	{
		if (value_list.empty())return;

		//�ʼ��Ϊvalue_lis[0]��value_lis[0]������ģ�
		//��value_list[1]���뵽������У���ʱ�������Ϊ0��1
		//���ν�1��n-1�����ݲ��뵽������У�ʵ������

		for (auto i = 1; i < value_list.size(); ++i)
		{
			for (auto j = i; j > 0 && cmp(value_list[j], value_list[j - 1]); --j)
			{
				std::swap(value_list[j], value_list[j - 1]);
			}
		}
	}

	//����
	/*
	�鲢���������Ϊ����������ֱ����򣬲��������������鲢ʹ��������������
	��������ͨ��һ���з�Ԫ�ؽ������Ϊ���������飬��������С�ڵ����з�Ԫ�أ�����������ڵ����з�Ԫ�أ�������������������Ҳ�ͽ��������������ˡ�
	*/
	template<class Value, class Cmp = std::less<Value>>
	void Quick(std::vector<Value>& value_list, int32_t begin, int32_t end, Cmp cmp = Cmp())
	{
		if (begin >= end)
			return;

		auto be = begin;
		auto en = end+1;
		const auto& key = value_list[be];

		while (be < en)
		{
			while (++be < en && cmp(value_list[be], key));
			while (--en > be && cmp(key, value_list[en]));

			if (be >= en)
				break;

			std::swap(value_list[be], value_list[en]);
		}

		std::swap(value_list[begin], value_list[en]);

		Quick(value_list, begin, en - 1, cmp);
		Quick(value_list, en + 1, end, cmp);
	}

	template<class Value, class Cmp = std::less<Value>>
	void Quick(std::vector<Value>& value_list, Cmp cmp = Cmp())
	{
		if (value_list.empty())return;

		Quick(value_list, 0, value_list.size() - 1, cmp);
	}

	//ϣ������
	/*
	���ڴ��ģ�����飬���������������Ϊ��ֻ�ܽ������ڵ�Ԫ�أ�ÿ��ֻ�ܽ������������� 1��
	ϣ������ĳ��־���Ϊ�˽��������������־����ԣ���ͨ�����������ڵ�Ԫ�أ�ÿ�ο��Խ������������ٴ��� 1��
	ϣ������ʹ�ò�������Լ�� h �����н�������ͨ�����ϼ�С h������� h=1���Ϳ���ʹ����������������ġ�
	*/
	template<class Value, class Cmp = std::less<Value>>
	void Shell(std::vector<Value>& value_list, Cmp cmp = Cmp())
	{
		auto N = value_list.size();
		auto h = 1;

		while (h < N / 3) 
		{
			h = 3 * h + 1; // 1, 4, 13, 40, ...
		}

		while (h >= 1) 
		{
			for (int i = h; i < N; i++) 
			{
				for (int j = i; j >= h && cmp(value_list[j], value_list[j - h]); j -= h)
				{
					std::swap(value_list[j], value_list[j - h]);
				}
			}
			h = h / 3;
		}
	}

	//�鲢
	//�鲢�����˼���ǽ�����ֳ������֣��ֱ��������Ȼ��鲢������
	template<class Value, class Cmp = std::less<Value>>
	void Merge(std::vector<Value>& value_list, int32_t begin, int32_t end, int32_t mid, Cmp cmp = Cmp())
	{
		std::vector<Value>temp_list;
		for (auto i = begin; i <= end; ++i)
		{
			temp_list.push_back(value_list[i]);
		}

		auto temp_begin = 0, temp_mid = mid - begin+1, temp_end = end - begin;
		const auto cmid = temp_mid-1;

		while (begin <= end)
		{
			if (temp_begin > cmid)
			{
				value_list[begin++] = temp_list[temp_mid++];
			}
			else if (temp_mid > temp_end)
			{
				value_list[begin++] = temp_list[temp_begin++];
			}
			//��֤�ȶ���,ֻ��ǰ��������ں���Ĳ��ú���ģ�ǰ���С�ڵ��ں���ľ���ǰ���
			else if (cmp(temp_list[temp_mid], temp_list[temp_begin]))
			{
				value_list[begin++] = temp_list[temp_mid++];
			}
			else
			{
				value_list[begin++] = temp_list[temp_begin++];
			}
		}
		
	}

	template<class Value, class Cmp = std::less<Value>>
	void MergeSort(std::vector<Value>& value_list, int32_t begin, int32_t end, Cmp cmp = Cmp())
	{
		if (begin >= end)
			return;

		auto mid = begin + (end-begin) / 2;

		MergeSort(value_list, begin, mid, cmp);
		MergeSort(value_list, mid+1, end, cmp);

		Merge(value_list, begin, end, mid, cmp);

	}

	template<class Value, class Cmp = std::less<Value>>
	void MergeSort(std::vector<Value>& value_list, Cmp cmp = Cmp())
	{
		if (value_list.empty())return;

		auto size = value_list.size();
		MergeSort(value_list, 0, size - 1, cmp);
	}

	//������

	//�ϸ�
	template<class Value, class Cmp = std::less<Value>>
	void Upward(std::vector<Value>& value_list, int32_t index, Cmp cmp = Cmp())
	{
		while (index > 0)
		{
			auto parent_index = (index+1) / 2-1;
			if (cmp(value_list[index], value_list[parent_index]))
			{
				std::swap(value_list[index], value_list[parent_index]);
			}
			else
			{
				break;
			}

			index = parent_index;
		}
	}

	//�³�
	template<class Value, class Cmp = std::less<Value>>
	void Sink(std::vector<Value>& value_list, int32_t index, int32_t size, Cmp cmp = Cmp())
	{
		while (index <= (size)/2-1)
		{
			auto left_index		= index * 2 + 1;
			auto right_index	= index * 2 + 2;

			if (left_index >= size)
				break;

			auto cur_index = (right_index < size && cmp(value_list[left_index], value_list[right_index])) ? right_index : left_index;

			if (!cmp(value_list[index], value_list[cur_index]))
				break;

			std::swap(value_list[index], value_list[cur_index]);

			index = cur_index;
		}
	}

	template<class Value, class Cmp = std::less<Value>>
	void HeapSink(std::vector<Value>& value_list, Cmp cmp = Cmp())
	{
		int32_t size = value_list.size();
		for (auto index = size - 1; index >= 0; --index)
		{
			Sink(value_list, index, size, cmp);
		}

		while (size-- > 0)
		{
			std::swap(value_list[0], value_list[size]);
			Sink(value_list, 0, size, cmp);
		}
	}

	template<class Value, class Cmp = std::less<Value>>
	void HeapUpward(std::vector<Value>& value_list, Cmp cmp = Cmp())
	{
		int32_t size = value_list.size();
		for (auto index = size - 1; index >= 0; --index)
		{
			Upward(value_list, index, cmp);
		}

		while (size-- > 0)
		{
			std::swap(value_list[0], value_list[size]);
			Sink(value_list, 0, size, cmp);
		}
	}


}