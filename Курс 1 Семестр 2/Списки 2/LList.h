#pragma once

#include <stdexcept>



template <class T>
class LList
{
private:

	struct LListNode
	{
		T item;
		LListNode* prev = NULL;
		LListNode* next = NULL;

		template <class T2 = T&&>
		LListNode(T2&& item) : item(std::forward<T2>(item)) {}

	};

	LListNode* first = nullptr;
	LListNode* last = nullptr;
	unsigned len = 0;

	void RemoveNode(LListNode* n) {
		auto prev = n->prev;
		auto next = n->next;

		if (prev)
			prev->next = next; else
			first = next;

		if (next)
			next->prev = prev; else
			last = prev;

		--len;
		delete n;
	}

public:
	LList() {}
	LList(LList&& l) noexcept
		: first(l.first)
		, last(l.last)
		, len(l.len)
	{
		l.first = nullptr;
		l.last = nullptr;
		l.len = 0;
	}

	~LList() {
		auto curr = first;
		while (curr) {
			auto next = curr->next;
			delete curr;
			curr = next;
		}
		len = -1;
	}

	auto Count() {
		return len;
	}

#pragma region Add

	template <class T2 = T&&>
	LList<T>& Add(T2&& el) {
		//static_assert(std::is_convertible_v<std::remove_reference<T2>, T>, "Bad element type");
		if (0 > len)
			throw std::out_of_range("ToDo"); //ToDo
		auto n = new LListNode(std::forward<T2>(el));

		if (last)
		{
			last->next = n;
			n->prev = last;
		}
		else
		{
			first = n;
		}
		last = n;

		++len;
		return *this;
	}
	template <class T2 = T&&>
	auto& operator<<(T2&& el) {
		return Add(std::forward<T2>(el));
	}

	template <class T2 = T>
	LList<T>& AddAt(T2&& el, int ind) {
		if ((unsigned)ind > len)
			throw std::out_of_range("ToDo"); //ToDo

		if (ind == len)
			return Add(std::forward(el));
		auto n = new LListNode<T>(std::forward(el));

		auto curr = first;
		while (ind--) curr = curr->next;
		auto prev = curr->prev;

		if (prev)
			prev->next = n; else
			first = n;
		curr->prev = n;

		n->prev = prev;
		n->next = curr;

		++len;
		return *this;
	}

#pragma endregion

#pragma region Remove

	LList<T>& Remove(const T& el) {

		auto curr = first;
		while (true) {
			auto next = curr->next;

			if (curr->item == el)
				RemoveNode(curr);

			if (!next) break;
			curr = next;
		}

		return *this;
	}

	LList<T>& RemoveAt(int ind) {

		if ((unsigned)ind >= len)
			throw std::out_of_range("ToDo"); //ToDo

		auto curr = first;
		while (ind--) curr = curr->next;

		RemoveNode(curr);

		return *this;
	}

#pragma endregion

#pragma region Sequence

private:
	class NodeIterator
	{
	private:
		LListNode* n;

		friend LList<T>;
		NodeIterator(LListNode* n) : n(n) {}

	public:

		void operator++() {
			this->n = n->next;
		}

		T& operator*() {
			return n->item;
		}

		bool operator!=(NodeIterator& other) {
			return this->n != other.n;
		}

	};

public:
	auto begin() const {
		return NodeIterator(first);
	}

	auto end() const {
		return NodeIterator(nullptr);
	}

#pragma endregion

};


