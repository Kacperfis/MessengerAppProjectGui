#pragma once
#include <memory>

#include "messengerInterfaces/ILoginHubFactory.hpp"
#include "messengerInterfaces/ILoginHub.hpp"
#include "AdminLoginHub.hpp"

namespace login
{

class AdminLoginHubCreator : public interface::ILoginHubFactory
{
public:
    std::shared_ptr<interface::ILoginHub> createHub() const override;
};

} // namespace login
