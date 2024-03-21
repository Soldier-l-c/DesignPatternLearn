#pragma once
namespace Sort
{
	//冒泡
	/*
	从左到右不断交换相邻逆序的元素，在一轮的循环之后，可以让未排序的最大元素上浮到右侧。
	在一轮循环中，如果没有发生交换，那么说明数组已经是有序的，此时可以直接退出。
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

	//选择
	/*
	从数组中选择最小元素，将它与数组的第一个元素交换位置。再从数组剩下的元素中选择出最小的元素，
	将它与数组的第二个元素交换位置。不断进行这样的操作，直到将整个数组排序。
	选择排序需要 ~N2/2 次比较和 ~N 次交换，它的运行时间与输入无关，
	这个特点使得它对一个已经排序的数组也需要这么多的比较和交换操作。
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

	//插入排序
	/*
	每次都将当前元素插入到左侧已经排序的数组中，使得插入之后左侧数组依然有序。
	对于数组 {3, 5, 2, 4, 1}，它具有以下逆序：(3, 2), (3, 1), (5, 2), (5, 4), (5, 1), (2, 1), (4, 1)，
	插入排序每次只能交换相邻元素，令逆序数量减少 1，因此插入排序需要交换的次数为逆序数量。
	插入排序的时间复杂度取决于数组的初始顺序，如果数组已经部分有序了，那么逆序较少，需要的交换次数也就较少，时间复杂度较低。
	平均情况下插入排序需要 ~N2/4 比较以及 ~N2/4 次交换；
	最坏的情况下需要 ~N2/2 比较以及 ~N2/2 次交换，最坏的情况是数组是倒序的；
	最好的情况下需要 N-1 次比较和 0 次交换，最好的情况就是数组已经有序了。
	*/
	template<class Value, class Cmp = std::less<Value>>
	void Insertion(std::vector<Value>& value_list, Cmp cmp = Cmp())
	{
		if (value_list.empty())return;

		//最开始认为value_lis[0]到value_lis[0]是有序的，
		//将value_list[1]插入到有序队列，此时有序队列为0到1
		//依次将1到n-1的数据插入到有序队列，实现排序

		for (auto i = 1; i < value_list.size(); ++i)
		{
			for (auto j = i; j > 0 && cmp(value_list[j], value_list[j - 1]); --j)
			{
				std::swap(value_list[j], value_list[j - 1]);
			}
		}
	}

	//快排
	/*
	归并排序将数组分为两个子数组分别排序，并将有序的子数组归并使得整个数组排序；
	快速排序通过一个切分元素将数组分为两个子数组，左子数组小于等于切分元素，右子数组大于等于切分元素，将这两个子数组排序也就将整个数组排序了。
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

	//希尔排序
	/*
	对于大规模的数组，插入排序很慢，因为它只能交换相邻的元素，每次只能将逆序数量减少 1。
	希尔排序的出现就是为了解决插入排序的这种局限性，它通过交换不相邻的元素，每次可以将逆序数量减少大于 1。
	希尔排序使用插入排序对间隔 h 的序列进行排序。通过不断减小 h，最后令 h=1，就可以使得整个数组是有序的。
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

	//归并
	//归并排序的思想是将数组分成两部分，分别进行排序，然后归并起来。
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
			//保证稳定性,只有前面的数大于后面的才用后面的，前面的小于等于后面的就用前面的
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

	//堆排序

	//上浮
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

	//下沉
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