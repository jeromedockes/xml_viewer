#ifndef XML_VIEWER_UTILS_H
#define XML_VIEWER_UTILS_H

#include <memory>


namespace xml_viewer
{

    template<typename T, typename ...Args>
        std::unique_ptr<T> make_unique(Args&& ...args)
        {
           return std::unique_ptr<T>(new T(std::forward<Args>(args)... ));
        }
}

#endif
