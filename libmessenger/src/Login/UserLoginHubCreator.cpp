#include "UserLoginHubCreator.hpp"

namespace login
{

std::shared_ptr<interface::ILoginHub> UserLoginHubCreator::createHub() const
{
    return std::make_shared<login::UserLoginHub>();
}

} // namespace login
