#pragma once

#include "GameObjectsComponentsIterator.h"

/*!
 * \brief Class for viewing of the collection of game objects with specified component
 *
 * This class allows to iterate over game objects collection with specified component
 */
template<class ComponentType>
class GameObjectsComponentsView {
 public:
  GameObjectsComponentsView(const GameObjectsComponentsIterator<ComponentType>& begin,
    const GameObjectsComponentsIterator<ComponentType>& end)
    : m_begin(begin),
      m_end(end)
  {
    GameObject gameObject = m_begin.getGameObject();

    // Prevent invalid iterator initialization
    if (!gameObject.isAlive()) {
      // If the first iterator points to invalid object,
      // increment it to find valid first value in internal loop
      ++m_begin;
    }
  }

  ~GameObjectsComponentsView() = default;

  /*!
   * \brief Returns an iterator to the beginning of the collection
   *
   * \return beginning of the sequence iterator
   */
  [[nodiscard]] inline const GameObjectsComponentsIterator<ComponentType>& begin() const
  {
    return m_begin;
  }

  /*!
   * \brief Returns an iterator to the end of the collection
   *
   * \return end of the sequence iterator
   */
  [[nodiscard]] inline const GameObjectsComponentsIterator<ComponentType>& end() const
  {
    return m_end;
  }

 private:
  GameObjectsComponentsIterator<ComponentType> m_begin{};
  GameObjectsComponentsIterator<ComponentType> m_end{};
};
