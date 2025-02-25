//
// Copyright 2016 Pixar
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
#ifndef PXRUSDMAYA_PRIMREADERCONTEXT_H
#define PXRUSDMAYA_PRIMREADERCONTEXT_H

#include <mayaUsd/base/api.h>

#include <pxr/pxr.h>
#include <pxr/usd/usd/prim.h>

#include <maya/MObject.h>

PXR_NAMESPACE_OPEN_SCOPE

/// \class UsdMayaPrimReaderContext
/// \brief This class provides an interface for reader plugins to communicate
/// state back to the core usd maya logic as well as retrieve information set by
/// other plugins.
///
/// Maya operations should be made directly with the Maya API.  Any additional
/// state that needs to be stored that isn't directly related to the Maya scene
/// should be stored here.  For example, we track objects that are added for
/// undo/redo.
///
/// We will likely need a mechanism where one plugin can invoke another one.
class UsdMayaPrimReaderContext
{
public:
    typedef std::map<std::string, MObject> ObjectRegistry;
    typedef TfSmallVector<MObject, 4>      MayaObjectList;

    MAYAUSD_CORE_PUBLIC
    UsdMayaPrimReaderContext(ObjectRegistry* pathNodeMap);

    /// \brief Returns the prim was registered at \p path.  If \p findAncestors
    /// is true and no object was found for \p path, this will return the object
    /// that corresponding to its nearest ancestor.
    ///
    /// Returns an invalid MObject if no such object exists.
    MAYAUSD_CORE_PUBLIC
    MObject GetMayaNode(const SdfPath& path, bool findAncestors) const;

    /// \brief Start tracking any new created node in a separate list.
    MAYAUSD_CORE_PUBLIC
    void StartNewMayaNodeTracking();

    /// \brief Return list of new Maya nodes being tracked.
    MAYAUSD_CORE_PUBLIC
    const MayaObjectList& GetTrackedNewMayaNodes() const;

    /// \brief Stop tracking new Maya nodes.
    MAYAUSD_CORE_PUBLIC
    void StopNewMayaNodeTracking();

    /// \brief Record \p mayaNode prim as being created \p path.
    ///
    /// Calling code may be interested in new objects being created.  Some
    /// reasons for this may be:
    /// - looking up later (for shader bindings, relationship targets, etc)
    /// - undo/redo purposes
    ///
    /// Plugins should call this as needed.
    MAYAUSD_CORE_PUBLIC
    void RegisterNewMayaNode(const std::string& path, const MObject& mayaNode) const;

    /// \brief returns true if prim traversal of the children of the current
    /// node can be pruned.
    MAYAUSD_CORE_PUBLIC
    bool GetPruneChildren() const;

    /// \brief If this plugin takes care of reading all of its children, it
    /// should SetPruneChildren(true).
    MAYAUSD_CORE_PUBLIC
    void SetPruneChildren(bool prune);

    /// \brief Return the time sample multiplier to convert from USD time to Maya time
    MAYAUSD_CORE_PUBLIC
    double GetTimeSampleMultiplier() const;

    /// \brief Set the time sample multiplier to convert from USD time to Maya time
    MAYAUSD_CORE_PUBLIC
    void SetTimeSampleMultiplier(double multiplier);

    /// \brief Set the if we are importing as edit as Maya
    MAYAUSD_CORE_PUBLIC
    void SetForceResetXform(bool resetXform);

    /// \Get the if we are importing as edit as Maya
    MAYAUSD_CORE_PUBLIC
    bool GetForceResetXform() const;

    ~UsdMayaPrimReaderContext() { }

private:
    bool   _prune;
    double _timeSampleMultiplier;
    // Added a boolean for
    bool _resetXform;

    // used to keep track of prims that are created.
    // for undo/redo
    ObjectRegistry* _pathNodeMap;

    // Tracks new nodes. It is possible that a code branch will decide to work on a copy of the
    // context, so wrap the tracker in a shared pointer.
    std::shared_ptr<MayaObjectList> _trackedNewMayaNodes;
};

PXR_NAMESPACE_CLOSE_SCOPE

#endif
