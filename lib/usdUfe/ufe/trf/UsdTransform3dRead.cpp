//
// Copyright 2022 Autodesk
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
#include "UsdTransform3dRead.h"

#include <usdUfe/ufe/Utils.h>

#include <pxr/usd/usdGeom/scope.h>

namespace USDUFE_NS_DEF {

// Ensure that UsdTransform3dRead is properly setup.
USDUFE_VERIFY_CLASS_SETUP(Ufe::Transform3dRead, UsdTransform3dRead);
USDUFE_VERIFY_CLASS_BASE(UsdTransform3dReadImpl, UsdTransform3dRead);

UsdTransform3dRead::Ptr UsdTransform3dRead::create(const UsdSceneItem::Ptr& item)
{
    return std::make_shared<UsdTransform3dRead>(item);
}

UsdTransform3dRead::UsdTransform3dRead(const UsdSceneItem::Ptr& item)
    : UsdTransform3dReadImpl(item)
    , Transform3dRead()
{
}

const Ufe::Path& UsdTransform3dRead::path() const { return UsdTransform3dReadImpl::path(); }

Ufe::SceneItem::Ptr UsdTransform3dRead::sceneItem() const
{
    return UsdTransform3dReadImpl::sceneItem();
}

Ufe::Matrix4d UsdTransform3dRead::matrix() const { return UsdTransform3dReadImpl::matrix(); }

Ufe::Matrix4d UsdTransform3dRead::segmentInclusiveMatrix() const
{
    return UsdTransform3dReadImpl::segmentInclusiveMatrix();
}

Ufe::Matrix4d UsdTransform3dRead::segmentExclusiveMatrix() const
{
    return UsdTransform3dReadImpl::segmentExclusiveMatrix();
}

//------------------------------------------------------------------------------
// UsdTransform3dReadHandler
//------------------------------------------------------------------------------

USDUFE_VERIFY_CLASS_SETUP(Ufe::Transform3dHandler, UsdTransform3dReadHandler);

UsdTransform3dReadHandler::UsdTransform3dReadHandler(
    const Ufe::Transform3dHandler::Ptr& nextHandler)
    : Ufe::Transform3dHandler()
    , _nextHandler(nextHandler)
{
}

/* static */
UsdTransform3dReadHandler::Ptr
UsdTransform3dReadHandler::create(const Ufe::Transform3dHandler::Ptr& nextHandler)
{
    return std::make_shared<UsdTransform3dReadHandler>(nextHandler);
}

namespace {

bool isUsdScope(const UsdSceneItem& usdItem)
{
    // Test the type of the USD prim, verify it is a scope.
    return PXR_NS::UsdGeomScope(usdItem.prim()) && !PXR_NS::UsdGeomXformable(usdItem.prim());
}

} // namespace

/* override */
Ufe::Transform3d::Ptr UsdTransform3dReadHandler::transform3d(const Ufe::SceneItem::Ptr& item) const
{
    return _nextHandler ? _nextHandler->transform3d(item) : nullptr;
}

/* override */
Ufe::Transform3dRead::Ptr
UsdTransform3dReadHandler::transform3dRead(const Ufe::SceneItem::Ptr& item) const
{
    auto usdItem = downcast(item);
    if (!usdItem || !isUsdScope(*usdItem))
        return _nextHandler ? _nextHandler->transform3dRead(item) : nullptr;

    return UsdTransform3dRead::create(usdItem);
}

/* override */
Ufe::Transform3d::Ptr UsdTransform3dReadHandler::editTransform3d(
    const Ufe::SceneItem::Ptr&      item,
    const Ufe::EditTransform3dHint& hint) const
{
    return _nextHandler ? _nextHandler->editTransform3d(item, hint) : nullptr;
}

} // namespace USDUFE_NS_DEF
