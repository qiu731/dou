/* Copyright (c) 2023 Renmin University of China
RMDB is licensed under Mulan PSL v2.
You can use this software according to the terms and conditions of the Mulan PSL v2.
You may obtain a copy of Mulan PSL v2 at:
        http://license.coscl.org.cn/MulanPSL2
THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
See the Mulan PSL v2 for more details. */

#include "rm_scan.h"
#include "rm_file_handle.h"
/**
/**
 * @brief 初始化file_handle和rid
 * @param file_handle

RmScan::RmScan(const RmFileHandle *file_handle) : file_handle_(file_handle) {
    // Todo:
    // 初始化file_handle和rid（指向第一个存放了记录的位置）

}

/**
 * @brief 找到文件中下一个存放了记录的位置

void RmScan::next() {
    // Todo:
    // 找到文件中下一个存放了记录的非空闲位置，用rid_来指向这个位置

}

/**
 * @brief ​ 判断是否到达文件末尾

bool RmScan::is_end() const {
    // Todo: 修改返回值

    return false;
}


@brief RmScan内部存放的rid

Rid RmScan::rid() const {
    return rid_;
}
*/

#include "rm_scan.h"
#include "rm_file_handle.h"

RmScan::RmScan(const RmFileHandle *file_handle) : file_handle_(file_handle) {
    // 初始化rid为第一个存放记录的位置
    rid_.page_no = 0;
    rid_.slot_no = 0;
}

void RmScan::next() {
    int pageCount = file_handle_->get_file_hdr().num_pages;
    int slotCount = file_handle_->get_file_hdr().num_records_per_page;

    while (rid_.page_no < pageCount) {
        // 在当前页中找到下一个非空闲位置
        rid_.slot_no++;

        if (rid_.slot_no >= slotCount) {
            // 当前页已经扫描完，继续下一页
            rid_.page_no++;
            rid_.slot_no = 0;
        }

        if (rid_.page_no >= pageCount) {
            // 已经到达文件末尾，退出循环
            break;
        }

        if (file_handle_->is_record(rid_)) {
            // 找到非空闲位置，退出循环
            break;
        }
    }
}

bool RmScan::is_end() const {
    int pageCount = file_handle_->get_file_hdr().num_pages;
    int slotCount = file_handle_->get_file_hdr().num_records_per_page;

    return (rid_.page_no >= pageCount) || (rid_.page_no == pageCount - 1 && rid_.slot_no >= slotCount);
}

Rid RmScan::rid() const {
    return rid_;
}
