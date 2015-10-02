#ifndef MATH_AABB_HPP
#define MATH_AABB_HPP

#include "Vector2.hpp"

// Microsoft compiler doesn't support constexpr
#ifdef _WIN32
#define constexpr
#endif

namespace geometry
{
    template <class T>
    class AABB
    {
        public:
            constexpr AABB()
            {};

            constexpr AABB(T x, T y, T w, T h) :
                    pos(x, y),
                    size(w, h)
            {};

            constexpr AABB(const Vector2<T>& pos_, const Vector2<T>& size_) :
                    pos(pos_),
                    size(size_)
            {};


        public:
            void move(const Vector2<T>& pos)
            {
                this->pos += pos;
            }

            void crop(const AABB<T>& rect)
            {
                if (intersects(rect))
                {
                    size += pos; //convert to "range"-rect

                    size.x = std::min(size.x, rect.pos.x + rect.size.x);
                    size.y = std::min(size.y, rect.pos.y + rect.size.y);

                    pos.x = std::max(pos.x, rect.pos.x);
                    pos.y = std::max(pos.y, rect.pos.y);

                    size -= pos;
                }
                else
                {
                    pos = size = Vector2<T>(); //null rect
                }
            };


            constexpr bool contains(const Vector2<T>& point) const
            {
                return point >= pos && point < pos + size;
            }

            constexpr bool contains(const AABB<T>& rect) const
            {
                return contains(rect.pos) && contains(rect.pos + rect.size);
            }

            constexpr bool intersects(const AABB<T>& rect) const
            {
                return  pos.x <= rect.pos.x + rect.size.x &&
                        pos.x + size.x >= rect.pos.x &&
                        pos.y <= rect.pos.y + rect.size.y &&
                        pos.y + size.y >= rect.pos.y;
            }


            constexpr bool operator!=(const AABB<T>& r) const
            {
                return pos != r.pos || size != r.size;
            }

            constexpr bool operator==(const AABB<T>& r) const
            {
                return pos == r.pos && size == r.size;
            }


            template <class T2>
            constexpr operator AABB<T2>() const
            {
                return AABB<T2>(static_cast<Vector2<T2>>(pos), static_cast<Vector2<T2>>(size));
            }


        public:
            Vector2<T> pos;
            Vector2<T> size;
    };
}
#endif

