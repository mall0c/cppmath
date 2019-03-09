#ifndef MATH_OFFSETPOLYGON_HPP
#define MATH_OFFSETPOLYGON_HPP

#include <vector>
#include "Polygon.hpp"

namespace math
{
    template <typename T>
    class OffsetPolygon : public BasePolygon<T>
    {
        public:
            OffsetPolygon();
            OffsetPolygon(size_t capacity);
            virtual ~OffsetPolygon() {}

            virtual Point2<T> get(size_t i) const override;
            virtual size_t    size() const        override;

            void           setOffset(const Vec2<T>& offset);
            void           move(const Vec2<T>& rel);
            const Vec2<T>& getOffset() const;

        protected:
            virtual void _add(const Point2<T>& point)          override;
            virtual void _edit(size_t i, const Point2<T>& p)   override;
            virtual void _insert(size_t i, const Point2<T>& p) override;
            virtual void _remove(size_t i)                     override;
            virtual void _clear()                              override;

        protected:
            std::vector<Point2<T>> _vertices;
            Vec2<T> _offset;
    };
}

// Implementation
namespace math
{
    template <typename T>
	OffsetPolygon<T>::OffsetPolygon()
	{ }

    template <typename T>
	OffsetPolygon<T>::OffsetPolygon(size_t capacity)
	{
        _vertices.reserve(capacity);
    }

    template <typename T>
    void OffsetPolygon<T>::setOffset(const Vec2<T>& offset)
    {
        this->_bbox.pos += (offset - _offset);
        _offset = offset;
        this->_onVertexChanged();
    }

    template <typename T>
    void OffsetPolygon<T>::move(const Vec2<T>& rel)
    {
        setOffset(_offset + rel);
    }

    template <typename T>
    const Vec2<T>& OffsetPolygon<T>::getOffset() const
    {
        return _offset;
    }

    template <typename T>
	Point2<T> OffsetPolygon<T>::get(size_t i) const
	{
		return _vertices[i] + _offset;
	}

    template <typename T>
	size_t OffsetPolygon<T>::size() const
	{
        return _vertices.size();
	}

    template <typename T>
	void OffsetPolygon<T>::_add(const Point2<T>& point)
	{
        _vertices.push_back(point - _offset);
	}

    template <typename T>
	void OffsetPolygon<T>::_clear()
	{
        _vertices.clear();
	}

    template <typename T>
	void OffsetPolygon<T>::_edit(size_t i, const Point2<T>& p)
	{
        _vertices[i] = p - _offset;
	}

    template <typename T>
	void OffsetPolygon<T>::_insert(size_t i, const Point2<T>& p)
	{
        _vertices.insert(_vertices.begin() + i, p - _offset);
	}

    template <typename T>
	void OffsetPolygon<T>::_remove(size_t i)
	{
        _vertices.erase(_vertices.begin() + i);
	}
}

#endif
