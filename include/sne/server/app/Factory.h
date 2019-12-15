#pragma once

#include "DynamicObject.h"
#include <boost/dll/alias.hpp>
#include <boost/dll/import.hpp>
#include <memory>

namespace sne { namespace server {

using DynamicObjectPtr = std::shared_ptr<DynamicObject>;
typedef DynamicObjectPtr(SneAppFactoryCreator)();

#define DEFINE_SNE_APP_FACTORY(appClass) \
    namespace sne { namespace server { \
        struct SneAppFactory \
        { \
            static DynamicObjectPtr create() { \
                return std::make_shared<appClass>(); \
            } \
        }; \
    }} \
    BOOST_DLL_ALIAS(sne::server::SneAppFactory::create, sne_app_factory)


inline std::function<SneAppFactoryCreator> createAppFactory(const std::string& dllName)
{
    return boost::dll::import_alias<SneAppFactoryCreator>(dllName, "sne_app_factory");
}

}} // namespace sne { namespace server {
