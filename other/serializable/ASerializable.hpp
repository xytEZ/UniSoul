#ifndef ASERIALIZABLE_HPP_
# define ASERIALIZABLE_HPP_

# include <utility>
# include <boost/serialization/serialization.hpp>

namespace Serializable
{
  template <typename T>
  class ASerializable
  {
  private :
    friend class boost::serialization::access;

  protected :
    T		_component;
    
  public :
    ASerializable() = default;
    ASerializable(const T&);
    virtual ~ASerializable() = default;
    virtual const T& getSerializableComponent() const = 0;
    virtual void setSerializableComponent(const T&) = 0;

  private :
    template <class Archive>
    void serialize(Archive&, const unsigned int);
  };

  template <typename T>
  ASerializable<T>::ASerializable(const T& component) :
    _component(std::move(component))
  {
  }
  
  template <typename T>
  template <class Archive>
  void ASerializable<T>::serialize(Archive&, const unsigned int) { }
}

#endif /* !ASERIALIZABLE_HPP_ */
