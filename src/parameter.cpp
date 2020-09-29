#include "parameter.h"

namespace php {
    parameter_iterator parameters::begin() const {
        return {this, 0};
    }

    parameter_iterator parameters::end() const {
        return {this, size()};
    }

    parameter_reverse_iterator parameters::rbegin() const {
        return {this, size() - 1};
    }
    
    parameter_reverse_iterator parameters::rend() const {
        return {this, -1};
    }
}