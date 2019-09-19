/*
 * This source file is part of ArkNX
 * For the latest info, see https://github.com/ArkNX
 *
 * Copyright (c) 2013-2019 ArkNX authors.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#pragma once

#include "base/AFMacros.hpp"
#include "base/AFMap.hpp"
#include "base/AFList.hpp"
#include "base/AFDefine.hpp"

namespace ark {

class AFClassCallBackManager final
{
private:
    // class event list
    AFList<CLASS_EVENT_FUNCTOR> class_events_;

    // data call backs list
    using DataCallBacks = std::vector<DATA_EVENT_FUNCTOR>;
    AFNewHashmap<uint32_t, DataCallBacks> data_call_backs_list_;

    // table call backs list
    using TableCallBacks = std::vector<TABLE_EVENT_FUNCTOR>;
    AFNewHashmap<uint32_t, TableCallBacks> table_call_backs_list_;

    // container call backs list
    using ContainerCallBacks = std::vector<CONTAINER_EVENT_FUNCTOR>;
    AFNewHashmap<uint32_t, ContainerCallBacks> container_call_backs_list_;

public:
    explicit AFClassCallBackManager() = default;

    virtual ~AFClassCallBackManager()
    {
        class_events_.clear();
    }

    bool AddClassCallBack(CLASS_EVENT_FUNCTOR&& cb)
    {
        class_events_.emplace_back(std::forward<CLASS_EVENT_FUNCTOR>(cb));
        return true;
    }

    bool AddDataCallBack(const uint32_t index, DATA_EVENT_FUNCTOR&& cb)
    {
        auto iter = data_call_backs_list_.find(index);
        if (iter == data_call_backs_list_.end())
        {
            DataCallBacks* pDataCBs = ARK_NEW DataCallBacks;
            ARK_ASSERT_RET_VAL(pDataCBs != nullptr, false);

            pDataCBs->push_back(std::forward<DATA_EVENT_FUNCTOR>(cb));
            data_call_backs_list_.insert(index, pDataCBs);
        }
        else
        {
            iter->second->push_back(cb);
        }

        return true;
    }

    bool AddTableCallBack(const uint32_t index, TABLE_EVENT_FUNCTOR&& cb)
    {
        auto iter = table_call_backs_list_.find(index);
        if (iter == table_call_backs_list_.end())
        {
            TableCallBacks* pTableCBs = ARK_NEW TableCallBacks;
            ARK_ASSERT_RET_VAL(pTableCBs != nullptr, false);

            pTableCBs->push_back(std::forward<TABLE_EVENT_FUNCTOR>(cb));
            table_call_backs_list_.insert(index, pTableCBs);
        }
        else
        {
            iter->second->push_back(cb);
        }

        return true;
    }

    bool AddContainerCallBack(const uint32_t index, CONTAINER_EVENT_FUNCTOR&& cb)
    {
        auto iter = container_call_backs_list_.find(index);
        if (iter == container_call_backs_list_.end())
        {
            ContainerCallBacks* pDataCBs = ARK_NEW ContainerCallBacks;
            ARK_ASSERT_RET_VAL(pDataCBs != nullptr, false);

            pDataCBs->push_back(std::forward<CONTAINER_EVENT_FUNCTOR>(cb));
            container_call_backs_list_.insert(index, pDataCBs);
        }
        else
        {
            iter->second->push_back(cb);
        }

        return true;
    }

    bool DoEvent(
        const AFGUID& id, const std::string& class_name, const ArkEntityEvent eClassEvent, const AFIDataList& valueList)
    {
        for (auto iter : class_events_)
        {
            iter(id, class_name, eClassEvent, valueList);
        }

        return true;
    }

    bool OnDataCallBack(
        const AFGUID& self, const std::string& name, uint32_t index, const AFIData& old_data, const AFIData& new_data)
    {
        auto data_call_backs = data_call_backs_list_.find_value(index);
        if (data_call_backs == nullptr)
        {
            return false;
        }

        for (auto& cb : *data_call_backs)
        {
            cb(self, name, index, old_data, new_data);
        }

        return true;
    }

    bool OnTableCallBack(
        const AFGUID& id, const TABLE_EVENT_DATA& event_data, const AFIData& old_data, const AFIData& new_data)
    {
        auto table_call_backs = table_call_backs_list_.find_value(event_data.table_index_);
        if (table_call_backs == nullptr)
        {
            return false;
        }

        for (auto& cb : *table_call_backs)
        {
            cb(id, event_data, old_data, new_data);
        }

        return true;
    }

    bool OnContainerCallBack(const AFGUID& self, const uint32_t index, const ArkContainerOpType op_type,
        uint32_t src_index, uint32_t dest_index)
    {
        auto container_call_backs = container_call_backs_list_.find_value(index);
        if (container_call_backs == nullptr)
        {
            return false;
        }

        for (auto& cb : *container_call_backs)
        {
            cb(self, index, op_type, src_index, dest_index);
        }

        return true;
    }
};

} // namespace ark
