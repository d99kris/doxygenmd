namespace MyNamespace
{
  /**
   * @brief     Simple test class with documented API for 
   *            testing API generation using doxygenmd.
   */
  class MyClass
  {
  public:
    /**
     * @brief   Constructor
     * @param   p_MyParam             is a regular constructor argument.
     * @param   p_MyDefaultParam      is a constructor argument with a default value.
     */
    explicit MyClass(const bool p_MyParam, const long long p_MyDefaultParam = 1000)
    {
    }

  public:
    /**
     * @brief   is a public class member variable.
     */
    bool m_MyPublicVar;

  private:
    /**
     * @brief   is a private class member variable.
     */
    bool m_MyPrivateVar;
  };

  /**
   * @brief     Simple test struct for doxygenmd testing.
   */
  struct MyStruct
  {
    /**
     * @brief   is a member variable.
     */
    bool m_MyStructVar;
  };
}
