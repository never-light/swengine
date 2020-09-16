#pragma once

#include "GameObjectsSequentialIterator.h"

/*!
 * \brief Class for game objects collection viewing
 *
 * This class allows to iterate over game objects collection
 */
class GameObjectsSequentialView {
 public:
  GameObjectsSequentialView(const GameObjectsSequentialIterator& begin, const GameObjectsSequentialIterator& end)
    : m_begin(begin),
      m_end(end)
  {
    // Prevent invalid iterator init
    if (!m_begin.getGameObject().isAlive()) {
      // If the first iterator points to invalid object,
      // increment it to find valid first value in internal loop
      ++m_begin;
    }
  }

  ~GameObjectsSequentialView() = default;

  /*!
   * \brief Returns an iterator to the beginning of the collection
   *
   * \return beginning of the sequence iterator
   */
  [[nodiscard]] inline const GameObjectsSequentialIterator& begin() const
  {
    return m_begin;
  }

  /*!
   * \brief Returns an iterator to the end of the collection
   *
   * \return end of the sequence iterator
   */
  [[nodiscard]] inline const GameObjectsSequentialIterator& end() const
  {
    return m_end;
  }

 private:
  GameObjectsSequentialIterator m_begin;
  GameObjectsSequentialIterator m_end;
};