/*
 * changeevents.h
 * Copyright 2019, Thorbjørn Lindeijer <bjorn@lindeijer.nl>
 *
 * This file is part of Tiled.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program. If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include "mapobject.h"

#include <QList>

namespace Tiled {

class Layer;

class ChangeEvent
{
public:
    enum Type {
        LayerChanged,
        MapObjectAboutToBeAdded,
        MapObjectAboutToBeRemoved,
        MapObjectAdded,
        MapObjectRemoved,
        MapObjectsAboutToBeRemoved,
        MapObjectsAdded,
        MapObjectsChanged,
    } type;

protected:
    ChangeEvent(Type type)
        : type(type)
    {}
};

/**
 * Layer can change name, opacity, visible, locked or offset.
 */
class LayerChangeEvent : public ChangeEvent
{
public:
    enum LayerProperties {
        NameProperty            = 1 << 0,
        OpacityProperty         = 1 << 1,
        VisibleProperty         = 1 << 2,
        LockedProperty          = 1 << 3,
        OffsetProperty          = 1 << 4,
        AllProperties           = 0xFF
    };

    LayerChangeEvent(Layer *layer, int properties = AllProperties)
        : ChangeEvent(LayerChanged)
        , layer(layer)
        , properties(properties)
    {}

    Layer *layer;
    int properties;
};

class MapObjectsEvent : public ChangeEvent
{
public:
    MapObjectsEvent(Type type, QList<MapObject *> mapObjects)
        : ChangeEvent(type)
        , mapObjects(std::move(mapObjects))
    {}

    QList<MapObject *> mapObjects;
};

class MapObjectsChangeEvent : public MapObjectsEvent
{
public:
    MapObjectsChangeEvent(MapObject *mapObject,
                          MapObject::ChangedProperties properties = MapObject::AllProperties)
        : MapObjectsChangeEvent(QList<MapObject *> { mapObject }, properties)
    {}

    MapObjectsChangeEvent(QList<MapObject *> mapObjects,
                          MapObject::ChangedProperties properties = MapObject::AllProperties)
        : MapObjectsEvent(MapObjectsChanged, std::move(mapObjects))
        , properties(properties)
    {}

    MapObject::ChangedProperties properties;
};

class MapObjectEvent : public ChangeEvent
{
public:
    MapObjectEvent(Type type, ObjectGroup *objectGroup, int index)
        : ChangeEvent(type)
        , objectGroup(objectGroup)
        , index(index)
    {}

    ObjectGroup *objectGroup;
    int index;
};

} // namespace Tiled
