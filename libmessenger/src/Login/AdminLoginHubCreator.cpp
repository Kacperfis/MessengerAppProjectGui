#include "AdminLoginHubCreator.hpp"
#include <memory>

namespace login
{

std::shared_ptr<interface::ILoginHub> AdminLoginHubCreator::createHub() const
{
    return std::make_shared<login::AdminLoginHub>();
}

} // namespace login
