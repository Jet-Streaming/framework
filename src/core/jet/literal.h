#pragma once
#include <string.h>

namespace jet {

class literal
{
public:
    literal(const char *s, size_t l = -1)
        : _s(s), _l(l) {}
    ~literal() {}

    const char *data() const {
        return _s;
    }

    size_t len() {
        if (_l == -1) {
            _l = ::strlen(_s);
        }
        return _l;
    }

private:
    const char *_s;
    size_t _l;
};

inline bool
operator<(const literal &l, const literal &r)
{
    return ::strcmp(l.data(), r.data()) < 0;
}

} // jet