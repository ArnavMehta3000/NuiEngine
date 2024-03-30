namespace Nui
{
	/**
	 * @brief Template class for creating a singleton instance of a class.
	 * @tparam T The type of the class to be made a singleton.
	 */
	template <typename T>
	class Singleton final
	{
	public:
		/**
		 * @brief Get the singleton instance.
		 * @return T& A reference to the singleton instance.
		 */
		static T& Get() noexcept
		{
			return *GetPtr();
		}

		/**
		 * @brief Destroy the singleton instance.
		 */
		static void Destroy() noexcept
		{
			T* ptr = GetPtr();
			if (ptr)
			{
				delete ptr;
				ptr = nullptr;
			}
		}

		Singleton(Singleton&&) = delete;
		Singleton(const Singleton&) = delete;
		Singleton& operator=(Singleton&&) = delete;
		Singleton& operator=(Singleton&) = delete;

	private:

		/**
		 * @brief Get the pointer to the singleton instance.
		 * @return T* A pointer to the singleton instance.
		 */
		static T* GetPtr() noexcept
		{
			static Singleton<T> s_instance;
			return s_instance.m_obj;
		}

	private:
		Singleton() = default;

		/**
		 * @brief m_obj is a pointer to the singleton instance.
		 */
		T* m_obj = new T();
	};
}