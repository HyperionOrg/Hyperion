#pragma once

#include <mutex>
#include <queue>

namespace Hyperion
{
	template<typename T>
	class ThreadSafeQueue
	{
	public:
		using value_type = T;
		using reference = value_type&;
		using const_reference = const value_type&;

	private:
		std::mutex m_QueueMutex;
		std::deque<value_type> m_Queue;
		std::mutex m_BlockingMutex;
		std::condition_variable m_Blocking;

	public:
		ThreadSafeQueue() = default;
		ThreadSafeQueue(const ThreadSafeQueue<value_type>&) = delete;
		virtual ~ThreadSafeQueue()
		{
			clear();
		}

		reference front()
		{
			std::scoped_lock lock{ m_QueueMutex };
			return m_Queue.front();
		}

		const_reference front() const
		{
			std::scoped_lock lock{ m_QueueMutex };
			return m_Queue.front();
		}

		reference back()
		{
			std::scoped_lock lock{ m_QueueMutex };
			return m_Queue.back();
		}

		const_reference back() const
		{
			std::scoped_lock lock{ m_QueueMutex };
			return m_Queue.back();
		}

		value_type pop_front()
		{
			std::scoped_lock lock{ m_QueueMutex };
			auto t = std::move(m_Queue.front());
			m_Queue.pop_front();
			return t;
		}

		value_type pop_back()
		{
			std::scoped_lock lock{ m_QueueMutex };
			auto t = std::move(m_Queue.back());
			m_Queue.pop_back();
			return t;
		}

		void push_back(const_reference item)
		{
			std::scoped_lock lock{ m_QueueMutex };
			m_Queue.emplace_back(std::move(item));

			std::unique_lock<std::mutex> ul(m_BlockingMutex);
			m_Blocking.notify_one();
		}

		void push_front(const_reference item)
		{
			std::scoped_lock lock{ m_QueueMutex };
			m_Queue.emplace_front(std::move(item));

			std::unique_lock<std::mutex> ul(m_BlockingMutex);
			m_Blocking.notify_one();
		}

		bool empty()
		{
			std::scoped_lock lock{ m_QueueMutex };
			return m_Queue.empty();
		}

		size_t count()
		{
			std::scoped_lock lock{ m_QueueMutex };
			return m_Queue.size();
		}

		void clear()
		{
			std::scoped_lock lock{ m_QueueMutex };
			m_Queue.clear();
		}

		void wait()
		{
			while (empty())
			{
				std::unique_lock<std::mutex> uniqueLock{ m_BlockingMutex };
				m_Blocking.wait(uniqueLock);
			}
		}
	};
}
