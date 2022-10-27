/*! An Option wrapper for C++11

 * Copyright (C) 2022 Nichts Hsu

 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef OPTION_H
#define OPTION_H

/**
 * @brief The Option class
 * A wrapper with an instance of T or a None value.
 */
template<typename T>
class Option
{
public:
    /**
     * @brief Option
     * Initialize with None value
     */
    Option();

    /**
     * @brief Option
     * Deep copy of a Option
     * @param another
     */
    Option(const Option &another);

    /**
     * @brief Option
     * Initialize with instance of T
     * @param args
     * The instance of T or construct arguments of T
     */
    template<typename... Args>
    Option(const Args &...args);

    ~Option();

    /**
     * @brief isSome
     * @return if is an intance of T
     */
    bool isSome() const;

    /**
     * @brief isNone
     * @return if is a None value
     */
    bool isNone() const;

    /**
     * @brief get
     * @return get the unchecked instance, will be crash if is None value
     */
    T &get();
    const T &get() const;

    /**
     * @brief safeGet
     * Always return a valid reference.
     * @param def
     * Default value if is None.
     * @return
     */
    T &safeGet(T &def);
    const T &safeGet(const T &def) const;

    /**
     * @brief setNone
     * Set to None value
     */
    void setNone();

    /**
     * @brief set
     * Set to instance of T
     * @param args
     * The instance of T or construct arguments of T
     */
    template<typename... Args>
    void set(const Args &...args);

    /**
     * @brief operator =
     * Set to instance of T
     * @param another
     * The instance of T
     */
    T &operator=(const T &another);

    /**
     * @brief operator =
     * Deep copy from another Option
     * @param another
     */
    Option<T> &operator=(const Option<T> &another);

    /**
     * @brief operator bool
     * Implicit conversion from Option to bool.
     * So you can use `if (option)` or `if (!option)`.
     */
    operator bool() const;

private:
    T *_inner;
};

template<typename T>
Option<T>::Option()
{
    _inner = nullptr;
}

template<typename T>
Option<T>::Option(const Option<T> &another)
{
    if (another.isNone())
        _inner = nullptr;
    else
        _inner = new T(*another._inner);
}

template<typename T>
template<typename... Args>
Option<T>::Option(const Args &...args)
{
    _inner = new T(args...);
}

template<typename T>
Option<T>::~Option()
{
    if (_inner)
        delete _inner;
}

template<typename T>
bool Option<T>::isSome() const
{
    return _inner != nullptr;
}

template<typename T>
bool Option<T>::isNone() const
{
    return _inner == nullptr;
}

template<typename T>
T &Option<T>::get()
{
    return *_inner;
}

template<typename T>
const T &Option<T>::get() const
{
    return *_inner;
}

template<typename T>
T &Option<T>::safeGet(T &def)
{
    if (isNone())
        return def;
    return *_inner;
}

template<typename T>
const T &Option<T>::safeGet(const T &def) const
{
    if (isNone())
        return def;
    return *_inner;
}

template<typename T>
void Option<T>::setNone()
{
    if (_inner)
        delete _inner;
    _inner = nullptr;
}

template<typename T>
template<typename... Args>
void Option<T>::set(const Args &...args)
{
    if (!_inner)
        _inner = new T(args...);
    else
        new (_inner) T(args...);
}

template<typename T>
T &Option<T>::operator=(const T &another)
{
    if (!_inner)
        _inner = new T(another);
    else
        *_inner = another;
    return *_inner;
}

template<typename T>
Option<T> &Option<T>::operator=(const Option<T> &another)
{
    if (another.isNone())
        _inner = nullptr;
    else
        _inner = new T(*another._inner);
    return *this;
};

template<typename T>
Option<T>::operator bool() const
{
    return isSome();
}
#endif // OPTION_H
