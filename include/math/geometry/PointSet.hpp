#ifndef CPPMATH_POINT_SET_HPP
#define CPPMATH_POINT_SET_HPP

#include "Line2.hpp"
#include <vector>

namespace math
{
    template <typename T>
    class AbstractPointSet
    {
        public:
            virtual ~AbstractPointSet() {}

            virtual void      add(const Point2<T>& point)          = 0;
            virtual void      edit(size_t i, const Point2<T>& p)   = 0;
            virtual void      insert(size_t i, const Point2<T>& p) = 0;
            virtual void      remove(size_t i)                     = 0;
            virtual void      clear()                              = 0;
            virtual size_t    size() const                         = 0;
            virtual Point2<T> get(size_t i) const                  = 0;
            virtual AABB<T>   getBBox() const                      = 0;

            // Return a line segment from point i to point j.
            Line2<T> getSegment(size_t i, size_t j) const;

        protected:
            // NOTE: this is intentionally not the default for getBBox()
            //       as it is not cached but recalculates on every call.
            AABB<T> _calculateBBox() const;
    };


    // TODO: find a better class name
    // NOTE: if you change something from the lazy bbox logic,
    //       you probably want to change it in BasePolygon, too.
    template <typename T>
    class BasePointSet : public AbstractPointSet<T>
    {
        public:
            BasePointSet();
            virtual ~BasePointSet() {};

            void add(const Point2<T>& point)          final;
            void edit(size_t i, const Point2<T>& p)   final;
            void insert(size_t i, const Point2<T>& p) final;
            void remove(size_t i)                     final;
            void clear()                              final;

            virtual AABB<T> getBBox() const override;

        protected:
            // Called whenever the vertex list changed
            virtual void _onVertexChanged() {};

            virtual void _add(const Point2<T>& point)          = 0;
            virtual void _edit(size_t i, const Point2<T>& p)   = 0;
            virtual void _insert(size_t i, const Point2<T>& p) = 0;
            virtual void _remove(size_t i)                     = 0;
            virtual void _clear()                              = 0;

        protected:
            mutable AABB<T> _bbox;
            mutable bool _bboxdirty;
    };

    template <typename T>
    class PointSet : public BasePointSet<T>
    {
        public:
            PointSet() = default;
            PointSet(size_t capacity);
            virtual ~PointSet() {}

            virtual Point2<T> get(size_t i) const override;
            virtual size_t    size() const        override;

        protected:
            virtual void _add(const Point2<T>& point)          override;
            virtual void _edit(size_t i, const Point2<T>& p)   override;
            virtual void _insert(size_t i, const Point2<T>& p) override;
            virtual void _remove(size_t i)                     override;
            virtual void _clear()                              override;

        protected:
            std::vector<Point2<T>> _vertices;
    };
}


#include "../math.hpp"
#include "intersect.hpp"

// Implementation
namespace math
{
    // AbstractPointSet
    template <typename T>
    Line2<T> AbstractPointSet<T>::getSegment(size_t i, size_t j) const
    {
        return Line2<T>(get(i), get(j), Segment);
    }

    template <typename T>
    AABB<T> AbstractPointSet<T>::_calculateBBox() const
    {
        if (this->size() < 2)
        {
            return AABB<T>();
        }

        Vec2f min = this->get(0).asVector(),
              max = this->get(0).asVector();

        for (size_t i = 1; i < this->size(); ++i)
        {
            auto p = this->get(i);
            for (int k = 0; k < 2; ++k)
            {
                min[k] = std::min(min[k], p[k]);
                max[k] = std::max(max[k], p[k]);
            }
        }

        return AABB<T>(min.asPoint(), max - min);
    }




    // BasePointSet
    template <typename T>
    BasePointSet<T>::BasePointSet() :
        _bboxdirty(true)
        // NOTE: BBox should recalculate because it doesn't
        //       know if derived classes automatically add some vertices.
    { }

    template <typename T>
    void BasePointSet<T>::add(const Point2<T>& point)
    {
        _add(point);
        if (!intersect(_bbox, this->get(this->size() - 1)))
            _bboxdirty = true;
        _onVertexChanged();
    }

    template <typename T>
    void BasePointSet<T>::edit(size_t i, const Point2<T>& p)
    {
        _edit(i, p);
        _bboxdirty = true;
        _onVertexChanged();
    }

    template <typename T>
    void BasePointSet<T>::insert(size_t i, const Point2<T>& p)
    {
        _insert(i, p);
        if (!intersect(_bbox, this->get(i)))
            _bboxdirty = true;
        _onVertexChanged();
    }

    template <typename T>
    void BasePointSet<T>::remove(size_t i)
    {
        _remove(i);
        _bboxdirty = true;
        _onVertexChanged();
    }


    template <typename T>
    void BasePointSet<T>::clear()
    {
        _clear();
        _bboxdirty = true;
        _onVertexChanged();
    }

    template <typename T>
    AABB<T> BasePointSet<T>::getBBox() const
    {
        if (_bboxdirty)
        {
            _bbox = this->_calculateBBox();
            _bboxdirty = false;
        }
        return _bbox;
    }



    // PointSet
    template <typename T>
	PointSet<T>::PointSet(size_t capacity)
	{
        _vertices.reserve(capacity);
    }

    template <typename T>
	Point2<T> PointSet<T>::get(size_t i) const
	{
		return _vertices[i];
	}

    template <typename T>
	size_t PointSet<T>::size() const
	{
        return _vertices.size();
	}

    template <typename T>
	void PointSet<T>::_add(const Point2<T>& point)
	{
        _vertices.push_back(point);
	}

    template <typename T>
	void PointSet<T>::_clear()
	{
        _vertices.clear();
	}

    template <typename T>
	void PointSet<T>::_edit(size_t i, const Point2<T>& p)
	{
        _vertices[i] = p;
	}

    template <typename T>
	void PointSet<T>::_insert(size_t i, const Point2<T>& p)
	{
        _vertices.insert(_vertices.begin() + i, p);
	}

    template <typename T>
	void PointSet<T>::_remove(size_t i)
	{
        _vertices.erase(_vertices.begin() + i);
	}
}
#endif
