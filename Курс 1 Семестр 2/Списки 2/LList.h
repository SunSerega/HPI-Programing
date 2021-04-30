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

	void AddNode(LListNode* n, LListNode* before) {
		if (!before) {

			if (last) {
				last->next = n;
				n->prev = last;
			}
			else {
				first = n;
			}

			last = n;
			n->next = nullptr;
		}
		else {
			auto after = before->prev;

			if (after) {
				n->prev = after;
				after->next = n;
			}
			else {
				n->prev = nullptr;
				first = n;
			}

			before->prev = n;
			n->next = before;
		}
		++len;
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

	void Clear() {
		auto curr = first;
		while (curr) {
			auto next = curr->next;
			delete curr;
			curr = next;
		}
		first = nullptr;
		last = nullptr;
		len = 0;
	}

#pragma region Add

	template <class T2 = T&&>
	LList<T>& Add(T2&& el) {
		//static_assert(std::is_convertible_v<std::remove_reference<T2>, T>, "Bad element type");
		if (0 > len)
			throw std::out_of_range("ToDo"); //ToDo
		auto n = new LListNode(std::forward<T2>(el));

		AddNode(n, nullptr);
		return *this;
	}
	template <class T2 = T&&>
	auto& operator<<(T2&& el) {
		return Add(std::forward<T2>(el));
	}

	template <class T2 = T&&>
	LList<T>& AddAt(T2&& el, int ind) {
		if ((unsigned)ind > len)
			throw std::out_of_range("ToDo"); //ToDo
		auto n = new LListNode(std::forward<T2>(el));

		auto curr = first;
		while (ind--) curr = curr->next;

		AddNode(n, curr);
		return *this;
	}

#pragma endregion

#pragma region Remove

	template <class TProc>
	LList<T>& RemoveWhere(TProc p) {
		auto curr = first;

		while (true) {
			auto next = curr->next;

			if (p(curr->item))
				RemoveNode(curr);

			if (!next) break;
			curr = next;
		}

		return *this;
	}

	LList<T>& Remove(const T& el) {
		return RemoveWhere([&](T& x) { return x == el; });
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

#pragma region Sort

	template <class TComp>
	LList<T>& Sort(TComp comp) {
		auto curr = first;
		first = nullptr;
		last = nullptr;
		len = 0;

		while (true) {
			auto next = curr->next;

			auto curr2 = first;
			while (true) {
				if (!curr2 || comp(curr->item, curr2->item) < 0)
				{
					AddNode(curr, curr2);
					break;
				}
				curr2 = curr2->next;
			}

			if (!next) break;
			curr = next;
		}

		return *this;
	}

#pragma endregion

#pragma region Iteratorable

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


