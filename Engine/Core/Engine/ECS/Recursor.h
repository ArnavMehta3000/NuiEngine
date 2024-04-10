#pragma once
#include <Common/Types.h>
#include <functional>

namespace Nui::ECS
{
	class IRecursor
	{
	public:
		virtual ~IRecursor() = default;
		virtual void Run() = 0;
	};


	template <typename NodeType, typename RetType>
	class HierarchicalRecursor : public IRecursor
	{
	public:
		using Callback = std::function<RetType(NodeType&)>;

	public:
		virtual ~HierarchicalRecursor() = default;

		inline void BindRoot(NodeType root) { m_root = root; }
		inline void BindCallback(Callback callback) { m_callback = callback; }

	protected:
		inline NodeType& GetRoot() { return m_root; }
		inline Callback& GetCallback() { return m_callback; }

	private:
		NodeType m_root;
		Callback m_callback;
	};

	template <typename NodeType, typename RetType, typename IterType>
	class SequentialRecursor : public IRecursor
	{
	public:
		using iterator = IterType;
		using Callback = std::function<RetType(NodeType)>;

	public:
		virtual ~SequentialRecursor() = default;

		inline void BindIterator(IterType begin, IterType end) { m_begin = begin; m_end = end; }
		inline void BindCallback(Callback callback) { m_callback = callback; }

	protected:
		inline IterType begin() { return m_begin; }
		inline IterType end() { return m_end; }		
		inline Callback& GetCallback() { return m_callback; }

	private:
		IterType m_begin, m_end;
		Callback m_callback;
	};
}