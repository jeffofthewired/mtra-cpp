#pragma once

namespace mtra {

template <typename>
class function;

template <typename R, typename... Args>
class function<R(Args...)> {
    // wtf does this line do?
    using invoker = R (*)(void*, Args&&...);
    

};

};
