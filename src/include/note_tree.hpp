#pragma once
#include <algorithm>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <vector>


template <typename T>
class NoteTree;

template <typename T>
using SharedNoteTreePtr = std::shared_ptr<NoteTree<T>>;

template <typename T>
using WeakNoteTreePtr = std::weak_ptr<NoteTree<T>>;

template <typename T>
class NoteTree : public std::enable_shared_from_this<NoteTree<T>>
{
  public:
    static SharedNoteTreePtr<T>
    create(const T &label, SharedNoteTreePtr<T> parent = nullptr)
    {
        return SharedNoteTreePtr<T>(new NoteTree<T>(label, parent));
    }

    void
    removeChilds()
    {
        m_childs.clear();
    }

    SharedNoteTreePtr<T>
    appendChild(const T &label)
    {
        auto parent = this->shared_from_this();
        auto child  = SharedNoteTreePtr<T>(new NoteTree<T>(label, parent));
        m_childs.push_back(child);
        return child;
    }

    SharedNoteTreePtr<T>
    child(int row) const
    {
        if (row < 0 || row >= m_childs.size())
        {
            throw std::runtime_error("Invalid row given.");
        }
        return m_childs.at(row);
    }

    SharedNoteTreePtr<T>
    lastChild() const
    {
        return m_childs.back();
    }

    const T &
    value() const
    {
        return m_value;
    }

    int
    childCount() const
    {
        return (int)m_childs.size();
    }

    int
    columnCount() const
    {
        return 1;
    }

    int
    row() const
    {
        if (auto parent = m_parent.lock())
        {
            int  index = 0;
            auto iter  = std::find_if(
                parent->m_childs.begin(), parent->m_childs.end(), [&index, this](SharedNoteTreePtr<T> item) {
                    if (item.get() == this)
                    {
                        return true;
                    }
                    index++;
                    return false;
                });
            return (iter != parent->m_childs.end()) ? index : 0;
        }
        return 0;
    }

    WeakNoteTreePtr<T>
    parent() const
    {
        return m_parent;
    }

  private:
    T                                 m_value;
    WeakNoteTreePtr<T>                m_parent;
    std::vector<SharedNoteTreePtr<T>> m_childs;

    NoteTree(const T &label, SharedNoteTreePtr<T> parent = nullptr) : m_value(label), m_parent(parent)
    {
    }
};
