#pragma once
#include <memory>

#include "ILoginHub.hpp"

namespace interface
{

class ILoginHubFactory
{
public:
    virtual ~ILoginHubFactory(){};
    virtual std::shared_ptr<ILoginHub> createHub() const = 0;
};

} // namespace interface
